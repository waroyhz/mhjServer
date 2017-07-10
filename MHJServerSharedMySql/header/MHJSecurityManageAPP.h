/*
 * MHJSecurityManageAPP.h
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#ifndef MHJSecurityManageAPP_H_
#define MHJSecurityManageAPP_H_

#include "MHJSecurityManageFactory.h"

class MHJSecurityManageAPP: public MHJSecurityManageFactory {
public:
	MHJSecurityManageAPP();
	virtual ~MHJSecurityManageAPP();
	typedef boost::unordered_map<int, std::tr1::shared_ptr<MHJUserEntity> > MAP_Database;

	virtual char* getSecurity(const MHJDeviceMark* pdevice);
	virtual char* getSessionSecurity(const MHJDeviceMark* pdevice);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

	std::tr1::shared_ptr<MHJUserEntity> getDatabaseModel(const MHJDeviceMark* pdevice);

protected:
	void pushToMachineCache(std::tr1::shared_ptr<MHJUserEntity> deviceDatabase);
	void removeToMachineCache(std::tr1::shared_ptr<MHJUserEntity> deviceDatabase);
	std::tr1::shared_ptr<MHJUserEntity> findToMachineCache(const MHJDeviceMark* pdevice);
	std::tr1::shared_ptr<MHJUserEntity> findToSessionCache(const MHJDeviceMark* pdevice);
	void pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJUserEntity> userentity);
	void getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

private:
	MAP_Database mSecurityFactoryDatabaseCache;
	sp_thread_mutex_t *mMutexMachineDatabaseCache;
	MAP_Database mSecurityFactorySessionCache;
	sp_thread_mutex_t *mMutexMachineSessionCache;
};

#endif /* MHJSecurityManageAPP_H_ */
