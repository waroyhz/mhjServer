/*
 * MHJSecurityManageDevice.cpp
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#include "MHJSecurityManageDevice.h"

//MAP_Database MHJSecurityManageDevice::mSecurityFactoryDatabaseCache;
//sp_thread_mutex_t *MHJSecurityManageDevice::mMutexMachineDatabaseCache = NULL;
//MAP_Database MHJSecurityManageDevice::mSecurityFactorySessionCache;
//sp_thread_mutex_t *MHJSecurityManageDevice::mMutexMachineSessionCache = NULL;

MHJSecurityManageDevice::MHJSecurityManageDevice() {

	mMutexMachineDatabaseCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineDatabaseCache, NULL);
	mMutexMachineSessionCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineSessionCache, NULL);

	::getGeneralSecurity (mGeneralSecurity);
}

MHJSecurityManageDevice::~MHJSecurityManageDevice() {

}

char* MHJSecurityManageDevice::getSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > devicebase;
	devicebase = findToMachineCache(pdevice);
	if (devicebase.get() == NULL) {
		//从数据库得到安全码。
		uint16_t idSecurity;
		*((BYTE*) &idSecurity) = pdevice->idSecurity1;
		*(((BYTE*) &idSecurity) + 1) = pdevice->idSecurity2;

		devicebase = mDataBaseFactory->selectDeviceSecurity(pdevice->deviceID, idSecurity, pdevice->deviceType);
		if (devicebase.get())
			pushToMachineCache (devicebase);
	}
	if (devicebase.get() == 0) {
		printf("未找到设备：%d 的安全码，返回无效安全码.\n", pdevice->deviceID);
		return mErrSecurity;
	}
	return devicebase->security;
}

char* MHJSecurityManageDevice::getSessionSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > devicebase;
	devicebase = findToSessionCache(pdevice);
	if (devicebase.get())
		return devicebase->security;
	else
		return NULL;
}

void MHJSecurityManageDevice::pushToMachineCache(std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	mSecurityFactoryDatabaseCache.insert(MAP_Database::value_type(deviceDatabase->deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
}

std::tr1::shared_ptr<MHJDeviceBase> MHJSecurityManageDevice::findToMachineCache(const MHJDeviceMark* pdevice) {

	std::tr1::shared_ptr < MHJDeviceBase > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	MAP_Database::const_iterator got = mSecurityFactoryDatabaseCache.find(pdevice->deviceID);
	if (got != mSecurityFactoryDatabaseCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);

	return deviceDatabase;
}

void MHJSecurityManageDevice::pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	mSecurityFactorySessionCache.insert(MAP_Database::value_type(deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
}

std::tr1::shared_ptr<MHJDeviceBase> MHJSecurityManageDevice::findToSessionCache(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	MAP_Database::const_iterator got = mSecurityFactorySessionCache.find(pdevice->deviceID);
	if (got != mSecurityFactorySessionCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
	return deviceDatabase;
}

void MHJSecurityManageDevice::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	getOrCreateSessionSecurity(pdevice, outSecurity);
}
std::tr1::shared_ptr<MHJDeviceBase> MHJSecurityManageDevice::getDatabaseModel(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJDeviceBase > devicebase = findToMachineCache(pdevice);
	return devicebase;
}

void MHJSecurityManageDevice::getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
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
	if (deviceDatabase.get())
	{
	int i;
	for (i = 0; i < 32; i++) {
		outSecurity[i] = session->security[i];
		session->security[i] = session->security[i] ^ deviceDatabase->security[i];
	}
	}
	else
	{
		printf("APP getOrCreateSessionSecurity NULL, id=%d type=%d\n",pdevice->deviceID,pdevice->deviceType);
	}
//	return session;
}

//void MHJSecurityManageDevice::updateDatabase(std::tr1::shared_ptr<MHJDeviceBase> mhjdevicebase) {
////TODO 需要加入更新数据库队列，当服务器更新数据库的时候写入数据库。
//	mDataBaseFactory->updateDeviceBase(mhjdevicebase);
//}

