/*
 * HostClientFactory.h
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef HOSTCLIENTFACTORY_H_
#define HOSTCLIENTFACTORY_H_
//#include "MHJHostClient.h"
#include "CClientASIO.h"
#include <spmsgdecoder.hpp>
#include "MHJDefine.h"
#include "MHJHostClient.h"
#include "ProtocolTypeDefine.h"
#include "MHJSecurityFactory.h"
#include <vector>

class MHJHostClient;
class MHJSecurityFactory;

class HostClientFactory {
public:
	HostClientFactory(MHJDeviceMark *device, MHJProtocolType protocolType, const char*defautlIP, UINT16 defaultPort);
	virtual ~HostClientFactory();

	virtual void sendLogin(MHJHostClient *client);
	virtual void disConnect(MHJHostClient *client);
	virtual void sendheartbeat(MHJHostClient *client);
	virtual void revice(SP_MsgDecoder* msgDecoder, MHJHostClient* client);
	std::string getConnectHost();
	int getConnectProt();

	virtual void sendProtocol(BYTE mProtocolID, BYTE mver, BYTE mlength, BYTE *mdata);

	void Close();

	void addConnectIndex();

	void setSecurity(MHJSecurityFactory* security);

	bool isLogin(BYTE protocolID);

protected:
	std::string ip;
	int prot;
	MHJHostClient *mclient;
	MHJDeviceMark *mdevice;
	UINT16 mpackID;
//	char mSecurityData[32];

	uint mIpConnectIndex;
	std::vector<IPPORTADDRESS> mIpVector;

	MHJHostClient* client;
	WORD protocolType;

	MHJSecurityFactory* mSecurity;

	bool mIsLogin;
};

#endif /* HOSTCLIENTFACTORY_H_ */
