/*
 * MHJHostClient.h
 *
 *  Created on: 2015-7-8
 *      Author: waroy
 */

#ifndef MHJHOSTCLIENT_H_
#define MHJHOSTCLIENT_H_

#include <pthread.h>
#include "CClientASIO.h"
#include "HostClientFactory.h"
#include <spmsgdecoder.hpp>
#include "ProtocolTypeDefine.h"
#include "MHJSecurityFactory.h"
#include "MHJProtocol.h"
class HostClientFactory;

class MHJHostClient {
public:
	MHJHostClient(SP_MsgDecoder * msgDecoder, HostClientFactory* cilentFactory, MHJSecurityFactory* security);
	virtual ~MHJHostClient();
	void initTcpClient();
	void heartbeat();
	void revice();
	int decode(SP_Buffer*inBuffer);
	bool sendProtocol(BYTE mdeviceType, const MHJDeviceMark* pdevice, UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);
	void Close();
protected:
	pthread_t mtackid;
	pthread_t mHeartbeatid;
	CClientASIO* tcp;
	HostClientFactory* mClientFactory;
	SP_MsgDecoder * mMsgDecoder;
	sp_thread_mutex_t *mutex_heartBeat;
	bool heartbeatrun;
	MHJSecurityFactory* mSecurity;
	MHJProtocol* mSendProtocol;

};

#endif /* MHJHOSTCLIENT_H_ */
