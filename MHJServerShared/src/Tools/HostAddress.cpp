/*
 * HostAddress.cpp
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#include "Tools/HostAddress.h"
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

HostAddress::HostAddress() {


}

HostAddress::~HostAddress() {

}

int HostAddress::getAllAddress(char *outAddress, uint insize)
{
	uint retsize=0;
	 struct ifaddrs * ifAddrStruct=NULL;
	    void * tmpAddrPtr=NULL;

	    getifaddrs(&ifAddrStruct);

	    while (ifAddrStruct!=NULL) {
	        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
	            // is a valid IP4 Address
	            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
	            char addressBuffer[INET_ADDRSTRLEN];
	            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

	            if(strcmp(addressBuffer,"127.0.0.1")!=0){
	            *((in_addr_t*)(outAddress+retsize))= inet_addr( addressBuffer );
	            	retsize+=sizeof(in_addr_t);
	            	if (retsize+sizeof(in_addr_t)>insize)
	            		break;
	            }
//	            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
	        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
	            // is a valid IP6 Address
//	            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//	            char addressBuffer[INET6_ADDRSTRLEN];
//	            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//	            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
	        }
	        ifAddrStruct=ifAddrStruct->ifa_next;
	    }
	    return retsize;
}

int HostAddress:: getUsefulIPAddress(char *outAddress, uint insize)
{
	uint retsize=0;
	 struct ifaddrs * ifAddrStruct=NULL;
	    void * tmpAddrPtr=NULL;

	    getifaddrs(&ifAddrStruct);

	    while (ifAddrStruct!=NULL) {
	        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
	            // is a valid IP4 Address
	            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
	            char addressBuffer[INET_ADDRSTRLEN];

	            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
//	            printf("%s\t",addressBuffer);
	            if(!strncmp(addressBuffer,"192",3)){
					*((in_addr_t*)(outAddress+retsize))= inet_addr( addressBuffer );
						retsize+=sizeof(in_addr_t);
						if (retsize+sizeof(in_addr_t)>insize)
							break;
//					printf("%s IP4 Address: %s\n", ifAddrStruct->ifa_name, addressBuffer);

	            }
	        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
	            // is a valid IP6 Address
//	            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//	            char addressBuffer[INET6_ADDRSTRLEN];
//	            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//	            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
	        }
	        ifAddrStruct=ifAddrStruct->ifa_next;
	    }
//	    printf("%d\n",retsize);
	    return retsize;
}
