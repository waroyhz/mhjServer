/*
 * HostClientProcess.cpp
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#include "ServerClientProcess.h"
#include "sputils.hpp"
#include "MHJMsgDecoder.h"
#include "ProtocolServer.h"
#include "Tools/HostAddress.h"
#include <stdio.h>

using namespace boost;

ServerClientProcess::ServerClientProcess(const char* ip, uint16_t port, MHJDeviceMark *device) :
		HostClientFactory(device, MPT_SERVER, ip, port) {

}

ServerClientProcess::~ServerClientProcess() {

}

void ServerClientProcess::sendLogin(MHJHostClient *client) {
	//	 this->HostClientFactory::sendLogin(client);
	mIsLogin=false;
	mclient = client;
	char ipaddress[PACKDATAMAXSIZE];
	int size = HostAddress::getAllAddress(ipaddress, PACKDATAMAXSIZE);
	//	int size = HostAddress::getUsefulIPAddress(ipaddress, PACKDATAMAXSIZE);
	sendProtocol(MSPT_LOGIN, MSPT_LOGIN_V1, size, (BYTE*) ipaddress);
	std::cout << "发送登录协议 " << std::endl;
}

void ServerClientProcess::disConnect(MHJHostClient *client) {

}
void ServerClientProcess::sendheartbeat(MHJHostClient *client) {
	if (mIsLogin)
		sendProtocol(MSPT_HEARTBEAT, MSPT_HEARTBEAT_V1, 0, NULL);
}
void ServerClientProcess::revice(SP_MsgDecoder* msgDecoder, MHJHostClient* client) {
	MHJ_Device_MsgDecoder *decoder = (MHJ_Device_MsgDecoder *) msgDecoder;
	SP_CircleQueue * mqueue = decoder->getQueue();
	QueueData* queuedata = (QueueData*) mqueue->pop();

	switch (queuedata->Data->protocolID()) {
	case MSPT_LOGIN: {
		ProtocolLogin(queuedata->Data);
	}
		break;
	case MSPT_HEARTBEAT:
		ProtocolHeartbeat(queuedata->Data);
		break;
	}

	delete queuedata;
}

void ServerClientProcess::ProtocolLogin(shared_ptr<MHJProtocol> protocol) {
	if (MSPT_LOGIN_V1 == protocol->Ver()) {
		MSPT_LOGIN_RESPONSE_V1* response = (MSPT_LOGIN_RESPONSE_V1*) protocol->data();

#ifdef DEBUG
		char tmp[255]= {0};
		char tmpc[10];
		for(int i=0;i<32;i++)
		{
			sprintf(tmpc,"%02x",response->sessionSecurity[i]);
			strcat(tmp,tmpc);
		}
		printf(" ServerClientProcess 收到随机安全码：%s \n",tmp);
#endif

		mSecurity->setSessionSecurityData(response->sessionSecurity);
		mIsLogin = true;
		std::cout << "登录响应" << std::endl;
	} else
		std::cout << "未处理的版本" << std::endl;
}

void ServerClientProcess::ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol) {
	std::cout << "心跳响应" << std::endl;
}
