/*
 * ProcessTransformationRedis.cpp
 *
 *  Created on: 2015年11月25日
 *      Author: ruancongming
 */

#include <iostream>
#include "ProcessTransformationRedis.h"
#include "ProtocolServer.h"
#include "MHJRedisSubscribeFactory.h"
#include "MHJRedisBaseFactory.h"
#include <iostream>

using namespace boost;

ProcessTransformationRedis::ProcessTransformationRedis(SP_Response *response,
                                                       boost::shared_ptr<MHJProtocol> protocol,
                                                       MHJProtocolType protocolType) : ProcessTransformationFactory(
        response, protocol, protocolType) {

}


ProcessTransformationRedis::~ProcessTransformationRedis() {
    // TODO Auto-generated destructor stub
}

bool ProcessTransformationRedis::sendProtocolRedis(
        const MHJDeviceMark *destDevice, BYTE mprotocolID, BYTE mver,
        WORD mlength, void *pdata) {

    MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
    char key[64];
    memset(key, 0, 64);
    sprintf(key, "OnlineDevice_Type_%d_ID_%d", destDevice->deviceType, destDevice->deviceID);

    redisReply *reply = redisfactory->RedisCommand_GET(key);
    if (reply != NULL) {
#ifdef DEBUG
        std::cout << "设备 ID:" << destDevice->deviceID << " Type:" << destDevice->deviceType << "已登陆服务器" << reply->str <<
        std::endl;
#endif
        char *channel = reply->str;

        boost::shared_ptr<SP_Buffer> outBuffer = RedisSubscribeTransferProtocolPackage((MHJDeviceMark *) destDevice,
                                                                                       mprotocolID, mver, mlength,
                                                                                       pdata);
        MHJDeviceMark *srcDevice = this->ProcessTransformationFactory::protocol->device();
        MHJRedisSubscribeFactory::publish(MSPT_CMDTRANSFER, redisfactory->getServiceID(), srcDevice,
                                          outBuffer->getSize(), (void *) outBuffer->getRawBuffer(), channel);
        redisfactory->RedisCommand_FREE(reply);
#ifdef DEBUG
        std::cout << "转发成功" << std::endl;
#endif
        return true;
    } else {
#ifdef DEBUG
        std::cout << "****转发失败，找不到设备登陆的服务器！ key:" << key << std::endl;
#endif
        return false;
    }
}

bool ProcessTransformationRedis::sendProtocol(const MHJDeviceMark *destDevice,
                                              BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata) {
    if (!this->ProcessTransformationFactory::sendProtocol(destDevice,
                                                          mprotocolID, mver, mlength, pdata)) {
#ifdef DEBUG
        printf("****直接发送失败，现进行转发  deviceId：%d\n", destDevice->deviceID);
#endif
        return sendProtocolRedis(destDevice,
                                 mprotocolID, mver, mlength, pdata);
    }
#ifdef DEBUG
    printf("****直接发送成功  deviceId：%d\n", destDevice->deviceID);
#endif
    return true;
}


boost::shared_ptr<SP_Buffer> ProcessTransformationRedis::RedisSubscribeTransferProtocolPackage(
        MHJDeviceMark *destDevice, BYTE mprotocolID, BYTE mver,
        WORD mlength, void *pdata) {

    WORD size = sizeof(MHJSUBSCRIBETRANSFERProtocol) + mlength;

    MHJSUBSCRIBETRANSFERProtocol *txProtocolData = (MHJSUBSCRIBETRANSFERProtocol *) new char[size];
    memset((char *) txProtocolData, 0, size);
    txProtocolData->mpackID = mpackID;
    txProtocolData->mprotocolType = this->mprotocolType;
    txProtocolData->mprotocolID = mprotocolID;
    txProtocolData->mver = mver;

    if (destDevice != NULL)
        memcpy((char *) &txProtocolData->dstdevice, destDevice, sizeof(txProtocolData->dstdevice));
    if (mlength > 0)
        memcpy(&txProtocolData->data, pdata, mlength);

    boost::shared_ptr<SP_Buffer> mbuffer = boost::make_shared<SP_Buffer>();
    mbuffer->append(txProtocolData, size);

    delete[](char *) txProtocolData;
    return mbuffer;
}

