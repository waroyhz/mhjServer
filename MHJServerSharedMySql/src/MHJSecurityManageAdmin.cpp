/*
 * MHJSecurityManageAdmin.cpp
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#include "MHJSecurityManageAdmin.h"

#include "MoDbKey.h"
#include "MoAdminEntity.h"
//#include "JsonAdminEntity.h"
#include <iostream>
#include "JsonStrObjMap.h"
using namespace std;
//MAP_Database MHJSecurityManageAdmin::mSecurityFactoryDatabaseCache;
//sp_thread_mutex_t *MHJSecurityManageAdmin::mMutexMachineDatabaseCache = NULL;
//MAP_Database MHJSecurityManageAdmin::mSecurityFactorySessionCache;
//sp_thread_mutex_t *MHJSecurityManageAdmin::mMutexMachineSessionCache = NULL;

MHJSecurityManageAdmin::MHJSecurityManageAdmin() {

	mMutexMachineDatabaseCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineDatabaseCache, NULL);
	mMutexMachineSessionCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexMachineSessionCache, NULL);

	::getGeneralSecurity (mGeneralSecurity);
//TODO 什么写法？数据库可以一次性读取 mDataBaseFactory并不在初始化的时候设置，初始化需要重载setDatabaseFactory函数
//	std::tr1::shared_ptr < MHJAdminEntity > devicebase;
//	uint32_t deviceID = ADMIN_DATABASE_DEVICEID;
//	int i;
//	for (i = 0; i < ADMIN_DATABASE_SIZE; i++) {
//		devicebase = mDataBaseFactory->selectAdminSecurity(deviceID + i);
//		pushToMachineCache (devicebase);
//	}
}

MHJSecurityManageAdmin::~MHJSecurityManageAdmin() {

}

char* MHJSecurityManageAdmin::getSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJAdminEntity > devicebase;
	devicebase = findToMachineCache(pdevice);
	if (devicebase.get() == NULL) {
//		//从数据库得到安全码。
		uint16_t idSecurity;
		*((BYTE*) &idSecurity) = pdevice->idSecurity1;
		*(((BYTE*) &idSecurity) + 1) = pdevice->idSecurity2;
		//admin查询数据库的方式？？
		devicebase = mDataBaseFactory->selectAdminEntity(pdevice->deviceID); //,idSecurity, pdevice->deviceType);
//		devicebase=std::tr1::shared_ptr<MHJDeviceBase>(new MHJDeviceBase);
//		devicebase->deviceID=pdevice->deviceID;
//		::getGeneralSecurity(devicebase->security);
		if(devicebase.get()){
#ifdef ADMIN_DEBUG   //DBI的设备安全码与当前不同。 为了Admin测试端能顺利使用， 手动指定设备安全码
			for(int i=0;i<32;i++){
				devicebase->adminToken[i] = i+1;
			}
#endif
			pushToMachineCache (devicebase);
		}
	}
	if(devicebase.get())
		return devicebase->adminToken;
	else
		return NULL;
}

char* MHJSecurityManageAdmin::getSessionSecurity(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJAdminEntity > devicebase;
	devicebase = findToSessionCache(pdevice);
	return devicebase->adminToken;
}

void MHJSecurityManageAdmin::pushToMachineCache(std::tr1::shared_ptr<MHJAdminEntity> deviceDatabase) {
	//写入redis只用于测试功能，redis缓存写入由java负责
//#ifdef DATABASE_REDIS
//	std::string key = MoDbKey::getModelKey(MoAdminEntity::id(),deviceDatabase->id);
//	std::string value;
//	json2StrMHJAdminEntity(deviceDatabase, &value);
//	mDataBaseFactory->redisfactory->RedisCommand_SET((char*)key.c_str(),(char*)value.c_str());
//#endif
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	mSecurityFactoryDatabaseCache.insert(MAP_Database::value_type(deviceDatabase->id, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
}

std::tr1::shared_ptr<MHJAdminEntity> MHJSecurityManageAdmin::findToMachineCache(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJAdminEntity > deviceDatabase;
//#ifdef DATABASE_REDIS
	std::string key = MoDbKey::getModelKey(MoAdminEntity::id(),pdevice->deviceID);
	MHJRedisBaseFactory * redisbasefactory = mDataBaseFactory->GetRedisBaseFactory();
	redisReply* redisreply =(redisReply*)redisbasefactory->RedisCommand_GET((char *)key.c_str());
//	redisReply* redisreply =(redisReply*)mDataBaseFactory->redisfactory->RedisCommand_GET((char *)key.c_str());
	if(redisreply->str !=NULL) {
		std::string value(redisreply->str);
		deviceDatabase =json2MHJAdminEntity(&value);

		// Inorder to keep the area which the shared_ptr pointer 'deviceDatabase' pointed to available, we push the pointer to cacahe
		sp_thread_mutex_lock (mMutexMachineDatabaseCache);
		MAP_Database::const_iterator got = mSecurityFactoryDatabaseCache.find(pdevice->deviceID);
		if (got != mSecurityFactoryDatabaseCache.end()) {
			mSecurityFactoryDatabaseCache.erase(got->second->id);
		}
		mSecurityFactoryDatabaseCache.insert(MAP_Database::value_type(deviceDatabase->id, deviceDatabase));
		sp_thread_mutex_unlock(mMutexMachineDatabaseCache);
		redisbasefactory->RedisCommand_FREE(redisreply);

		return deviceDatabase;
	}

//#endif
	sp_thread_mutex_lock (mMutexMachineDatabaseCache);
	MAP_Database::const_iterator got = mSecurityFactoryDatabaseCache.find(pdevice->deviceID);
	if (got != mSecurityFactoryDatabaseCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineDatabaseCache);

	return deviceDatabase;
}

void MHJSecurityManageAdmin::pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJAdminEntity> deviceDatabase) {
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	mSecurityFactorySessionCache.insert(MAP_Database::value_type(deviceID, deviceDatabase));
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
}

std::tr1::shared_ptr<MHJAdminEntity> MHJSecurityManageAdmin::findToSessionCache(const MHJDeviceMark* pdevice) {
	std::tr1::shared_ptr < MHJAdminEntity > deviceDatabase;
	sp_thread_mutex_lock (mMutexMachineSessionCache);
	MAP_Database::const_iterator got = mSecurityFactorySessionCache.find(pdevice->deviceID);
	if (got != mSecurityFactorySessionCache.end()) {
		deviceDatabase = got->second;
	}
	sp_thread_mutex_unlock(mMutexMachineSessionCache);
	return deviceDatabase;
}

void MHJSecurityManageAdmin::createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	getOrCreateSessionSecurity(pdevice, outSecurity);
}

void MHJSecurityManageAdmin::getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity) {
	std::tr1::shared_ptr < MHJAdminEntity > session;
	session = findToSessionCache(pdevice);
	if (session.get() == NULL) {
		session = std::tr1::shared_ptr < MHJAdminEntity > (new MHJAdminEntity);
		pushToSessionCache(pdevice->deviceID, session);
	}

	session->id = pdevice->deviceID;
//	session->deviceType = pdevice->deviceType;

//	*((BYTE*) &session->tokenReplaceTime) = pdevice->idSecurity1;
//	*(((BYTE*) &session->tokenReplaceTime) + 1) = pdevice->idSecurity2;
	createSessionSecurityData(session->adminToken);

	std::tr1::shared_ptr < MHJAdminEntity > deviceDatabase = findToMachineCache(pdevice);
	int i;
	for (i = 0; i < 32; i++) {
		outSecurity[i] = session->adminToken[i];
		session->adminToken[i] = session->adminToken[i] ^ deviceDatabase->adminToken[i];
	}
}
