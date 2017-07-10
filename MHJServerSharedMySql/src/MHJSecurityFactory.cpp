/*
 * MHJSecurity.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: ruancongming
 */

#include "MHJSecurityFactory.h"
#include "ProcessFactory.h"
#include <string.h>

#include<iostream>
#include<ctime>
#include<cstdlib>
//#include "MHJDataBaseFactory.h"

#include <memory>
#include <tr1/memory>
#include <tr1/shared_ptr.h>

//#include <odb/mysql/database.hxx>
//#include "MHJDatabase.hxx"
//#include "MHJDatabase-odb.hxx"

MHJSecurityFactory::MHJSecurityFactory() {

}

MHJSecurityFactory::~MHJSecurityFactory() {

}

char* MHJSecurityFactory::getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID) {

	return NULL;
}

void MHJSecurityFactory::createSessionSecurity(const MHJDeviceMark* pdevice, char *security) {
//	return NULL;
}

void MHJSecurityFactory::setSessionSecurityData(BYTE* SessionSecurityData) {

}

inline void* MHJSecurityFactory::getDataBaseFactory() {
	return NULL;
}

inline void* MHJSecurityFactory::getDatabaseManage(MHJDeviceMark* mark) {
	return NULL;
}
//int MHJSecurity::getMachineSecurityData(uint32_t deviceID,uint16_t idSecurity,uint8_t deviceType,char *g_SecNum){
//
//	std::tr1::shared_ptr<MHJDeviceBase> ptr = mDataBaseFactory->selectDeviceSecurity(deviceID,idSecurity,deviceType);
//	memcpy(g_SecNum,ptr->security, 32);
//	return 0;
//}

