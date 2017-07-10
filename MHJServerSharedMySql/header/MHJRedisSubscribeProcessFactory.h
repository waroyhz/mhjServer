/*
 * MHJRedisSubscribeProcess.h
 *
 *  Created on: 2015年11月16日
 *      Author: ruancongming
 */

#ifndef MHJREDISSUBSCRIBEPROCESSFACTORY_H_
#define MHJREDISSUBSCRIBEPROCESSFACTORY_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "spbuffer.hpp"
#include "ProtocolTypeDefine.h"
#include "spserver.hpp"
#include "speventcb.hpp"

using namespace boost;

class MHJRedisSubscribeProcessFactory {
public:
    MHJRedisSubscribeProcessFactory(SP_Server *spserver);

    virtual ~MHJRedisSubscribeProcessFactory();

    virtual void ProcessLogIn(char *source, boost::shared_ptr<SP_Buffer> mbuffer);

    virtual void ProcessHeartBeat(char *source, boost::shared_ptr<SP_Buffer> mbuffer);

    virtual void ProcessServerInfo(char *source, boost::shared_ptr<SP_Buffer> mbuffer);

    virtual void ProcessCommand(char *source, boost::shared_ptr<SP_Buffer> mbuffer);

    virtual void ProcessOnlineInform(char *source, boost::shared_ptr<SP_Buffer> mbuffer);

    virtual void pushSPServerResponseMessage(SP_Sid_t *sid, const SP_Buffer *buff);

protected:
    SP_Server *pspserver;
};

#endif /* MHJREDISSUBSCRIBEPROCESSFACTORY_H_ */
