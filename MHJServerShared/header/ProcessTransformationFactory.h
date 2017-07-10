/*
 * ProcessTransformationFactory.h
 *  消息转换工厂
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#ifndef PROCESSTRANSFORMATIONFACTORY_H_
#define PROCESSTRANSFORMATIONFACTORY_H_

#include <spresponse.hpp>
#include <SP_OnlineManager.h>
#include "MHJSecurityFactory.h"
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include "MHJProtocol.h"
#include <ProtocolTypeDefine.h>

class MHJProtocol;

class ProcessTransformationFactory {
public:
	ProcessTransformationFactory(SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType protocolType);
	virtual ~ProcessTransformationFactory();
//	void sendProtocol();

	void setOnlimeManager(SP_OnlineManager* onlineManager);
//	void setSecurity(MHJSecurityFactoryServer* security);

	template<class T>
	 bool sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, T *pdata) {
		return sendProtocol(destDevice, mprotocolID, mver, sizeof(T), pdata);
	}

	virtual bool sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);

protected:
	SP_Response *response;
	MHJDeviceMark* sendMark;
	SP_OnlineManager* mOnlineManager;
	boost::shared_ptr<MHJProtocol> protocol;
	UINT16 mpackID;
//	MHJSecurityFactoryServer* mSecurity;
	BYTE mprotocolType;
};

#endif /* PROCESSTRANSFORMATIONFACTORY_H_ */
