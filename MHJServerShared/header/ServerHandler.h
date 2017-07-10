/*
 * DeviceHandler.h
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#ifndef DEVICEHANDLER_H_
#define DEVICEHANDLER_H_

#include "sphandler.hpp"
#include "spthreadpool.hpp"
#include "SP_OnlineManager.h"
#include "ProcessFactory.h"
#include "MHJSecurityFactory.h"

class MHJServerHandler: public SP_Handler {
public:
	MHJServerHandler(SP_OnlineManager* mOnlineManager, ProcessFactory *processFactory, MHJSecurityFactory *pSecurity);
	virtual ~MHJServerHandler();

	// return -1 : terminate session, 0 : continue
	virtual int start(SP_Request * request, SP_Response * response);

	// return -1 : terminate session, 0 : continue
	virtual int handle(SP_Request * request, SP_Response * response);

	virtual void error(SP_Response * response);

	virtual void timeout(SP_Response * response);

	virtual void close();

	void setkey(int size);

private:
	SP_OnlineManager* mOnlineManager;
	int mChatID;
	SP_Sid_t mSid;
	ProcessFactory *mprocessFactory;
	MHJSecurityFactory *mSecurity;

};

#endif /* DEVICEHANDLER_H_ */
