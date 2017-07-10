/*
 * MHJDataBaseFactoryServer.cpp
 *
 *  Created on: Jul 31, 2015
 *      Author: ruancongming
 */

#include "MHJDataBaseServer.h"

#include <memory>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include <ctime>
#include "MHJSqlTracer.h"
#include "ProtocolDeviceSwitch.h"
#include "MHJDBInterface.h"
#include "JsonStrObjMap.h"

using namespace boost;
//using namespace std;
//using namespace std::tr1;

//MHJSqlTracer tracerMHJDataBaseServer;

MHJDataBaseServer::MHJDataBaseServer(MHJRedisBaseFactory* redisbasefactory) :
		MHJDataBaseFactory(redisbasefactory) {
	mMutexChildMap = new sp_thread_mutex_t;
	mMutexDevice = new sp_thread_mutex_t;
	mMutexVirtualDefineMap = new sp_thread_mutex_t;

	sp_thread_mutex_init(mMutexChildMap, NULL);
	sp_thread_mutex_init(mMutexDevice, NULL);
	sp_thread_mutex_init(mMutexVirtualDefineMap, NULL);
}

MHJDataBaseServer::~MHJDataBaseServer() {
	sp_thread_mutex_destroy (mMutexChildMap);
	sp_thread_mutex_destroy (mMutexDevice);
	sp_thread_mutex_destroy (mMutexVirtualDefineMap);
}

