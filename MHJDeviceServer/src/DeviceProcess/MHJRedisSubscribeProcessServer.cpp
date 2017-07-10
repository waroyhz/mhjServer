/*
 * MHJRedisSubscribeProcessServer.cpp
 *
 *  Created on: 2015年11月21日
 *      Author: ruancongming
 */

#include "MHJRedisSubscribeProcessServer.h"
#include <iostream>
#include "MHJRedisBaseFactory.h"
#include "SP_OnlineManager.h"
#include "ProcessFactory.h"
#include "ProtocolDeviceSwitch.h"
#include "ProtocolServer.h"
//#include "MHJSecurityFactory.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <json/json.h>
using namespace boost;

MHJRedisSubscribeProcessServer::MHJRedisSubscribeProcessServer(MHJSecurityFactoryServer* securityfactoryserver, SP_Server* spserver) :
		MHJRedisSubscribeProcessFactory(spserver) {
	// TODO Auto-generated constructor stub
	this->securityfactoryserver = securityfactoryserver;
}

MHJRedisSubscribeProcessServer::~MHJRedisSubscribeProcessServer() {
	// TODO Auto-generated destructor stub
}

/*
 * 处理发布的命令
 * source：命令来源
 * devieMark：需要操作的目的设备
 * mbuffer：目的设备执行的数据
 */
void MHJRedisSubscribeProcessServer::ProcessCommand(char* source, shared_ptr<SP_Buffer> mbuffer) {
	//时间戳
//	time_t retime;
//	time ( &retime );

	struct  timeval tv_receTime;
	gettimeofday(&tv_receTime,NULL);


	MHJSUBSCRIBETRANSFERProtocol* recivedata = (MHJSUBSCRIBETRANSFERProtocol*) mbuffer->getRawBuffer();
	int size = mbuffer->getSize();
	if (recivedata == NULL) {
#ifdef DEBUG
		std::cout << "Warning:  Pointer is NULL!" << "In File:"<< __FILE__ <<"On Line:"<<__LINE__ <<std::endl;
#endif
		return;
	}
	MHJDeviceMark* deviceMark = &recivedata->dstdevice;
#ifdef DEBUG
	std::cout << "待转发的目的设备Type=" << deviceMark->deviceType << "  ID="<< deviceMark->deviceID << std::endl;
#endif

#ifdef DEBUG
	char inf[255];
	sprintf(inf,"type=%d,deviceid=%d,protocol=%d",recivedata->dstdevice.deviceType,recivedata->dstdevice.deviceID,recivedata->mprotocolID);
	switch (deviceMark->deviceType)
	{
		case MDT_APP:
			std::cout << "转发给APP" << inf<< std::endl;
		break;
		case MDT_ADMIN:
			std::cout << "转发给ADMIN" << inf<< std::endl;
		break;
		case MDT_SERVER:
			std::cout << "转发给SERVER" << inf<< std::endl;
		break;
		case MDT_SWITCH:
			std::cout << "转发给SWITCH" << inf<< std::endl;
		break;
		case MDT_HOST:
			std::cout << "转发给HOST" << inf<< std::endl;
		break;
	}
#endif

	MHJProtocol* protocol = new MHJProtocol(securityfactoryserver);
	shared_ptr < SP_Buffer > outBuffer = protocol->getSendBuffer(recivedata->mprotocolType, &recivedata->dstdevice, recivedata->mpackID,
			recivedata->mprotocolID, recivedata->mver, size - (sizeof(MHJSUBSCRIBETRANSFERProtocol) - sizeof(BYTE)), (void*) &recivedata->data);

	bool success = true;

	if (outBuffer.get() == NULL) {
#ifdef DEBUG
		std::cout << "Warning:  Pointer is NULL!" << "In File:"<< __FILE__ <<"On Line:"<<__LINE__ <<std::endl;
#endif
		success = false;
	}
	else {
		SP_Sid_t *sid = ProcessFactory::getOnlimeManager()->getSid(deviceMark);
		if (sid == NULL) {
#ifdef DEBUG
			std::cout << "Warning:  Pointer is NULL!" << "In File:" << __FILE__ << "On Line:" << __LINE__ << std::endl;
#endif
			success = false;
		}


		if (success) {
			this->MHJRedisSubscribeProcessFactory::pushSPServerResponseMessage(sid, outBuffer.get());
		}


		//TODO 记录数据传递结果
//	BYTE mprotocolType;          //协议类型
//	MHJDeviceMark dstdevice;  //目标设备
//	BYTE mprotocolID;			 //协议ID
//	BYTE mver;						 //协议版本号

//	time_t overtime;
//	time ( &overtime );

		struct timeval tv_overTime;
		gettimeofday(&tv_overTime, NULL);

		Json::Value root;
		Json::FastWriter writer;
		if (success) {
			root["isSucess"] = "Sucess";
		} else {
			root["isSucess"] = "failed";
		}


		root["mprotocolType"] = recivedata->mprotocolType;
		root["dstdeviceID"] = recivedata->dstdevice.deviceID;
		root["mprotocolID"] = recivedata->mprotocolID;
		root["mver"] = recivedata->mver;
		root["receivetime"] = Json::Value(
				(Json::Value::UInt64) (tv_receTime.tv_sec * 1000 + tv_receTime.tv_usec / 1000));
		root["overtime"] = Json::Value((Json::Value::UInt64) (tv_overTime.tv_sec * 1000 + tv_overTime.tv_usec / 1000));

		std::string receiveDataToRedis = writer.write(root);

		// 打印显示
		std::cout << "receiveDataToRedis " << receiveDataToRedis << std::endl;


		//写入readis
		MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
		char *ReceiveDeviceInstructionIsSuccessList = redisfactory->getReceiveDeviceInstructionIsSuccessList();

		const char *buf = receiveDataToRedis.c_str();
		char *value = new char[strlen(buf) + 1];
		strcpy(value, buf);
		redisReply *reply = redisfactory->RedisCommand_LPUSH(ReceiveDeviceInstructionIsSuccessList, value);
		redisfactory->RedisCommand_FREE(reply);
		ReceiveDeviceInstructionIsSuccessList = NULL;

		redisReply *reply_delete;
		reply_delete = (redisReply *) redisfactory->RedisCommand_LLEN(
				redisfactory->getReceiveDeviceInstructionIsSuccessList());
		int queuesize = reply_delete->integer;
		redisfactory->RedisCommand_FREE(reply_delete);
		if (queuesize > 50000) {
			for (int i = 0; i < 4000; i++) {
				reply_delete = (redisReply *) redisfactory->RedisCommand_LPOP(
						redisfactory->getReceiveDeviceInstructionIsSuccessList());
				redisfactory->RedisCommand_FREE(reply);
			}
		}


	}
	delete protocol;
//	} else {
//#ifdef DEBUG
//		cout << "待转发的目的设备为空，Do Nothing！" << endl;
//#endif
//	}
}

