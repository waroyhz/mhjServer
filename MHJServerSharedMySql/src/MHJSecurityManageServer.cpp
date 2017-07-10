/*
 * MHJSecurityManageServer.cpp
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#include "MHJSecurityManageServer.h"
#include "ProtocolTypeDefine.h"

//MAP_Database MHJSecurityManageServer::mSecurityFactoryDatabaseCache;
//sp_thread_mutex_t *MHJSecurityManageServer::mMutexMachineDatabaseCache = NULL;
//MAP_Database MHJSecurityManageServer::mSecurityFactorySessionCache;
//sp_thread_mutex_t *MHJSecurityManageServer::mMutexMachineSessionCache = NULL;

MHJSecurityManageServer::MHJSecurityManageServer() {

	mMutexMachineDatabaseCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineDatabaseCache, NULL);
	mMutexMachineSessionCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineSessionCache, NULL);

	::getGeneralSecurity (mGeneralSecurity);
}

MHJSecurityManageServer::~MHJSecurityManageServer() {

}

char* MHJSecurityManageServer::getSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > devicebase;
	devicebase = findToMachineCache(pdevice);
	if (devicebase.get() == NULL) {
//		//从数据库得到安全码。
//		uint16_t idSecurity;
//		*((BYTE*) &idSecurity) = pdevice->idSecurity1;
//		*(((BYTE*) &idSecurity) + 1) = pdevice->idSecurity2;
//
//		devicebase = mDataBaseFactory->selectDeviceSecurity(pdevice->deviceID,
//				idSecurity, pdevice->deviceType);
		devicebase = std::tr1::shared_ptr < MHJDeviceBase > (new MHJDeviceBase);
		devicebase->deviceID = pdevice->deviceID;
		::getGeneralSecurity(devicebase->security);

#ifdef DEBUG
		{
			char tmp[255]= {0};
			char tmpc[10];
			for(int i=0;i<32;i++)
			{
				sprintf(tmpc,"%02x",devicebase->security[i]);
				strcat(tmp,tmpc);
			}
			printf(" MHJSecurityManageServer::getSecurity id %d type:%d security：%s \n",devicebase->deviceID,devicebase->deviceType,tmp);
		}
#endif

		pushToMachineCache (devicebase);
	}
	return devicebase->security;

}

char* MHJSecurityManageServer::getSessionSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > devicebase;
	devicebase = findToSessionCache(pdevice);
	if (devicebase.get())
		return devicebase->security;
	else
		return NULL;
}

void MHJSecurityManageServer::pushToMachineCache(std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	mSecurityFactoryDatabaseCache.insert(MAP_Database::value_type(deviceDatabase->deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
}

std::tr1::shared_ptr<MHJDeviceBase> MHJSecurityManageServer::findToMachineCache(const MHJDeviceMark* pdevice) {

	std::tr1::shared_ptr < MHJDeviceBase > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	MAP_Database::const_iterator got = mSecurityFactoryDatabaseCache.find(pdevice->deviceID);
	if (got != mSecurityFactoryDatabaseCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);

	return deviceDatabase;
}

void MHJSecurityManageServer::pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	mSecurityFactorySessionCache.insert(MAP_Database::value_type(deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
}

std::tr1::shared_ptr<MHJDeviceBase> MHJSecurityManageServer::findToSessionCache(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	MAP_Database::const_iterator got = mSecurityFactorySessionCache.find(pdevice->deviceID);
	if (got != mSecurityFactorySessionCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
	return deviceDatabase;
}

void MHJSecurityManageServer::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	getOrCreateSessionSecurity(pdevice, outSecurity);
}

void MHJSecurityManageServer::getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	std::tr1::shared_ptr < MHJDeviceBase > session;
	session = findToSessionCache(pdevice);
	if (session.get() == NULL) {
		session = std::tr1::shared_ptr < MHJDeviceBase > (new MHJDeviceBase);
		pushToSessionCache(pdevice->deviceID, session);
	}

	session->deviceID = pdevice->deviceID;
	session->deviceType = pdevice->deviceType;

	*((BYTE*) &session->idsecurity) = pdevice->idSecurity1;
	*(((BYTE*) &session->idsecurity) + 1) = pdevice->idSecurity2;
	createSessionSecurityData(session->security);

	std::tr1::shared_ptr < MHJDeviceBase > deviceDatabase = findToMachineCache(pdevice);
	int i;
	for (i = 0; i < 32; i++) {
		outSecurity[i] = session->security[i];
		session->security[i] = session->security[i] ^ deviceDatabase->security[i];
	}
}