std::tr1::shared_ptr<MHJDeviceChild> MHJDataBaseServer::selectDeviceChildForDB(uint32_t deviceDBID, uint32_t switchNumber) {
//	shared_ptr < odb::mysql::database > db = getDatabase();
//	shared_ptr < MHJDeviceChild > childEntity;
//	{
//		typedef odb::query<MHJDeviceChild> query;
//		typedef odb::result<MHJDeviceChild> result;
//
//		odb::transaction t(db->begin());
//		result r(db->query < MHJDeviceChild > (query::deviceID == deviceDBID && query::keyID == switchNumber));
//		for (result::iterator i(r.begin()); i != r.end(); ++i) {
//			childEntity = i.load();
//			break;
//		}
//	}
	std::tr1::shared_ptr < MHJDeviceChild > childEntity;
	MHJDBInterface http;
	std::string response;
	if (http.selectDeviceChildByDeviceDBIDAndSwitchNumber(deviceDBID, switchNumber, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		childEntity = json2MHJDeviceChild(data);
	}

	return childEntity;
}

uint8_t MHJDataBaseServer::selectDeviceChildDefineListCount(uint32_t deviceDBID, uint32_t switchNumber) {
	uint64_t key;
	uint32_t* pkey = (uint32_t*) &key;
	pkey[0] = deviceDBID;
	pkey[1] = switchNumber;

	std::tr1::shared_ptr < SP_Buffer > virtualList;

	sp_thread_mutex_lock (mMutexVirtualDefineMap);
	MAP_VritualDefine::const_iterator got = mVirtualDefineMap_KeyDeviceIDKeyIndex.find(key);
	if (got != mVirtualDefineMap_KeyDeviceIDKeyIndex.end()) {
		virtualList = got->second;
	}
	sp_thread_mutex_unlock(mMutexVirtualDefineMap);

	if (virtualList.get() == NULL) {
//		shared_ptr < odb::mysql::database > db = getDatabase();
		std::tr1::shared_ptr < MHJDeviceChild > childEntity;
//		{
//			typedef odb::query<MHJDeviceChild> query;
//			typedef odb::result<MHJDeviceChild> result;
//
//			odb::transaction t(db->begin());
//			result r(
//					db->query < MHJDeviceChild
//							> (query::deviceID == deviceDBID && query::keyID == switchNumber && query::KeyDefine >= MDKTS_VIRTUAL_KEY1
//									&& query::KeyDefine < MDKTS_VIRTUAL_KEY_LIMIT));
//			for (result::iterator i(r.begin()); i != r.end(); ++i) {
//				childEntity = i.load();
//				break;
//			}
		MHJDBInterface mhjdb;
		std::string response;
		if (mhjdb.selectDeviceChildByDeviceDBIDAndSwitchNumber(deviceDBID, switchNumber, &response)) {
			Json::Value data = jsonGetMHJReturnData(response);
			childEntity = json2MHJDeviceChild(data);
		}
		if (childEntity.get()) {
			MHJDeviceChild* pChildEntity = childEntity.get();

			std::tr1::shared_ptr < MHJDeviceVirtualDefine > virtualDefine;
			virtualList = std::tr1::shared_ptr < SP_Buffer > (new SP_Buffer);
//			{
//				typedef odb::query<MHJDeviceVirtualDefine> query;
//				typedef odb::result<MHJDeviceVirtualDefine> result;
//
//				odb::transaction t(db->begin());
//				result r(db->query < MHJDeviceVirtualDefine > (query::deviceKeyID == childEntity->id_));
//				for (result::iterator i(r.begin()); i != r.end(); ++i) {
//					virtualDefine = i.load();
//					virtualList->append(virtualDefine.get(), sizeof(MHJDeviceVirtualDefine));
//				}
			response = "";
			std::vector < std::tr1::shared_ptr<MHJDeviceVirtualDefine> > vector;
			if (mhjdb.selectMHJDeviceVirtualDefines(pChildEntity->id_, &response)) {
				Json::Value data = jsonGetMHJReturnData(response);
				if (json2MHJDeviceVirtualDefines(data, &vector)) {
					for (std::vector<std::tr1::shared_ptr<MHJDeviceVirtualDefine> >::iterator it = vector.begin(); it != vector.end(); ++it) {
						virtualDefine = *it;
						virtualList->append(virtualDefine.get(), sizeof(MHJDeviceVirtualDefine));
					}
				}
			}

			sp_thread_mutex_lock(mMutexVirtualDefineMap);
			mVirtualDefineMap_KeyDeviceIDKeyIndex.insert(MAP_VritualDefine::value_type(key, virtualList));
			sp_thread_mutex_unlock(mMutexVirtualDefineMap);
		}
	}
	if (virtualList.get())
		return virtualList->getSize() / sizeof(MHJDeviceVirtualDefine);
	else
		return 0;
}

void MHJDataBaseServer::RemoveDeviceChildDefineListCache(uint32_t deviceDBID, uint32_t switchNumber) {
	uint64_t key;
	uint32_t* pkey = (uint32_t*) &key;
	pkey[0] = deviceDBID;
	pkey[1] = switchNumber;

	sp_thread_mutex_lock (mMutexVirtualDefineMap);
	MAP_VritualDefine::const_iterator got = mVirtualDefineMap_KeyDeviceIDKeyIndex.find(key);
	if (got != mVirtualDefineMap_KeyDeviceIDKeyIndex.end()) {
		mVirtualDefineMap_KeyDeviceIDKeyIndex.erase(got);
	}
	sp_thread_mutex_unlock(mMutexVirtualDefineMap);
}

void MHJDataBaseServer::selectDeviceChildDefineUpdateTime(std::tr1::shared_ptr<MHJDeviceBase> device, uint32_t* virtualKeyUpdateTime) {
	std::tr1::shared_ptr < MHJDeviceChild > childEntity;
	MHJDeviceChild* childlist[64] = { 0 };
	std::vector < std::tr1::shared_ptr<MHJDeviceChild> > retObjList;
	MHJDBInterface mhjdb;
	std::string response;

	if (mhjdb.selectDeviceChildsByDeviceDBID(device->id_, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		if (json2MHJDeviceChilds(data, &retObjList)) {
			for (std::vector<std::tr1::shared_ptr<MHJDeviceChild> >::iterator i = retObjList.begin(); i != retObjList.end(); ++i) {
				childEntity = *i;
				sp_thread_mutex_lock (mMutexChildMap);
				MAP_DeviceChild::const_iterator got = mChildMap_KeyChildID.find(childEntity->id_);
				if (got != mChildMap_KeyChildID.end()) {
					mChildMap_KeyChildID.erase(got);
				}
				mChildMap_KeyChildID.insert(MAP_DeviceChild::value_type(childEntity->id_, childEntity));
				sp_thread_mutex_unlock(mMutexChildMap);

				int keyindex = childEntity->keyID;
				if (keyindex > 0 && keyindex <= 6 && childEntity->KeyDefine > MDKTS_KEY_SWITCH_LIMIT) //6为8266预设储存区的大小，8266预设储存区为6个。
						{
					childlist[childEntity->keyID - 1] = childEntity.get();
				}
//				char strKeydefine[16];
//				sprintf(strKeydefine, "%d", childEntity->KeyDefine);
//				char* findstr = strstr(device->virtualKeyDefine, strKeydefine);
//				if (findstr) {
//					uint32_t pos = device->virtualKeyDefine - findstr;
//					uint32_t index = pos / 2;
//					childlist[index] = childEntity.get();
//				}
//				if (childEntity->KeyDefine >= MDKTS_VIRTUAL_KEY1 && childEntity->KeyDefine < MDKTS_VIRTUAL_KEY_LIMIT)
//					childlist[childEntity->KeyDefine - MDKTS_VIRTUAL_KEY1] = childEntity.get();
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		if (childlist[i]) {
			if (!childlist[i]->defineChangeTime.is_not_a_date_time()) {
				tm tm1 = to_tm(childlist[i]->defineChangeTime);
				time_t tt = mktime(&tm1);
				virtualKeyUpdateTime[i] = tt;
			} else
				virtualKeyUpdateTime[i] = 0;
		}
	}
}

MHJDeviceVirtualDefine* MHJDataBaseServer::selectDeviceChildDefine(uint32_t deviceDBID, uint32_t switchNumber, uint8_t index) {
	uint64_t key;
	uint32_t* pkey = (uint32_t*) &key;
	pkey[0] = deviceDBID;
	pkey[1] = switchNumber;

	std::tr1::shared_ptr < SP_Buffer > virtualList;

	sp_thread_mutex_lock (mMutexVirtualDefineMap);
	MAP_VritualDefine::const_iterator got = mVirtualDefineMap_KeyDeviceIDKeyIndex.find(key);
	if (got != mVirtualDefineMap_KeyDeviceIDKeyIndex.end()) {
		virtualList = got->second;
	}
	sp_thread_mutex_unlock(mMutexVirtualDefineMap);
	if (virtualList.get()) {
		MHJDeviceVirtualDefine* mvritualDefine = (MHJDeviceVirtualDefine*) virtualList->getBuffer();
		if (index < virtualList->getSize() / sizeof(MHJDeviceVirtualDefine)) {
			return &mvritualDefine[index];
		}
	}
	return NULL;
}

std::tr1::shared_ptr<MHJDeviceBase> MHJDataBaseServer::selectDeviceByDBID(uint32_t deviceDbID) {

//	shared_ptr < odb::mysql::database > db = getDatabase();
	std::tr1::shared_ptr < MHJDeviceBase > device;

	sp_thread_mutex_lock (mMutexDevice);
	MAP_Device::const_iterator got = mDevice_KeyID.find(deviceDbID);
	if (got != mDevice_KeyID.end()) {
		device = got->second;
//		return device;
	}
	sp_thread_mutex_unlock(mMutexDevice);
	if (device.get())
		return device;

//	{
//		typedef odb::query<MHJDeviceBase> query;
//		typedef odb::result<MHJDeviceBase> result;
//
//		odb::transaction t(db->begin());
//		result r(db->query < MHJDeviceBase > (query::id == deviceDbID));
//		for (result::iterator i(r.begin()); i != r.end(); ++i) {
//			device = i.load();
//			sp_thread_mutex_lock(mMutexDevice);
//			mDevice_KeyID.insert(MAP_Device::value_type(device->id_, device));
//			sp_thread_mutex_unlock(mMutexDevice);
//			break;t
//		}
//	}
	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getMhjDeviceBaseByDBID(deviceDbID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		device = json2MHJDeviceBase(data);
		if (device.get()) {
			sp_thread_mutex_lock(mMutexDevice);
			mDevice_KeyID.insert(MAP_Device::value_type(device->id_, device));
			sp_thread_mutex_unlock(mMutexDevice);
		}
	}

	return device;
}
