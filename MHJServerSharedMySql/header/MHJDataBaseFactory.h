/*
 * mhjDB.h
 *
 *  Created on: 2015-7-21
 *      Author: waroy
 */

#ifndef MHJDataBaseFactory_H_
#define MHJDataBaseFactory_H_

#include <tr1/memory>
#include <tr1/shared_ptr.h>

#include "MHJDatabase.hxx"
#ifdef SQLITE
#include <odb/session.hxx>
#endif
#ifdef DATABASE_MYSQL
#include <odb/mysql/database.hxx>
#include "MHJDatabase-odb.hxx"
#include <vector>
#endif

#ifdef DATABASE_SQLITE
#include <odb/sqlite/database.hxx>
#endif
//#ifdef DATABASE_REDIS
#include "MHJRedisBaseFactory.h"
//#endif
//#include <tr1/shared_ptr.h>
//using namespace std::tr1;

class MHJDataBaseFactory {
public:
	MHJDataBaseFactory(MHJRedisBaseFactory* redisbasefactory);
	virtual ~MHJDataBaseFactory();

	virtual std::tr1::shared_ptr<MHJDeviceBase> selectDeviceSecurity(uint32_t deviceID, uint16_t idSecurity, uint8_t deviceType);
	virtual std::tr1::shared_ptr<MHJUserEntity> selectUserEntity(uint32_t userID);
	virtual std::tr1::shared_ptr<MHJAdminEntity> selectAdminEntity(uint32_t adminID);
	virtual std::tr1::shared_ptr<MHJDeviceChild> selectDeviceChild(uint32_t childID);
	virtual std::tr1::shared_ptr<MHJDeviceActivation> selectDeviceMHJDeviceActivation(uint32_t deviceDBID);
	virtual void selectDeviceChildMHJDeviceAuthorize(uint32_t childDBID, std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> > *vector);

//	virtual std::tr1::shared_ptr<odb::mysql::database> getDatabase();

//	virtual void updateDeviceBase(std::tr1::shared_ptr<MHJDeviceBase> devicebase);
	virtual void updateMHJDeviceActivationsoftVer(std::tr1::shared_ptr<MHJDeviceActivation> activation);
	virtual void updateDeviceChildState(std::tr1::shared_ptr<MHJDeviceChild> devicechild);

	void selectUserSecurityByIDList(std::vector<unsigned long> &useridList, std::vector<std::tr1::shared_ptr<MHJUserEntity> > *vector);
	MHJRedisBaseFactory * GetRedisBaseFactory();

protected:
#ifdef DATABASE_MYSQL
	static std::auto_ptr<odb::mysql::connection_factory> odbfactory;
#endif
#ifdef DATABASE_SQLITE
	static std::auto_ptr<odb::sqlite::connection_factory> odbfactory;
#endif
//#ifdef DATABASE_REDIS
	MHJRedisBaseFactory *redisfactory;
//#endif
};

#endif /* MHJDB_H_ */
