/*
 * ProcessHostFactory.cpp
 *
 *  Created on: 2015-7-10
 *      Author: waroy
 */

#include <MHJConfigFactory.h>
#include "ProcessServerFactory.h"
#include "ProcessHost.h"
//#include "ProcessServer.h"
#include "ProcessSwitch.h"
#include "ProcessAPP.h"
#include "ServiceNum2KeyMap.h"
#include "MHJRedisCmd.h"
#include "MHJFunctionTimeRecord.h"
#include "MHJRedisBaseFactory.h"
#include "MHJFuncTimeRecordMap.h"
#include "ServiceNum2KeyMap.h"
#include "MHJRedisSubscribeFactory.h"
#include "ProtocolServer.h"
#include <iostream>
#include "MHJSecurityManageAPP.h"
#include "MHJSecurityManageDevice.h"
#include <stdio.h>
#include <string>
#include <json/json.h>
#include <DeviceProcess/MHJMqtt.h>
using namespace boost;
using  namespace std;
//需要用到mDeviceManage，但是初始化并不一致。
MHJDeviceManage ProcessServerFactory::mDeviceManage;

ProcessServerFactory::ProcessServerFactory(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType classProtocolType) :
		ProcessFactory(request, response, protocol, classProtocolType) {

}

ProcessServerFactory::~ProcessServerFactory() {

}

ProcessFactory *ProcessServerFactory::getProcess(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol) {
	ProcessFactory *process = NULL;

	//时间戳
//	time_t retime;
//	time ( &retime );

	struct  timeval tv_receTime;
	gettimeofday(&tv_receTime,NULL);

	switch (protocol->protocolType()) {
	case MPT_SERVER:
//		process = new ProcessServer(request, response, protocol);
		break;
	case MPT_SWITCH:
		process = new ProcessSwitch(request, response, protocol);
		break;
	case MPT_HOST:
		process = new ProcessHost(request, response, protocol);
		break;
	case MPT_APP:
		process = new ProcessAPP(request, response, protocol);
		break;
	default:
		break;
	}


	//TODO 记录接受到的指令
	// 协议ID  设备Mark  设备ID 设备类型 版本号
	// key = serverReceiveCommd
	// value = {"protocolID":xxx,"deviceIDA":xxx,"deviceIDB":xxx,"protocolType":xxx,"protocolVer":xxx}
	//"{\"protocolID\":"+ protocol->protocolID() + ",\"deviceIDA\":" + protocol->device()->deviceID + ",\"deviceIDB\":" + protocol->deviceID() + ",\"protocolType\":" + protocol->protocolType() + ",\"protocolVer\"" + protocol->Ver();

//
//	time_t overtime;
//	time ( &overtime );

	struct  timeval tv_overTime;
	gettimeofday(&tv_overTime,NULL);

	Json::Value root;
	Json::FastWriter writer;

	char strMark[32] = {0};

	root["protocolID"] = protocol->protocolID();

	sprintMark(strMark, protocol->device());
	root["sourceDevice"]  =strMark;
	root["targedeviceID"] =  protocol->deviceID();
	root["protocolType"] =  protocol->protocolType();
	root["protocolVer"] = protocol->Ver();
	root["receivetime"] =  Json::Value((Json::Value::UInt64)(tv_receTime.tv_sec*1000 + tv_receTime.tv_usec/1000));
	root["overtime"]    = Json::Value((Json::Value::UInt64)(tv_overTime.tv_sec*1000 + tv_overTime.tv_usec/1000));

	string receiveDataToRedis = writer.write(root);

	// 打印显示
		cout << "receiveDataToRedis " << receiveDataToRedis << endl;

	//写入readis
	MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* receiveDeviceInstructionList = redisfactory->getReceiveDeviceInstructionList();

	const char * buf = receiveDataToRedis.c_str();
	char *value = new char[strlen(buf)+1];
	strcpy(value, buf);

	redisReply* reply = redisfactory->RedisCommand_LPUSH(receiveDeviceInstructionList, value);
	redisfactory->RedisCommand_FREE(reply);
	free(value);
	buf = NULL;


	redisReply* reply_delete;
	reply_delete = (redisReply*) redisfactory->RedisCommand_LLEN(redisfactory->getReceiveDeviceInstructionList());
	int queuesize = reply_delete->integer;
	redisfactory->RedisCommand_FREE(reply_delete);
	if (queuesize > 50000) {
		for (int i = 0; i < 4000; i++) {
			reply_delete = (redisReply*) redisfactory->RedisCommand_LPOP(redisfactory->getReceiveDeviceInstructionList());
			redisfactory->RedisCommand_FREE(reply);
		}
	}
	//MHJRedisSubscribeFactory::publish(MSPT_SERVERINFO, this->serviceID, NULL, sizeof(MSPT_SERVERINFO_REDIS_V1), &senddata, this->channel);




//TODO：计算函数执行时间，便于统计不同的设备耗时
	return process;
}

