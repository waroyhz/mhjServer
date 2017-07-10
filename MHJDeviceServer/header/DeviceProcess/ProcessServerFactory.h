/*
 * ProcessHostFactory.h
 *
 *  Created on: 2015-7-10
 *      Author: waroy
 */

#ifndef PROCESSSERVERFACTORY_H_
#define PROCESSSERVERFACTORY_H_

#include <ProcessFactory.h>
#include <stdio.h>
#include <iostream>
#include "MHJDeviceManage.h"

using namespace boost;

const boost::shared_ptr<MHJProtocol> initprotocol;

class ProcessServerFactory: public ProcessFactory {
public:
	ProcessServerFactory(SP_Request *request = NULL, SP_Response *response = NULL, boost::shared_ptr<MHJProtocol> protocol = initprotocol,
			MHJProtocolType classProtocolType = MPT_NONE);
	virtual ~ProcessServerFactory();

	virtual ProcessFactory * getProcess(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol);

	virtual void setDatabaseFactory(MHJDataBaseFactory*dataBaseFactory);

	virtual MHJDataBaseFactory* getDatabaseFactory();
	virtual void Release(void* ptr);

	virtual void ProtocolOffline(boost::shared_ptr<MHJProtocol> protocol);
	virtual void ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol);

	virtual void clientClose(SP_Sid_t sid);

	virtual void ProtocolTransferToJava(boost::shared_ptr<MHJProtocol> protocol);
protected:
	static MHJDeviceManage mDeviceManage;
};

#endif /* PROCESSHOSTFACTORY_H_ */
