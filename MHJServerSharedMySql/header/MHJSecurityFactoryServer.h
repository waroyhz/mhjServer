/*
 * MHJSecurityServer.h
 *
 *  Created on: 2015-7-31
 *      Author: waroy
 */

#ifndef MHJSECURITYSERVER_H_
#define MHJSECURITYSERVER_H_

#include "MHJSecurityFactory.h"
#include "MHJSecurityManageDevice.h"
#include "MHJSecurityManageServer.h"
#include "MHJSecurityManageAPP.h"
#include "MHJSecurityManageHost.h"
#include "MHJSecurityManageAdmin.h"

class MHJSecurityFactoryServer: public MHJSecurityFactory {
public:
	MHJSecurityFactoryServer();
	virtual ~MHJSecurityFactoryServer();

	virtual char* getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);

	void setFactory(ProcessFactory *mProcessFactory, MHJDataBaseFactory* mDataBaseFactory);

	virtual void* getDataBaseFactory();
	virtual void* getDatabaseManage(MHJDeviceMark* mark);

protected:
	MHJSecurityManageFactory* getSecurityManage(const MHJDeviceMark* pdevice);
	ProcessFactory *mProcessFactory;
	MHJDataBaseFactory* mDataBaseFactory;

	MHJSecurityManageFactory mSecurityManageFactory;
	MHJSecurityManageDevice mSecurityDevice;
	MHJSecurityManageServer mSecurityServer;
	MHJSecurityManageAPP mSecurityAPP;
	MHJSecurityManageHost mSecurityHost;
	MHJSecurityManageAdmin mSecurityAdmin;

};

#endif /* MHJSECURITYSERVER_H_ */