void ProcessServerFactory::setDatabaseFactory(MHJDataBaseFactory* dataBaseFactory) {
	mDeviceManage.setDataBaseFactory(dataBaseFactory);
}

MHJDataBaseFactory* ProcessServerFactory::getDatabaseFactory() {
	return mDeviceManage.getDataBaseFactory();
}

void ProcessServerFactory::Release(void* ptr) {
	//执行release函数时， 将函数执行时间写入Redis
	MHJFunctionTimeRecorder *recoder = (MHJFunctionTimeRecorder*) ptr;
	MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	FunctionTimeRecord* start_addr = recoder->getStart();
	redisReply* reply;
	int account = recoder->getAccount();
	for (int i = 0; i < account; i++) {
		reply = (redisReply*) redisfactory->RedisCommand_RPUSH(redisfactory->getTimeRecodlist(), start_addr[i].record);
		redisfactory->RedisCommand_FREE(reply);
	}

	reply = (redisReply*) redisfactory->RedisCommand_LLEN(redisfactory->getTimeRecodlist());
	int queuesize = reply->integer;
	redisfactory->RedisCommand_FREE(reply);
	if (queuesize > 50000) {
		for (int i = 0; i < 4000; i++) {
			reply = (redisReply*) redisfactory->RedisCommand_LPOP(redisfactory->getTimeRecodlist());
			redisfactory->RedisCommand_FREE(reply);
		}
	}

}

void ProcessServerFactory::ProtocolOffline(boost::shared_ptr<MHJProtocol> protocol) {
	MHJDeviceMark *devicemark = protocol->device();
	char value[64];
	sprintf(value, "OnlineDevice_Type_%d_ID_%d", devicemark->deviceType, devicemark->deviceID);
	MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* serviceID = redisfactory->getServiceID();
	//	char* deviceList = redisfactory->getDeviceList();

	redisReply* reply = NULL;

	//发布在线信息，如果设备之前已登陆其他服务器，则该服务器收到后会删除跟设备相关的sid
	reply = redisfactory->RedisCommand_GET(value);
	if (reply->str != NULL) {
#ifdef DEBUG
		std::cout << "设备上线,Type:" << devicemark->deviceType << " Id:"<<devicemark->deviceID<< " 原服务号:" << reply->str << std::endl;
#endif
		MHJRedisSubscribeFactory::publish(MSPT_ONLINEINFORM, serviceID, devicemark, sizeof(MHJDeviceMark), devicemark, reply->str);

		//从redis服务列表中删除
		redisReply* replyRemove = redisfactory->RedisCommand_LREMOVEALL(reply->str, value);
		redisfactory->RedisCommand_FREE(replyRemove);

		redisfactory->RedisCommand_FREE(reply);
	} else {
#ifdef DEBUG
		std::cout << "设备新上线,Type:" << devicemark->deviceType << " Id:"<<devicemark->deviceID<< std::endl;
#endif
	}

//		//设备已登陆本机
//		if (redisfactory->isKeyInList(serviceID, value)) {
//			//删除本机设备列表中该设备的信息
//			reply = redisfactory->RedisCommand_LREMOVEALL(serviceID, value);
//			redisfactory->RedisCommand_FREE(reply);
//		}
}

void ProcessServerFactory::ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol) {
	ProtocolOffline(protocol);
	MHJDeviceMark *devicemark = protocol->device();
	char value[64];
	sprintf(value, "OnlineDevice_Type_%d_ID_%d", devicemark->deviceType, devicemark->deviceID);
	MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* serviceID = redisfactory->getServiceID();
//	char* deviceList = redisfactory->getDeviceList();

	redisReply* reply = NULL;

	//将设备添加到Redis中的本机设备列表
	reply = redisfactory->RedisCommand_RPUSH(serviceID, value);
	redisfactory->RedisCommand_FREE(reply);
	//新建设备key值，key值指向本机服务号
	reply = redisfactory->RedisCommand_SET(value, serviceID);
	redisfactory->RedisCommand_FREE(reply);

}

