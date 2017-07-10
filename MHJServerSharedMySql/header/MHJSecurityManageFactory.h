/*
 * MHJSecurityManageFactory.h
 *
 *  Created on: 2015-8-13
 *      Author: waroy
 */

#ifndef MHJSECURITYMANAGEFACTORY_H_
#define MHJSECURITYMANAGEFACTORY_H_

#include "ProcessFactory.h"
#include "MHJDataBaseFactory.h"

class MHJSecurityManageFactory {
public:
	MHJSecurityManageFactory();
	virtual ~MHJSecurityManageFactory();

	virtual char* getSecurity(const MHJDeviceMark* pdevice);
	virtual char* getSessionSecurity(const MHJDeviceMark* pdevice);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);
	virtual void createSessionSecurityData(char* outSecurityData);
	char* getGeneralSecurity();

	virtual void* getDatabaseModel(MHJDeviceMark* mark);

	virtual void setDatabaseFactory(MHJDataBaseFactory* dataBaseFactory);

protected:

	static MHJDataBaseFactory* mDataBaseFactory;
	char mGeneralSecurity[SECURITYSIZE];
	//错误的安全码
	char mErrSecurity[SECURITYSIZE];
};

#endif /* MHJSECURITYMANAGEFACTORY_H_ */
