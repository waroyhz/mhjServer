/*
 * MHJSecurityManageFactory.cpp
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#include "MHJSecurityManageFactory.h"

MHJDataBaseFactory* MHJSecurityManageFactory::mDataBaseFactory = NULL;

MHJSecurityManageFactory::MHJSecurityManageFactory() {
	BYTE tErrSecurity[SECURITYSIZE] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
			12, 13, 14, 15 };
	memcpy(mErrSecurity, tErrSecurity, SECURITYSIZE);
}

MHJSecurityManageFactory::~MHJSecurityManageFactory() {

}

char* MHJSecurityManageFactory::getSecurity(const MHJDeviceMark* pdevice) {
	return mGeneralSecurity;
}

char* MHJSecurityManageFactory::getSessionSecurity(const MHJDeviceMark* pdevice) {
	return NULL;
}

void MHJSecurityManageFactory::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {

}

void MHJSecurityManageFactory::createSessionSecurityData(char* outSecurityData) {
	srand (time(NULL));int i;
	for (i = 0; i < 32; i++) {
		outSecurityData[i] = rand() / 256;
	}
}

void* MHJSecurityManageFactory::getDatabaseModel(MHJDeviceMark* mark) {
	return NULL;
}

void MHJSecurityManageFactory::setDatabaseFactory(MHJDataBaseFactory* dataBaseFactory) {
	mDataBaseFactory = dataBaseFactory;
}
