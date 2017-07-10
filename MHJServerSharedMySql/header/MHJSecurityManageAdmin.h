/*
 * MHJSecurityManageAdmin.h
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#ifndef MHJSecurityManageAdmin_H_
#define MHJSecurityManageAdmin_H_

#include "MHJSecurityManageFactory.h"

#define ADMIN_DATABASE_DEVICEID 113500
#define ADMIN_DATABASE_SIZE		100

class MHJSecurityManageAdmin: public MHJSecurityManageFactory {
public:
	MHJSecurityManageAdmin();
	virtual ~MHJSecurityManageAdmin();
	typedef boost::unordered_map<int,std::tr1::shared_ptr<MHJAdminEntity> > MAP_Database;

	virtual char* getSecurity(const MHJDeviceMark* pdevice);
	virtual char* getSessionSecurity(const MHJDeviceMark* pdevice);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

protected:
	void pushToMachineCache(std::tr1::shared_ptr<MHJAdminEntity> deviceDatabase);
	std::tr1::shared_ptr<MHJAdminEntity> findToMachineCache(const MHJDeviceMark* pdevice);
	std::tr1::shared_ptr<MHJAdminEntity> findToSessionCache(const MHJDeviceMark* pdevice);
	void pushToSessionCache(int deviceID,std::tr1::shared_ptr<MHJAdminEntity> deviceDatabase);
	void getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

private :
	MAP_Database mSecurityFactoryDatabaseCache;
   sp_thread_mutex_t *mMutexMachineDatabaseCache;
	 MAP_Database mSecurityFactorySessionCache;
	 sp_thread_mutex_t *mMutexMachineSessionCache;
};

#endif /* MHJSecurityManageAdmin_H_ */
