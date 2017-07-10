/*
 * ProcessTransformationFactory.cpp
 *
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#include "ProcessTransformationFactory.h"
#include <spbuffer.hpp>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include <stdio.h>

//using namespace std::tr1;

ProcessTransformationFactory::ProcessTransformationFactory(SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType protocolType) {
	this->response = response;
	this->protocol = protocol;
	this->sendMark = protocol->device();

	mprotocolType = protocolType;
	mpackID = 0;
}

ProcessTransformationFactory::~ProcessTransformationFactory() {

}

/*
 * 发送协议
 */bool ProcessTransformationFactory::sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata) {
	boost::shared_ptr < SP_Buffer > outBuffer = protocol->getSendBuffer(mprotocolType, destDevice, mpackID, mprotocolID, mver, mlength, pdata);

	if (outBuffer.get()) {
//		SP_Sid_t *sid = mOnlineManager->getSid(destDevice);
		SP_ArrayList sidList;
		mOnlineManager->getSidList(destDevice, &sidList);
		if(sidList.getCount()>0){
			for (int i = 0; i < sidList.getCount(); i++) {
				SP_Sid_t *sid = (SP_Sid_t *) sidList.getItem(i);
				SP_Message * msg = new SP_Message();
				msg->getMsg()->append(outBuffer->getBuffer(), outBuffer->getSize());
				msg->getToList()->add(*sid);
				response->addMessage(msg);
			}

			return true;
		}
	}
		return false;
}

void ProcessTransformationFactory::setOnlimeManager(SP_OnlineManager *onlineManager) {
	mOnlineManager = onlineManager;
}

// void ProcessTransformationFactory::setSecurity(MHJSecurityFactoryServer* security){
//	mSecurity=security;
//}
