/*
 * MHJSecurityServerClient.cpp
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#include "MHJSecurityServerClient.h"

MHJSecurityServerClient::MHJSecurityServerClient(char*ServerSecurityData) :
		MHJSecurityFactoryClient(ServerSecurityData) {

}

MHJSecurityServerClient::~MHJSecurityServerClient() {

}

//char* MHJSecurityServerClient::getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID) {
//	if (mProcessFactory->isLogin(protocolID))
//		return mMachineSecurity;
//	else
//		return mSessionSecurityData;
//}
