/*
 * MHJSecurityManageServer.h
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#ifndef MHJSecurityManageServer_H_
#define MHJSecurityManageServer_H_

#include "MHJSecurityManageFactory.h"

class MHJSecurityManageServer: public MHJSecurityManageFactory {
public:
	MHJSecurityManageServer();
	virtual ~MHJSecurityManageServer();
	typedef boost::unordered_map<int,std::tr1::shared_ptr<MHJDeviceBase> > MAP_Database;

	virtual char* getSecurity(const MHJDeviceMark* pdevice);
	virtual char* getSessionSecurity(const MHJDeviceMark* pdevice);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

protected:
	void pushToMachineCache(std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase);
	std::tr1::shared_ptr<MHJDeviceBase> findToMachineCache(const MHJDeviceMark* pdevice);
	std::tr1::shared_ptr<MHJDeviceBase> findToSessionCache(const MHJDeviceMark* pdevice);
	void pushToSessionCache(int deviceID,std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase);
	void getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

private :
	MAP_Database mSecurityFactoryDatabaseCache;
   sp_thread_mutex_t *mMutexMachineDatabaseCache;
	 MAP_Database mSecurityFactorySessionCache;
	 sp_thread_mutex_t *mMutexMachineSessionCache;
};

#endif /* MHJSecurityManageServer_H_ */
