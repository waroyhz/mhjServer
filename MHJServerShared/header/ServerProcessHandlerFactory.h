/*
 * DeviceServer.h
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#ifndef DEVICESERVER_H_
#define DEVICESERVER_H_

//#include "sphandler.hpp"
#include "ServerHandler.h"
#include "SP_OnlineManager.h"
#include "ProcessFactory.h"
//#include "MHJSecurity.h"
class MHJSecurityFactory;
//class SP_HandlerFactory;

class MHJServerHandlerFactory: public SP_HandlerFactory {
public:
	MHJServerHandlerFactory(ProcessFactory *processFactory, MHJSecurityFactory *security);
	virtual ~MHJServerHandlerFactory();

	virtual SP_Handler * create() const;

//	virtual SP_CompletionHandler * createCompletionHandler() const;
private:
	SP_OnlineManager* mOnlineManager;
	ProcessFactory *mprocessFactory;
	MHJSecurityFactory *mSecurity;
};

#endif /* DEVICESERVER_H_ */
