/*
 * MHJSecurityClient.h
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#ifndef MHJSECURITYCLIENT_H_
#define MHJSECURITYCLIENT_H_

#include "MHJSecurityFactory.h"
#include "HostClientFactory.h"

//class HostClientFactory;

class MHJSecurityFactoryClient: public MHJSecurityFactory {
public:
	MHJSecurityFactoryClient(const char* MachineSecurity);
	virtual ~MHJSecurityFactoryClient();

	virtual char* getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID);

	virtual void setFactory(HostClientFactory *mProcessFactory);

	virtual void setSessionSecurityData(BYTE* SessionSecurityData);

protected:
	const char* mMachineSecurity;
	char mSessionSecurityData[SECURITYSIZE];
	HostClientFactory *mProcessFactory;
};

#endif /* MHJSECURITYCLIENT_H_ */
