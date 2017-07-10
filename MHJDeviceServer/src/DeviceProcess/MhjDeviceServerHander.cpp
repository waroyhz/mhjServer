//
// Created by whyxp on 16-9-8.
//

#include <iostream>
#include <MHJRedisBaseFactory.h>
#include "MhjDeviceServerHander.h"


MhjDeviceServerHander::MhjDeviceServerHander(uint32_t deviceId, uint8_t deviceType) {

    char Key[64];
    sprintf(Key, "OnlineDevice_Type_%d_ID_%d", deviceType, deviceId);
    printf("deviceServer offLine device %s \r\n",Key);

    MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	redisReply* reply =redisfactory->RedisCommand_DEL(Key);
}
