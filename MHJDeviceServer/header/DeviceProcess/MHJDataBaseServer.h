/*
 * MHJDataBaseFactoryServer.h
 *
 *  Created on: Jul 31, 2015
 *      Author: ruancongming
 */
#include "MHJDataBaseFactory.h"
#include <spbuffer.hpp>
#include <boost/unordered_map.hpp>
#include <spthread.hpp>

#ifndef MHJDATABASEFACTORYSERVER_H_

#define MHJDATABASEFACTORYSERVER_H_
class MHJDataBaseServer: public MHJDataBaseFactory {
public:
	MHJDataBaseServer(MHJRedisBaseFactory* redisbasefactory);
	~MHJDataBaseServer();

	typedef boost::unordered_map<uint64_t, std::tr1::shared_ptr<SP_Buffer> > MAP_VritualDefine;
	typedef boost::unordered_map<uint64_t, std::tr1::shared_ptr<MHJDeviceChild> > MAP_DeviceChild;
	typedef boost::unordered_map<uint64_t, std::tr1::shared_ptr<MHJDeviceBase> > MAP_Device;

	std::tr1::shared_ptr<MHJDeviceChild> selectDeviceChildForDB(uint32_t deviceDBID, uint32_t switchNumber);
	uint8_t selectDeviceChildDefineListCount(uint32_t deviceDBID, uint32_t switchNumber);
	void RemoveDeviceChildDefineListCache(uint32_t deviceDBID, uint32_t switchNumber);

	void selectDeviceChildDefineUpdateTime(std::tr1::shared_ptr<MHJDeviceBase> device, uint32_t* virtualKeyUpdateTime);

	MHJDeviceVirtualDefine* selectDeviceChildDefine(uint32_t deviceDBID, uint32_t switchNumber, uint8_t index);
	std::tr1::shared_ptr<MHJDeviceBase> selectDeviceByDBID(uint32_t deviceDbID);

private:
	MAP_VritualDefine mVirtualDefineMap_KeyDeviceIDKeyIndex;
	MAP_DeviceChild mChildMap_KeyChildID;
	MAP_Device mDevice_KeyID;

	sp_thread_mutex_t *mMutexVirtualDefineMap;
	sp_thread_mutex_t *mMutexChildMap;
	sp_thread_mutex_t *mMutexDevice;
};

#endif /* MHJDATABASEFACTORYSERVER_H_ */
