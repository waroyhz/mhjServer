/*
 * MHJSecurityDeviceServer.h
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#ifndef MHJSECURITYDEVICESERVER_H_
#define MHJSECURITYDEVICESERVER_H_

#include <MHJSecurityFactoryServer.h>

class MHJSecurityDeviceServer: public MHJSecurityFactoryServer {
public:
	MHJSecurityDeviceServer();
	virtual ~MHJSecurityDeviceServer();
};

#endif /* MHJSECURITYDEVICESERVER_H_ */
