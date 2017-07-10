/*
 * MHJSecurityManageAPP.cpp
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#include "MHJSecurityManageAPP.h"

//MAP_Database MHJSecurityManageAPP::mSecurityFactoryDatabaseCache;
//sp_thread_mutex_t *MHJSecurityManageAPP::mMutexMachineDatabaseCache = NULL;
//MAP_Database MHJSecurityManageAPP::mSecurityFactorySessionCache;
//sp_thread_mutex_t *MHJSecurityManageAPP::mMutexMachineSessionCache = NULL;

MHJSecurityManageAPP::MHJSecurityManageAPP() {

	mMutexMachineDatabaseCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineDatabaseCache, NULL);
	mMutexMachineSessionCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineSessionCache, NULL);

	::getGeneralSecurity (mGeneralSecurity);
}

MHJSecurityManageAPP::~MHJSecurityManageAPP() {

}

char* MHJSecurityManageAPP::getSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJUserEntity > devicebase;
	devicebase = findToMachineCache(pdevice);
	bool findBydb = false;
	if (devicebase.get() == NULL)
		findBydb = true;
	else {
		uint16_t idsecurity = pdevice->idSecurity1 + (pdevice->idSecurity2 << 8);
		uint16_t cachescurity = devicebase->tokenReplaceTime & 0xffff;
		if (cachescurity != idsecurity)
			findBydb = true;
	}

	if (findBydb) {
		if (devicebase.get()) {
			removeToMachineCache (devicebase);
		}
		//		//从数据库得到安全码。
		uint16_t idSecurity;
		*((BYTE*) &idSecurity) = pdevice->idSecurity1;
		*(((BYTE*) &idSecurity) + 1) = pdevice->idSecurity2;

		devicebase = mDataBaseFactory->selectUserEntity(pdevice->deviceID);
		if (devicebase.get()) {
			uint16_t idsecurity = pdevice->idSecurity1 + (pdevice->idSecurity2 << 8);

			uint16_t cachescurity = devicebase->tokenReplaceTime & 0xffff;
			if (cachescurity != idsecurity) {
				printf("app idSecurity Error , DB ID Security is %04x , post ID Security is %04x\n", cachescurity, idsecurity);
				return NULL;
			}
			pushToMachineCache (devicebase);
		} else
			return NULL;
	}
	return devicebase->userToken;

}

char* MHJSecurityManageAPP::getSessionSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJUserEntity > devicebase;
	devicebase = findToSessionCache(pdevice);
	if (devicebase.get())
		return devicebase->userToken;
	else
		return NULL;
}

void MHJSecurityManageAPP::pushToMachineCache(std::tr1::shared_ptr<MHJUserEntity> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	mSecurityFactoryDatabaseCache.insert(MAP_Database::value_type(deviceDatabase->id, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
}

void MHJSecurityManageAPP::removeToMachineCache(std::tr1::shared_ptr<MHJUserEntity> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	mSecurityFactoryDatabaseCache.erase(deviceDatabase->id);
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
}

std::tr1::shared_ptr<MHJUserEntity> MHJSecurityManageAPP::findToMachineCache(const MHJDeviceMark* pdevice) {

	std::tr1::shared_ptr < MHJUserEntity > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	MAP_Database::const_iterator got = mSecurityFactoryDatabaseCache.find(pdevice->deviceID);
	if (got != mSecurityFactoryDatabaseCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);

	return deviceDatabase;
}

void MHJSecurityManageAPP::pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJUserEntity> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	mSecurityFactorySessionCache.insert(MAP_Database::value_type(deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
}

std::tr1::shared_ptr<MHJUserEntity> MHJSecurityManageAPP::findToSessionCache(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJUserEntity > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	MAP_Database::const_iterator got = mSecurityFactorySessionCache.find(pdevice->deviceID);
	if (got != mSecurityFactorySessionCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
	return deviceDatabase;
}

void MHJSecurityManageAPP::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	getOrCreateSessionSecurity(pdevice, outSecurity);
}

void MHJSecurityManageAPP::getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	std::tr1::shared_ptr < MHJUserEntity > session;
	session = findToSessionCache(pdevice);
	if (session.get() == NULL) {
		session = std::tr1::shared_ptr < MHJUserEntity > (new MHJUserEntity);
		pushToSessionCache(pdevice->deviceID, session);
	}

	session->id = pdevice->deviceID;
//	session->deviceType = pdevice->deviceType;

//	*((BYTE*) &session->idsecurity) = pdevice->idSecurity1;
//	*(((BYTE*) &session->idsecurity) + 1) = pdevice->idSecurity2;
	createSessionSecurityData(session->userToken);

	std::tr1::shared_ptr < MHJUserEntity > deviceDatabase = findToMachineCache(pdevice);
	if (deviceDatabase.get())
	{
		int i;
	for (i = 0; i < 32; i++) {
		outSecurity[i] = session->userToken[i];
		session->userToken[i] = session->userToken[i] ^ deviceDatabase->userToken[i];
	}
	}
	else
	{
		printf("APP getOrCreateSessionSecurity NULL, id=%d type=%d\n",pdevice->deviceID,pdevice->deviceType);
	}
}

std::tr1::shared_ptr<MHJUserEntity> MHJSecurityManageAPP::getDatabaseModel(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJUserEntity > devicebase = findToMachineCache(pdevice);
	return devicebase;
}
