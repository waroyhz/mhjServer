/*
 * MHJDeviceManage.cpp
 *
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#include "MHJDeviceManage.h"

using namespace std::tr1;

MHJDeviceManage::MHJDeviceManage() {
	mMutexChildDatabaseCache = new sp_thread_mutex_t;
	sp_thread_mutex_init(mMutexChildDatabaseCache, NULL);

}

MHJDeviceManage::~MHJDeviceManage() {
	sp_thread_mutex_destroy (mMutexChildDatabaseCache);

}

shared_ptr<MHJDeviceChild> MHJDeviceManage::getDeviceChild(uint64_t dbKeyID) {

	shared_ptr < MHJDeviceChild > devicechild = findByCache(dbKeyID);
	if (devicechild.get() == NULL) {
		devicechild = findByDb(dbKeyID);
	}
	return devicechild;
}

void MHJDeviceManage::pushToCache(std::tr1::shared_ptr<MHJDeviceChild> deviceChild) {

	sp_thread_mutex_lock (mMutexChildDatabaseCache);
	mChildDatabaseCacheKyeID.insert(MAP_MHJDeviceChild::value_type(deviceChild->id_, deviceChild));
	sp_thread_mutex_unlock(mMutexChildDatabaseCache);
}
std::tr1::shared_ptr<MHJDeviceChild> MHJDeviceManage::findByCache(uint64_t dbkey) {
	std::tr1::shared_ptr < MHJDeviceChild > child;
	sp_thread_mutex_lock (mMutexChildDatabaseCache);
	MAP_MHJDeviceChild::const_iterator got = mChildDatabaseCacheKyeID.find(dbkey);
	if (got != mChildDatabaseCacheKyeID.end()) {
		child = got->second;
	}
	sp_thread_mutex_unlock(mMutexChildDatabaseCache);
	return child;
}

void MHJDeviceManage::setDataBaseFactory(MHJDataBaseFactory* dataBaseFactory) {
	mDataBaseFactory = dataBaseFactory;
}

MHJDataBaseFactory * MHJDeviceManage::getDataBaseFactory(){
	return mDataBaseFactory;
}
std::tr1::shared_ptr<MHJDeviceChild> MHJDeviceManage::findByDb(uint64_t dbkey) {
	shared_ptr < MHJDeviceChild > child = mDataBaseFactory->selectDeviceChild(dbkey);
	if (child.get())
		pushToCache (child);
	return child;
}

//void MHJDeviceManage::updateDeviceChild(std::tr1::shared_ptr<MHJDeviceChild> devicechild) {
//	mDataBaseFactory->updateDeviceChild(devicechild);
//}
