/*
 * MHJRedisSubscribeFactory.h
 *
 *  Created on: 2015年11月13日
 *      Author: ruancongming
 */

#ifndef MHJREDISSUBSCRIBEFACTORY_H_
#define MHJREDISSUBSCRIBEFACTORY_H_
#include <stdio.h>
#include <stdlib.h>
#include <hiredis.h>
#include <async.h>
#include <adapters/libevent.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "ProtocolTypeDefine.h"
#include "MHJDefine.h"
#include "spbuffer.hpp"

#include "MHJRedisSubscribeProcessFactory.h"
using namespace boost;

class MHJRedisSubscribeFactory {
public:
	MHJRedisSubscribeFactory(char* ip,int port, char* passwd, char* channel,MHJRedisSubscribeProcessFactory* process);
	virtual ~MHJRedisSubscribeFactory();
	virtual void auth(char* passwd);
	virtual void subscribe(char* channel);
	virtual void DoSubscribe();
	static void publish(BYTE type, char* serverId, MHJDeviceMark* deviceMark, WORD length, void* data, char* channel);
//	virtual void DoPublish();
	virtual void setChannel(char *channel);
	virtual char* getCmd();
	static boost::shared_ptr < SP_Buffer> RedisSubscribeProtocolPackage(BYTE mprotocolType, char *source, MHJDeviceMark* pdevice, WORD mlength,
				void *pdata);

	virtual MHJRedisSubscribeProcessFactory* getProcess();
private:
	   struct event_base *eventBase;
	   redisAsyncContext *asyncContext ;
	   char* passwd;
	   char* redisCmd;
	   char* channel;

	   MHJRedisSubscribeProcessFactory* process;
};

#endif /* MHJREDISSUBSCRIBEFACTORY_H_ */
