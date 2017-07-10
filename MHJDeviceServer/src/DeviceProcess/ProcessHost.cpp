/*
 * ProcessSwitch.cpp
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#include "ProcessHost.h"
#include "MHJProtocol.h"
#include "ProtocolHost.h"
#include "ProtocolApp.h"
#include "ProtocolDeviceSwitch.h"
#include "MHJSecurityManageHost.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "ProtocolServer.h"
//#include "DeviceServerManage.h"

#define cout std::cout
#define endl std::endl

ProcessHost::ProcessHost(SP_Request *request, SP_Response *response, shared_ptr<MHJProtocol> protocol) :
		ProcessServerFactory(request, response, protocol, MPT_HOST) {

}

ProcessHost::~ProcessHost() {

}

void ProcessHost::Process() {
	switch (protocol->protocolID()) {
	case MHPT_LOGIN:
		ProtocolLogin (protocol);
		break;
	case MHPT_HEARTBEAT:
		ProtocolHeartbeat(protocol);
		break;
	case MHPT_CLIENT_LOGIN:
		ProtocolClientLogin(protocol);
//	case MHPT_COMMON_HOST_TO_SERVER:
//		ProcessClientProtocol(protocol);
	default:
		//其他业务处理，转到java服务器
		this->ProcessServerFactory::ProtocolTransferToJava(protocol);
		break;
	}
}

void ProcessHost::Release() {
}

/*
 * 客户端数据区为内网地址列表。
 * TODO 需要根据客户端外网地址分配服务器，将同一个外网地址到设备集中到同一个服务器上，减少服务器之间通讯开销。
 * 1、先判断客户端相同外网地址到设备有没有链接到设备服务器
 * 2、如果有，则分配相同到外网服务器
 * 3、如果没有，则先分配空闲的服务器列表，由客户端自己ping速度决定
 *
 */
void ProcessHost::ProtocolLogin(shared_ptr<MHJProtocol> protocol) {
	if (MHPT_LOGIN_V1 == protocol->Ver()) {
		this->ProcessFactory::ProtocolLogin(protocol);
		MHPT_LOGIN_RESPONSE_V1 response;
		char sessionOutSecurity[32];
		protocol->createSessionSecurityData(sessionOutSecurity);
		memcpy(response.sessionSecurity, sessionOutSecurity, sizeof(response.sessionSecurity));
		response.serverCount = 0;

		if (protocol->length() > 0) {
			MHPT_LOGIN_SEND_V1* requestData = (MHPT_LOGIN_SEND_V1*) protocol->data();
			//TODO 调度服务器暂不处理内网地址
			char tmp[255];
			//			in_addr_t * ipaddress = (in_addr_t*) protocol->data();
			uint8_t* ip = (uint8_t*) &requestData->ip;
			sprintf(tmp, "设备登录客户端内网ip： %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
			cout << tmp << endl;
		} else {
			cout << "返回地址为空" << endl;
		}
//	shared_ptr<IPPORTADDRESS> ipport= DeviceServerManage::getAvailableServer();
		this->ProcessServerFactory::ProtocolLogin(protocol);

		ResponseProtocol(protocol->packID(), protocol->protocolID(), MHPT_LOGIN_V1, sizeof(MHPT_LOGIN_RESPONSE_V1), &response);

	} else
		cout << "ProtocolLogin:未知版本号 " << endl;
}

void ProcessHost::ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol) {
	if (MHPT_HEARTBEAT_V1 == protocol->Ver()) {
		if (protocol->length() > 0) {
			cout << "ProtocolHeartbeat" << endl;
		} else {
			cout << "心跳数据为空" << endl;
		}
//	shared_ptr<IPPORTADDRESS> ipport= DeviceServerManage::getAvailableServer();
		ResponseProtocol(protocol->packID(), protocol->protocolID(), MHPT_HEARTBEAT_V1, NULL, NULL);
	} else
		cout << "ProtocolHeartbeat:未知版本号 " << endl;
}

void ProcessHost::ProtocolClientLogin(shared_ptr<MHJProtocol> protocol) {
	if (MHPT_CLIENT_LOGIN_V1 == protocol->Ver()) {
		shared_ptr < MHJProtocol > loginProtocol = make_shared < MHJProtocol > (protocol->data(), protocol->length(), protocol->getSecurityFactory());
		if (loginProtocol->isValid()) {
			//解析出登录协议，产生会话安全码，并发送给主机，同时产生返回协议，由主机转发给设备
			shared_ptr < SP_Buffer > clientResponseBuffer;
			if (loginProtocol->protocolType() == MPT_APP) {
				MAPT_LOGIN_RESPONSE_V1 responseData;
				responseData.serverCount = 0;
				loginProtocol->createSessionSecurityData((char*) responseData.sessionSecurity);
				clientResponseBuffer = loginProtocol->getSendBuffer(loginProtocol->protocolType(), loginProtocol->device(), loginProtocol->packID(),
						loginProtocol->protocolID(), MAPT_LOGIN_V1, sizeof(responseData), &responseData);
			} else if (loginProtocol->protocolType() == MPT_SWITCH) {
				MDPTS_LOGIN_RESPONSE_V1 responseData;
				responseData.serverCount = 0;
				loginProtocol->createSessionSecurityData((char*) responseData.sessionSecurity);
				clientResponseBuffer = loginProtocol->getSendBuffer(loginProtocol->protocolType(), loginProtocol->device(), loginProtocol->packID(),
						loginProtocol->protocolID(), MDPTS_LOGIN_V1, sizeof(responseData), &responseData);
			}

//			MHJSecurityManageHost* hostdatabaseManage = (MHJSecurityManageHost*) protocol->getDatabaseManage();
//			MHJSecurityManageFactory * databaseManage = (MHJSecurityManageFactory*) protocol->getDatabaseManage();
			//未加密的会话安全码
			char* clientsessionSecurity = protocol->getSecurityFactory()->getSecurity(loginProtocol->device(), 2);
			SP_Buffer senddata;
			senddata.append(clientsessionSecurity, SECURITYSIZE);
			senddata.append(clientResponseBuffer.get());
			ResponseProtocol(protocol->packID(), protocol->protocolID(), MHPT_CLIENT_LOGIN_V1, senddata.getSize(), (void*) senddata.getBuffer());

			ProtocolHostClientProtocolLogin(loginProtocol);
		}
	} else
		cout << "ProtocolHeartbeat:未知版本号 " << endl;
}

/**
 * 负责处理在线消息及其他服务端
 */
void ProcessHost::ProtocolHostClientProtocolLogin(
		shared_ptr<MHJProtocol> clientprotocol) {

		MHJDeviceMark *hostdevicemark = protocol->device();
		char strhostmark[32];
		memset(strhostmark,0,sizeof(strhostmark));
		sprintMark(strhostmark,hostdevicemark);

		ProtocolOffline(clientprotocol);
		MHJDeviceMark *devicemark = clientprotocol->device();
		char value[64];
		sprintf(value, "OnlineDevice_Type_%d_ID_%d", devicemark->deviceType, devicemark->deviceID);
		MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
//		char* serviceID = redisfactory->getServiceID();
	//	char* deviceList = redisfactory->getDeviceList();

		redisReply* reply = NULL;

		//将设备添加到Redis中的本机设备列表
		reply = redisfactory->RedisCommand_RPUSH(strhostmark, value);
		redisfactory->RedisCommand_FREE(reply);
		//新建设备key值，key值指向本机服务号
		reply = redisfactory->RedisCommand_SET(value, strhostmark);
		redisfactory->RedisCommand_FREE(reply);

}
