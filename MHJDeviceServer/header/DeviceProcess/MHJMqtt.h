//
// Created by waroy on 16-11-10.
//

#ifndef MHJSPSERVERLIB_MHJMQTT_H
#define MHJSPSERVERLIB_MHJMQTT_H

#include <spserver.hpp>
#include <boost/shared_ptr.hpp>
#include "MHJSecurityDeviceServer.h"

int mhj_mqtt_Client_init(SP_Server *pServer,MHJSecurityDeviceServer *pDeviceServer);
void mhj_mqtt_Client_close();
void mhj_mqtt_publish_Protocol(boost::shared_ptr <MHJProtocol> ptr);

#endif //MHJSPSERVERLIB_MHJMQTT_H
