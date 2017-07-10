/*
 * MHJSecurityClient.cpp
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#include "MHJSecurityFactoryClient.h"
#include <string>

MHJSecurityFactoryClient::MHJSecurityFactoryClient(const char* MachineSecurity) {
	mMachineSecurity = MachineSecurity;
}

MHJSecurityFactoryClient::~MHJSecurityFactoryClient() {

}

char* MHJSecurityFactoryClient::getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID) {
	if (mProcessFactory->isLogin(protocolID))
		return (char*) mMachineSecurity;
	else
		return mSessionSecurityData;
}

void MHJSecurityFactoryClient::setFactory(HostClientFactory *mProcessFactory) {
	this->mProcessFactory = mProcessFactory;
}

void MHJSecurityFactoryClient::setSessionSecurityData(BYTE* SessionSecurityData) {
	int i;
	for (i = 0; i < 32; i++) {
		mSessionSecurityData[i] = SessionSecurityData[i] ^ mMachineSecurity[i];
	}
//	memcpy(mSessionSecurityData,SessionSecurityData,sizeof(mSessionSecurityData));
}
