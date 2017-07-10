/*
 * HostAddress.h
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef HOSTADDRESS_H_
#define HOSTADDRESS_H_

#include <sys/types.h>
class HostAddress {
public:
	HostAddress();
	virtual ~HostAddress();
	static int getAllAddress(char* outAddress,uint insize);
	static int getUsefulIPAddress(char *outAddress, uint insize);
};

#endif /* HOSTADDRESS_H_ */
