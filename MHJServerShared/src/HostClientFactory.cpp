/*
 * HostClientFactory.cpp
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#include "HostClientFactory.h"
#include "ProtocolHost.h"
#include "Tools/HostAddress.h"
#include "Protocol.h"
#include <string.h>
#include "Tools/stringTools.h"

HostClientFactory::HostClientFactory(MHJDeviceMark *device, MHJProtocolType protocolType, const char*defautlIP, UINT16 defaultPort) {
	ip = defautlIP;
	prot = defaultPort;
//	memcpy(&mdeviceID, "A000100A", 8);
	//TODO 从配置文件读取安全码
//	char tmp[32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
//			18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
//	memcpy(mSecurityData, tmp, sizeof(tmp));
	this->protocolType = protocolType;
	mdevice = device;
	mIsLogin = false;
}

HostClientFactory::~HostClientFactory() {

}

void HostClientFactory::sendLogin(MHJHostClient *client) {
	mIsLogin = false;
	mclient = client;
	char ipaddress[255];
	int size = HostAddress::getAllAddress(ipaddress, sizeof(ipaddress));
	sendProtocol(MHPT_LOGIN, MHPT_LOGIN_V1, size, (BYTE*) ipaddress);
	std::cout << "发送登录协议 " << std::endl;
}
void HostClientFactory::disConnect(MHJHostClient *client) {
	mclient = NULL;
//	addConnectIndex();
}
void HostClientFactory::sendheartbeat(MHJHostClient *client) {
	//					tcp->SendData((void*) "hello", 5);
}
std::string HostClientFactory::getConnectHost() {
	if (mIpConnectIndex == mIpVector.size())
		return ip;
	else {
		IPPORTADDRESS ipport = mIpVector[mIpConnectIndex];
		std::string strip = format("%d.%d.%d.%d", ((BYTE*) (&ipport.ip))[0], ((BYTE*) (&ipport.ip))[1], ((BYTE*) (&ipport.ip))[2], ((BYTE*) (&ipport.ip))[3]);
		return strip;
	}
}
int HostClientFactory::getConnectProt() {
	if (mIpConnectIndex == mIpVector.size())
		return prot;
	else {
		IPPORTADDRESS ipport = mIpVector[mIpConnectIndex];
		return ipport.prot;
	}
}

void HostClientFactory::revice(SP_MsgDecoder *msgDecoder, MHJHostClient* client) {

}

void HostClientFactory::sendProtocol(BYTE mProtocolID, BYTE mver, BYTE mlength, BYTE *mdata) {
	mclient->sendProtocol(protocolType, mdevice, mpackID++, mProtocolID, mver, mlength, mdata);
}

void HostClientFactory::Close() {
	mclient->Close();
}

void HostClientFactory::addConnectIndex() {
	mIpConnectIndex++;
	if (mIpConnectIndex > mIpVector.size()) {
		mIpConnectIndex = 0;
	}
}

void HostClientFactory::setSecurity(MHJSecurityFactory* security) {
	mSecurity = security;
}

bool HostClientFactory::isLogin(BYTE protocolID) {
	if (protocolID == MHPT_LOGIN)
		return true;
	else
		return false;
}
