/*
 * MHJSecurityManageDevice.h
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#ifndef MHJSECURITYMANAGEDEVICE_H_
#define MHJSECURITYMANAGEDEVICE_H_

#include "MHJSecurityManageFactory.h"

class MHJSecurityManageDevice: public MHJSecurityManageFactory {
public:
	MHJSecurityManageDevice();
	virtual ~MHJSecurityManageDevice();
	typedef boost::unordered_map<int, std::tr1::shared_ptr<MHJDeviceBase> > MAP_Database;

	virtual char* getSecurity(const MHJDeviceMark* pdevice);
	virtual char* getSessionSecurity(const MHJDeviceMark* pdevice);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

	virtual std::tr1::shared_ptr<MHJDeviceBase> getDatabaseModel(const MHJDeviceMark* pdevice);

//	void updateDatabase(std::tr1::shared_ptr<MHJDeviceBase> mhjdevicebase);

protected:
	void pushToMachineCache(std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase);
	std::tr1::shared_ptr<MHJDeviceBase> findToMachineCache(const MHJDeviceMark* pdevice);
	std::tr1::shared_ptr<MHJDeviceBase> findToSessionCache(const MHJDeviceMark* pdevice);
	void pushToSessionCache(int deviceID, std::tr1::shared_ptr<MHJDeviceBase> deviceDatabase);
	void getOrCreateSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

private:
	MAP_Database mSecurityFactoryDatabaseCache;
	sp_thread_mutex_t *mMutexMachineDatabaseCache;
	MAP_Database mSecurityFactorySessionCache;
	sp_thread_mutex_t *mMutexMachineSessionCache;
};

#endif /* MHJSECURITYMANAGEDEVICE_H_ */
