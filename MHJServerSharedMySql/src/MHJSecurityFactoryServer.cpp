/*
 * MHJSecurityServer.cpp
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#include "MHJSecurityFactoryServer.h"
#include "MHJFunctionTimeRecord.h"

extern struct FunctionTimeRecord *pFunTimeRecoder;
MHJSecurityFactoryServer::MHJSecurityFactoryServer() {

}

MHJSecurityFactoryServer::~MHJSecurityFactoryServer() {
	//	sp_thread_mutex_destroy(mMutexDatabaseCache);
	//	sp_thread_mutex_destroy(mMutexSessionCache);
}

char* MHJSecurityFactoryServer::getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID) {
	MHJSecurityManageFactory * mSecurityManage = getSecurityManage(pdevice);
	if (mProcessFactory->isLogin(pdevice, protocolID)) {
		return mSecurityManage->getSecurity(pdevice);
	} else {
		if (mSecurityManage) {
			char* mSessionSecurity = mSecurityManage->getSessionSecurity(pdevice);
			return mSessionSecurity;
		} else
			return NULL;
	}
}
void MHJSecurityFactoryServer::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	MHJSecurityManageFactory * mSecurityManage = getSecurityManage(pdevice);
	mSecurityManage->createSessionSecurity(pdevice, outSecurity);
}

void MHJSecurityFactoryServer::setFactory(ProcessFactory *mProcessFactory, MHJDataBaseFactory* sDataBaseFactory) {
	this->mProcessFactory = mProcessFactory;
	this->mDataBaseFactory = sDataBaseFactory;
	mSecurityManageFactory.setDatabaseFactory(sDataBaseFactory);
}

MHJSecurityManageFactory* MHJSecurityFactoryServer::getSecurityManage(const MHJDeviceMark* pdevice) {
//	MHJSecurityManageFactory*mSecurityManage = &mSecurityManageFactory;
	//默认设置为设备登录，因为设备类型会增加。
	MHJSecurityManageFactory*mSecurityManage = &mSecurityDevice;
//	if (pdevice->deviceType == MDT_SWITCH || pdevice->deviceType == MDT_CURTAIN || pdevice->deviceType == MDT_Transponder || pdevice->deviceType == MDT_SENSOR)
//		mSecurityManage = &mSecurityDevice;
//	else
	if (pdevice->deviceType == MDT_SERVER)
		mSecurityManage = &mSecurityServer;
	else if (pdevice->deviceType == MDT_APP)
		mSecurityManage = &mSecurityAPP;
	else if (pdevice->deviceType == MDT_HOST)
		mSecurityManage = &mSecurityHost;
	else if (pdevice->deviceType == MDT_ADMIN)
		mSecurityManage = &mSecurityAdmin;

	return mSecurityManage;
}

void* MHJSecurityFactoryServer::getDataBaseFactory() {
	return mDataBaseFactory;
}

void* MHJSecurityFactoryServer::getDatabaseManage(MHJDeviceMark* mark) {
	MHJSecurityManageFactory* securityManage = getSecurityManage(mark);
	return securityManage;
//	return securityManage->getDatabaseModel(mark);
}