void ProcessServerFactory::clientClose(SP_Sid_t sid) {
	SP_OnlineInfo_t*clientinfo = mOnlineManager->getClientInfo(sid);
//	if(clientinfo!=NULL&&clientinfo->mClientMark.deviceID!=0){
//		MHJDeviceMark *devicemark =&clientinfo->mClientMark;
//		MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
//		char* serviceID = redisfactory->getServiceID();
//		char* deviceList = redisfactory->getDeviceList();
//		char value[64];
//		sprintf(value,"OnlineDevice_Type_%d_%d",devicemark->deviceType,devicemark->deviceID);
//
//		redisReply* reply =NULL;
//		//1，删除ServerID映射的Device
//		reply = redisfactory->RedisCommand_LREMOVEALL(serviceID,value);
//		redisfactory->RedisCommand_FREE(reply);
//		//2，删除DeviceID映射的Service
//		reply = redisfactory->RedisCommand_DEL(value);
//		redisfactory->RedisCommand_FREE(reply);
//		reply = redisfactory->RedisCommand_LREMOVEALL(deviceList,value);
//		redisfactory->RedisCommand_FREE(reply);
//	}

	this->ProcessFactory::clientClose(sid);
//	char key[64];
//	memset(key, 0, 64 * sizeof(char));
//	sprintf(key,"%s", ServiceNum2KeyMap::devicelist(redisfactory->getServiceID()).c_str());
//	redisReply* reply = redisfactory->RedisCommand_LRANGE(key);
//	int counter = reply->elements;
//	redisfactory->RedisCommand_FREE(reply);
//
//	for(int i=0; i<counter; i++){
//		reply = redisfactory->RedisCommand_LPOP(key);
//		redisfactory->RedisCommand_FREE(reply);
//	}

}

//将待传输数据push到Redis的指定列表中
void ProcessServerFactory::ProtocolTransferToJava(boost::shared_ptr<MHJProtocol> protocol) {
	int buffSize = protocol->getBufferSize() - 2;
	void* mbuffer = protocol->getBuffer();
	MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* serviceID = redisfactory->getServiceID();
	MHJDeviceMark* srcDevice = protocol->device();
	boost::shared_ptr < SP_Buffer > buffer = MHJRedisSubscribeFactory::RedisSubscribeProtocolPackage(MSPT_TOJAVASERVER, serviceID, srcDevice, buffSize,
			(void*) mbuffer);

	uint8_t* ptr = (uint8_t*) buffer->getRawBuffer();
	char* sent = new char[buffer->getSize() * 2 + 1];

	for (unsigned int i = 0; i < buffer->getSize(); i++) {
		sprintf(&sent[i * 2], "%02x", ptr[i]);
//		printf("%02x", ptr[i]);
	}
//	printf("\n");
	sent[buffer->getSize() * 2] = '\0'; //Redis是以命令行的形式发送字符串的，因而在结尾添加字符串结束符

	MHJRedisBaseFactory* redisbase = MHJRedisBaseFactory::GetRedisFactory();
	char* javaBlpop = redisbase->getJavaBlpop();

	if (0) //开启测试通道会导致服务器报错。暂时关闭。
	{ //如果指定了测试通道，则使用测试通道
		char DBSetChnnel[33];
		if (protocol->device()->deviceType == MDT_APP) {
			MHJSecurityManageAPP* databaseApp = (MHJSecurityManageAPP*) protocol->getDatabaseManage();
			std::tr1::shared_ptr < MHJUserEntity > user = databaseApp->getDatabaseModel(protocol->device());
			if (user->serverChnnel.length() > 0) {
				strcpy(DBSetChnnel, user->serverChnnel.c_str());
				javaBlpop = DBSetChnnel;
			}
		} else
//			if (protocol->device()->deviceType == MDT_SWITCH || protocol->device()->deviceType == MDT_HOST || protocol->device()->deviceType == MDT_CURTAIN
//				|| protocol->device()->deviceType >= MDT_BluetoothDOOR)
		{
			MHJSecurityManageDevice* databaseManage = (MHJSecurityManageDevice*) protocol->getDatabaseManage();
			std::tr1::shared_ptr < MHJDeviceBase > devicebase = databaseManage->getDatabaseModel(protocol->device());
			if (devicebase->serverChnnel.length() > 0) {
				strcpy(DBSetChnnel, devicebase->serverChnnel.c_str());
				javaBlpop = DBSetChnnel;
			}
		}
	}
//	redisReply* reply = redisbase->RedisCommand_RPUSH(javaBlpop,sent);

	redisReply* reply = redisbase->RedisCommand_LPUSH(javaBlpop, sent);
	std::cout << "推送到Redis列表 " << javaBlpop << "数据：" << sent << std::endl;
	if (reply != NULL) {
		redisbase->RedisCommand_FREE(reply);
	}
	delete[] (char*) sent;
}

