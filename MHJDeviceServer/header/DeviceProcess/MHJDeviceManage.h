/*
 * MHJDeviceManage.h
 *
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#ifndef MHJDEVICEMANAGE_H_
#define MHJDEVICEMANAGE_H_

#include "MHJDataBaseFactory.h"
#include <boost/unordered_map.hpp>
#include "MHJDefine.h"
#include <spthread.hpp>

class MHJDeviceManage {
public:
	MHJDeviceManage();
	virtual ~MHJDeviceManage();

	typedef boost::unordered_map<uint64_t, std::tr1::shared_ptr<MHJDeviceChild> > MAP_MHJDeviceChild;

	std::tr1::shared_ptr<MHJDeviceChild> getDeviceChild(uint64_t dbKeyID);
//	virtual void updateDeviceChild(std::tr1::shared_ptr<MHJDeviceChild> devicechild);

	void setDataBaseFactory(MHJDataBaseFactory *dataBaseFactory);
	MHJDataBaseFactory * getDataBaseFactory();
private:

	void pushToCache(std::tr1::shared_ptr<MHJDeviceChild> deviceChild);
	std::tr1::shared_ptr<MHJDeviceChild> findByCache(uint64_t dbkey);
	std::tr1::shared_ptr<MHJDeviceChild> findByDb(uint64_t dbkey);

	MAP_MHJDeviceChild mChildDatabaseCacheKyeID;
	sp_thread_mutex_t *mMutexChildDatabaseCache;

	MHJDataBaseFactory *mDataBaseFactory;
};

#endif /* MHJDEVICEMANAGE_H_ */
