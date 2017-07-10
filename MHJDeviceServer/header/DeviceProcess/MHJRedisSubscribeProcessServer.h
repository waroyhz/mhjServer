/*
 * MHJRedisSubscribeProcessServer.h
 *
 *  Created on: 2015年11月21日
 *      Author: ruancongming
 */

#ifndef DEVICEPROCESS_MHJREDISSUBSCRIBEPROCESSSERVER_H_
#define DEVICEPROCESS_MHJREDISSUBSCRIBEPROCESSSERVER_H_

#include <MHJRedisSubscribeProcessFactory.h>
#include  "ProtocolTypeDefine.h"
#include "MHJSecurityFactoryServer.h"
#include <boost/shared_ptr.hpp>
//设备服务器，订阅处理函数
/*
 * ，设置订阅：以设备号作为订阅号
 *
 */
using namespace boost;

class MHJRedisSubscribeProcessServer: public MHJRedisSubscribeProcessFactory {
public:

	MHJRedisSubscribeProcessServer(MHJSecurityFactoryServer* securityfactoryserver, SP_Server* spserver);
	virtual ~MHJRedisSubscribeProcessServer();

	void ProcessCommand(char* source, boost::shared_ptr<SP_Buffer> mbuffer);
	void ProcessOnlineInform(char* source,boost::shared_ptr<SP_Buffer> mbuffer);

//	void pushSPServerResponseMessage(SP_Sid_t * sid,const SP_Buffer* buff);
private:
	MHJSecurityFactoryServer* securityfactoryserver;
};

#endif /* DEVICEPROCESS_MHJREDISSUBSCRIBEPROCESSSERVER_H_ */