/*
 * 处理发布的设备在线信息：
 * 设备已登陆其他服务器，删除sid
 */
void MHJRedisSubscribeProcessServer::ProcessOnlineInform(char* source, shared_ptr<SP_Buffer> mbuffer) {
	MHJDeviceMark* deviceMark = (MHJDeviceMark*) mbuffer->getRawBuffer();

	MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* ServerNum = redisfactory->getServiceID();
	//设备已登陆其他服务器的通知
	if (strcmp(source, ServerNum) != 0) {
		if (deviceMark != NULL) {
#ifdef DEBUG
			std::cout << "收到设备登陆其他服务器的通知,已登陆服务号："<<source <<"设备  Type:" << deviceMark->deviceType << "  Id:"<<deviceMark->deviceID<< std::endl;
			std::cout << "删除本机设备列表中该设备的信息"<<std::endl;
#endif
			//删除本机设备列表中该设备的信息
			char value[64];
			sprintf(value, "OnlineDevice_Type_%d_ID_%d", deviceMark->deviceType, deviceMark->deviceID);
			redisReply* reply = NULL;
			reply = redisfactory->RedisCommand_LREMOVEALL(ServerNum, value);
			redisfactory->RedisCommand_FREE(reply);

#ifdef DEBUG
			std::cout << "删除设备上一次登陆本机的会话Id"<<std::endl;
#endif
			SP_Sid_t * sid = ProcessFactory::getOnlimeManager()->getSid(deviceMark);
			if (sid == NULL) {
#ifdef DEBUG
				std::cout << "会话ID为空! " << std::endl;
#endif
				return;
			} else {
#ifdef DEBUG
				std::cout << "删除会话ID"<< std::endl;
#endif
				ProcessFactory::getOnlimeManager()->remove(*sid);
			}
		}
	} else {
#ifdef DEBUG
		std::cout << "来自本机的上线通知"<< std::endl;
#endif
	}
}

//void MHJRedisSubscribeProcessServer::pushSPServerResponseMessage(SP_Sid_t* sid,
//		const SP_Buffer* buff) {
////	SP_Response * response = new SP_Response(*sid);
////	SP_Message * message = new SP_Message();
////	message->getMsg()->append(buff);
////	message->getToList()->add(*sid);
////	response->addMessage(message);
////	pspserver->pushResponseQueue(response);
//}
