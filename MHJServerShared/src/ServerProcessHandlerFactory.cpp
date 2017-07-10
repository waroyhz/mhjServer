/*
 * DeviceServer.cpp
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#include "ServerProcessHandlerFactory.h"
#include "ProcessFactory.h"
#include "MHJSecurityFactory.h"

MHJServerHandlerFactory::MHJServerHandlerFactory(ProcessFactory *processFactory, MHJSecurityFactory *pSecurity) {
	mOnlineManager = new SP_OnlineManager;
	ProcessFactory::setOnlimeManager (mOnlineManager);
	mprocessFactory = processFactory;
	mSecurity = pSecurity;
}

MHJServerHandlerFactory::~MHJServerHandlerFactory() {
	delete mOnlineManager;
//	mSecurity = NULL;
}

inline SP_Handler *MHJServerHandlerFactory::create() const {
	return (SP_Handler *) new MHJServerHandler(mOnlineManager, mprocessFactory, mSecurity);
}

//---------------------------------------------------------
