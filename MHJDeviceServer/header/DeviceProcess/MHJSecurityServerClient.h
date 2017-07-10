/*
 * MHJSecurityServerClient.h
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#ifndef MHJSECURITYSERVERCLIENT_H_
#define MHJSECURITYSERVERCLIENT_H_

#include <MHJSecurityFactoryClient.h>

class MHJSecurityServerClient: public MHJSecurityFactoryClient {
public:
	MHJSecurityServerClient(char *ServerSecurityData);
	virtual ~MHJSecurityServerClient();
//	virtual char* getSecurity(const MHJDeviceMark* pdevice,BYTE protocolID);
};

#endif /* MHJSECURITYSERVERCLIENT_H_ */
