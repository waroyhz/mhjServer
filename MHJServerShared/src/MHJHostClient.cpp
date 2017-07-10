/*
 * MHJHostClient.cpp
 *
 *  Created on: 2015-7-8
 *      Author: waroy
 */

#include "MHJHostClient.h"
#include <spthread.hpp>
#include <event.h>
#include <iostream>
//#include "Protocol.h"
#include <time.h>
#include "MHJProtocol.h"

using namespace boost;

void* ThreadHostClient(void *arg) {
	MHJHostClient*mhjHouseClient = (MHJHostClient*) arg;

	mhjHouseClient->initTcpClient();

	return 0;
}

void* ThreadHeartBeat(void *arg) {
	MHJHostClient*mhjHouseClient = (MHJHostClient*) arg;
	mhjHouseClient->heartbeat();

	return 0;
}

void reviceDataFunc(SP_Buffer *inBuffer, void *arg) {
	MHJHostClient*mhjHouseClient = (MHJHostClient*) arg;
	if (mhjHouseClient->decode(inBuffer) == 0/*eOk*/) {
		mhjHouseClient->revice();
	}
}

MHJHostClient::MHJHostClient(SP_MsgDecoder * msgDecoder, HostClientFactory* cilentFactory, MHJSecurityFactory *security) :
		heartbeatrun(false) {
	mutex_heartBeat = new pthread_mutex_t;
	sp_thread_mutex_init(mutex_heartBeat, NULL);
	mMsgDecoder = msgDecoder;
	mClientFactory = cilentFactory;
	mSecurity = security;
	mSendProtocol = new MHJProtocol(mSecurity);

	//线程启动必须放在最后
	pthread_create(&mtackid, NULL, ThreadHostClient, (void *) this);
	pthread_create(&mHeartbeatid, NULL, ThreadHeartBeat, (void *) this);
}

MHJHostClient::~MHJHostClient() {
	sp_thread_mutex_destroy (mutex_heartBeat);
	delete mutex_heartBeat;
	delete mClientFactory;
}

void MHJHostClient::initTcpClient() {
	while (true) {
		io_service io_service_;
		mClientFactory->addConnectIndex();
		tcp = new CClientASIO(mClientFactory->getConnectHost(), mClientFactory->getConnectProt(), &io_service_, reviceDataFunc, (void*) this);
		io_service_.poll();
		if (tcp->isConnected())
			mClientFactory->sendLogin(this);

		sp_thread_mutex_lock (mutex_heartBeat);
		heartbeatrun = true;
		sp_thread_mutex_unlock(mutex_heartBeat);
		while (tcp->isConnected()) {

//			heartbeat();
			io_service_.poll();
			usleep(1);
		}
		sp_thread_mutex_lock(mutex_heartBeat);
		heartbeatrun = false;
		sp_thread_mutex_unlock(mutex_heartBeat);

		mClientFactory->disConnect(this);
		delete tcp;
		sleep(5);
	}

}

void MHJHostClient::heartbeat() {
	static time_t lastInfoTime;
	time(&lastInfoTime);

	while (1) {
		sp_thread_mutex_lock (mutex_heartBeat);

		if (time(NULL) - lastInfoTime > 5 && heartbeatrun) {
			time(&lastInfoTime);
			if (tcp->isConnected()) {
				mClientFactory->sendheartbeat(this);
			}
		}

		sp_thread_mutex_unlock(mutex_heartBeat);
		sleep(5);
	}

}

void MHJHostClient::revice() {
	mClientFactory->revice(mMsgDecoder, this);
}

int MHJHostClient::decode(SP_Buffer*inBuffer) {
	return mMsgDecoder->decode(inBuffer);
}

bool MHJHostClient::sendProtocol(BYTE mprotocolType, const MHJDeviceMark* pdevice, UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata) {
//	MHJProtocol::getSendBuffer();
	shared_ptr < SP_Buffer > outBuffer = mSendProtocol->getSendBuffer(mprotocolType, pdevice, mpackID, mprotocolID, mver, mlength, pdata);
	if (tcp->isConnected()) {
		tcp->SendData(outBuffer->getBuffer(), outBuffer->getSize());
		std::cout << "发送数据 长度" << outBuffer->getSize() << std::endl;
		return true;
	}
	return false;
}

void MHJHostClient::Close() {
	tcp->Close();
}
