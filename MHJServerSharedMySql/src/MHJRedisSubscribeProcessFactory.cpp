/*
 * MHJRedisSubscribeProcess.cpp
 *
 *  Created on: 2015年11月16日
 *      Author: ruancongming
 */
#include "ProtocolServer.h"

#include "MHJRedisSubscribeProcessFactory.h"
#include "spresponse.hpp"

MHJRedisSubscribeProcessFactory::MHJRedisSubscribeProcessFactory(SP_Server *spserver) {
    this->pspserver = spserver;
}

MHJRedisSubscribeProcessFactory::~MHJRedisSubscribeProcessFactory() {

}

void MHJRedisSubscribeProcessFactory::ProcessLogIn(char *source, boost::shared_ptr<SP_Buffer> mbuffer) {
}

void MHJRedisSubscribeProcessFactory::ProcessServerInfo(char *source, boost::shared_ptr<SP_Buffer> mbuffer) {
}

void MHJRedisSubscribeProcessFactory::ProcessHeartBeat(char *source, boost::shared_ptr<SP_Buffer> mbuffer) {
}

void MHJRedisSubscribeProcessFactory::ProcessCommand(char *source, boost::shared_ptr<SP_Buffer> mbuffer) {
}

void MHJRedisSubscribeProcessFactory::ProcessOnlineInform(char *source, boost::shared_ptr<SP_Buffer> mbuffer) {
}

void MHJRedisSubscribeProcessFactory::pushSPServerResponseMessage(SP_Sid_t *sid, const SP_Buffer *buff) {
    SP_Response *response = new SP_Response(*sid);
    SP_Message *message = new SP_Message();
    message->getMsg()->append(buff);
    message->getToList()->add(*sid);
    response->addMessage(message);
    pspserver->pushResponseQueue(response);
}
