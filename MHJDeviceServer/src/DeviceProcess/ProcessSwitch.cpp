/*
 * ProcessSwitch.cpp
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */


#include <iostream>
#include <string>

using namespace std;


#include "ProcessSwitch.h"
#include "ProtocolDeviceSwitch.h"
#include "ProtocolToApp.h"
#include "MHJSecurityManageDevice.h"
#include "MHJDataBaseServer.h"
#include <time.h>
#include <json/json.h>
#include <MHJMqtt.h>
#include "MHJDBInterface.h"
#include "ProtocolServer.h"
#include "MHJRedisSubscribeFactory.h"
#include "MHJRedisBaseFactory.h"

ProcessSwitch::ProcessSwitch(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol) :
        ProcessServerFactory(request, response, protocol, MPT_SERVER) {

}

ProcessSwitch::~ProcessSwitch() {

}

void ProcessSwitch::Process() {
#ifdef DEBUG
    printf("%s,%s:%s,%d\n", __DATE__, __TIME__, __FILE__, __LINE__);
    printf("ProcessSwitch::Process, protocol->protocolID = %d \r\n", protocol->protocolID());
#endif
    switch (protocol->protocolID()) {
        case MDPTS_LOGIN:
            ProtocolLogin(protocol);
            break;
        case MDPTS_HEARTBEAT:
//            ProtocolHeartbeat(protocol);
            break;
        case MDPTS_SWITCH_STATE_SET:
            ProtocolSwitchStateSet(protocol);
            break;
        case MDPTS_SYSTEM_UPDATE:
            ProtocolSystemUpdate(protocol);
            break;
        case MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE:
            ProtocolSystemUpdateDownloadComplete(protocol);
            break;
        case MDPTS_SYSTEM_UPDATE_RESTART:
            ProtocolSystemUpdateRestart(protocol);
        case MDPTS_INFO_SYNC:
            ProtocolInfoSync(protocol);
            break;
        case MDPTS_UPDATE_SETUP:
            ProtocolUpdateSetup(protocol);
            break;
        case MDPTS_VIRTUAL_KEY_DEFINE_SYNC:
            ProtocolVirtualKeyDefineSync(protocol);
            break;
        case MDPTS_UPINLESS_SETUP:
            ProtocolUpinlessSetup(protocol);
            break;
        case MDPTS_SOFTRESTART:
            ProtocolSoftRestart(protocol);
            break;
//	case MDPTS_RFIRLEARN :
//		ProtocolSetSwitchRFIRLearn(protocol);
//		break;
//	case MDPTS_RFIRTRANSMIT:
//		ProtocolSetSwitchRFIRTransmit(protocol);
//		break;
        default:
            //其他业务处理，转到java服务器
            this->ProcessServerFactory::ProtocolTransferToJava(protocol);
            break;
    }
//	if (protocol->protocolID()!=MDPTS_LOGIN) //除登录协议外，其他协议均转发到mqtt
    mhj_mqtt_publish_Protocol(protocol);
}

void ProcessSwitch::Release() {
}

//yexiaopeng 20160930
//加入了 V2版本的登录包，新加字段 ssid
void ProcessSwitch::ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_LOGIN_V1 == protocol->Ver()) {
        this->ProcessFactory::ProtocolLogin(protocol);
        MDPTS_LOGIN_RESPONSE_V1 response;
        char sessionOutSecurity[32];
        protocol->createSessionSecurityData(sessionOutSecurity);

        memcpy(response.sessionSecurity, sessionOutSecurity, sizeof(response.sessionSecurity));
        response.serverCount = 0;

        if (protocol->length() > 0) {
            MDPTS_LOGIN_SEND_V1 *requestData = (MDPTS_LOGIN_SEND_V1 *) protocol->data();
            MHJSecurityManageDevice *databaseManage = (MHJSecurityManageDevice *) protocol->getDatabaseManage();
            std::tr1::shared_ptr<MHJDeviceBase> devicebase = databaseManage->getDatabaseModel(protocol->device());
            MHJDataBaseFactory *database = (MHJDataBaseFactory *) protocol->getDataBaseFactory();
            std::tr1::shared_ptr<MHJDeviceActivation> deviceactivation = database->selectDeviceMHJDeviceActivation(
                    devicebase->id_);
            if (deviceactivation.get()) {
                std::string ver = requestData->buildTime;
                if (deviceactivation->softVer != ver) {
                    deviceactivation->softVer = ver;
                    database->updateMHJDeviceActivationsoftVer(deviceactivation);
                }
            }
            //TODO 调度服务器暂不处理内网地址
            char tmp[255];
//			in_addr_t * ipaddress = (in_addr_t*) protocol->data();
            uint8_t *ip = (uint8_t *) &requestData->ip;
            sprintf(tmp, "设备登录客户端内网ip： %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

            std::cout << tmp << std::endl;

            sprintf(tmp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
//TODO 后期需要更改，将数据写入redis里，由java服务器进行处理。
            MHJDBInterface dbpost;
            dbpost.postDevicelogin(protocol->device()->deviceID, protocol->device()->deviceType, protocol->protocolID(),
                                   tmp,
                                   (char *) this->request->getClientIP());

        } else {
            std::cout << "返回地址为空" << std::endl;
        }
//	shared_ptr<IPPORTADDRESS> ipport= DeviceServerManage::getAvailableServer();
        this->ProcessServerFactory::ProtocolLogin(protocol);
        ResponseProtocol(protocol->packID(), protocol->protocolID(), MDPTS_LOGIN_V1, sizeof(MDPTS_LOGIN_RESPONSE_V1),
                         &response);

        //将设备登陆信息发送到java服务器，告知该设备已登陆
        this->ProcessServerFactory::ProtocolTransferToJava(protocol);

    } else if (MDPTS_LOGIN_V2 == protocol->Ver()) {

        struct timeval loginTime;
        gettimeofday(&loginTime, NULL);

        std::cout << "MDPTS_LOGIN_V2 " << std::endl;

        this->ProcessFactory::ProtocolLogin(protocol);
        MDPTS_LOGIN_RESPONSE_V2 response;
        char sessionOutSecurity[32];
        protocol->createSessionSecurityData(sessionOutSecurity);

        memcpy(response.sessionSecurity, sessionOutSecurity, sizeof(response.sessionSecurity));
        response.serverCount = 0;

        if (protocol->length() > 0) {
            MDPTS_LOGIN_SEND_V2 *requestData = (MDPTS_LOGIN_SEND_V2 *) protocol->data();
            std::cout << " ssid = " << requestData->ssid << std::endl;
            MHJSecurityManageDevice *databaseManage = (MHJSecurityManageDevice *) protocol->getDatabaseManage();
            std::tr1::shared_ptr<MHJDeviceBase> devicebase = databaseManage->getDatabaseModel(protocol->device());
            MHJDataBaseFactory *database = (MHJDataBaseFactory *) protocol->getDataBaseFactory();
            std::tr1::shared_ptr<MHJDeviceActivation> deviceactivation = database->selectDeviceMHJDeviceActivation(
                    devicebase->id_);
            if (deviceactivation.get()) {
                std::string ver = requestData->buildTime;
                if (deviceactivation->softVer != ver) {
                    deviceactivation->softVer = ver;
                    database->updateMHJDeviceActivationsoftVer(deviceactivation);
                }
            }
            //TODO 调度服务器暂不处理内网地址
            char tmp[255];
//			in_addr_t * ipaddress = (in_addr_t*) protocol->data();
            uint8_t *ip = (uint8_t *) &requestData->ip;
            sprintf(tmp, "设备登录客户端内网ip： %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

            std::cout << tmp << std::endl;

            sprintf(tmp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

            //将设备的信息写入redis中
            Json::Value root;
            Json::FastWriter writer;

            stringstream ss;
            for (int i = 0; i < 32; ++i) {
                ss << requestData->ssid[i];
            }
            string ssidStr = ss.str();

            root["deviceId"] = protocol->device()->deviceID;
            root["wifiSsid"] = ssidStr;
            root["wifiDelay"] = 0xffffffff;//初始默认
            root["loginTime"] = Json::Value((Json::Value::UInt64) (loginTime.tv_sec * 1000 + loginTime.tv_usec / 1000));
            string DeviceMessage = writer.write(root);

            char key[64];
            memset(key, 0, 64);
            sprintf(key, "DeviceMessage_%d_idmap", protocol->device()->deviceID);
            MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
            const char *buf = DeviceMessage.c_str();
            char *value = new char[strlen(buf) + 1];
            strcpy(value, buf);

            redisReply *reply = redisfactory->RedisCommand_SET(key, value);
            redisfactory->RedisCommand_FREE(reply);
            free(value);
            buf = NULL;

            MHJDBInterface dbpost;
            dbpost.postDevicelogin(protocol->device()->deviceID, protocol->device()->deviceType, protocol->protocolID(),
                                   tmp,
                                   (char *) this->request->getClientIP());

        } else {
            std::cout << "返回地址为空" << std::endl;
        }
//	shared_ptr<IPPORTADDRESS> ipport= DeviceServerManage::getAvailableServer();
        this->ProcessServerFactory::ProtocolLogin(protocol);
        ResponseProtocol(protocol->packID(), protocol->protocolID(), MDPTS_LOGIN_V2, sizeof(MDPTS_LOGIN_RESPONSE_V2),
                         &response);

        //将设备登陆信息发送到java服务器，告知该设备已登陆
        this->ProcessServerFactory::ProtocolTransferToJava(protocol);


    } else
        std::cout << "Switch Login:未知版本号 " << std::endl;
}

void ProcessSwitch::ProtocolHeartbeat(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_HEARTBEAT_V1 == protocol->Ver()) {
        if (protocol->length() > 0) {
            MDPTS_HEARTBEAT_SEND_V1 *request = protocol->data<MDPTS_HEARTBEAT_SEND_V1>();
            if (request) {
                std::cout << "Switch Heartbeat deviceID:" << protocol->device() << "TemperatureData:" <<
                request->TemperatureData << std::endl;

                char tmp[255];
                sprintf(tmp, "TemperatureData： %d", request->TemperatureData);
                MHJDBInterface dbpost;
                dbpost.postDeviceHeartbeat(protocol->device()->deviceID, protocol->device()->deviceType,
                                           protocol->protocolID(), tmp);
            } else {
                std::cout << "Switch Heartbeat deviceID:" << protocol->device()->deviceID << std::endl;
            }
        } else {
            std::cout << "Switch Heartbeat:，状态空" << std::endl;
        }
        //	shared_ptr<IPPORTADDRESS> ipport= DeviceServerManage::getAvailableServer();
        ResponseProtocol(protocol->packID(), protocol->protocolID(), MDPTS_HEARTBEAT_V1, NULL, NULL);
    } else if (protocol->Ver() == MDPTS_HEARTBEAT_V2) {
        std::cout << "Switch Heartbeat: 版本号为2" << std::endl;

        uint16_t lastTimeDurationMs = 0;

        if (protocol->length() > 0) {
            MDPTS_HEARTBEAT_SEND_V2 *request = protocol->data<MDPTS_HEARTBEAT_SEND_V2>();
            if (request) {
                std::cout << "Switch Heartbeat deviceID:" << protocol->device() << std::endl;

                lastTimeDurationMs = request->lastTimeDurationMs;

                char tmp[255];
                sprintf(tmp, "TemperatureData： %d", request->TemperatureData);
                MHJDBInterface dbpost;
                dbpost.postDeviceHeartbeat(protocol->device()->deviceID, protocol->device()->deviceType,
                                           protocol->protocolID(), tmp);
            } else {
                std::cout << "Switch Heartbeat deviceID:" << protocol->device()->deviceID << std::endl;
            }
        } else {
            std::cout << "Switch Heartbeat:，状态空" << std::endl;
        }

        MDPTS_HEARTBEAT_SEND_V2 data;
        data.lastTimeDurationMs = lastTimeDurationMs;


        char key[64];
        memset(key, 0, 64);
        sprintf(key, "DeviceMessage_%d_idmap", protocol->device()->deviceID);
        MHJRedisBaseFactory *redisfactory = MHJRedisBaseFactory::GetRedisFactory();
        redisReply *reply = redisfactory->RedisCommand_GET(key);
        cout << "replay: " << reply->str << endl;

        Json::Value old;
        Json::Value root;
        Json::Reader reader;
        Json::FastWriter writer;


        reader.parse(reply->str, old);
        std::cout << "wifiSsid: " << old["wifiSsid"].asString() << "deviceId " << old["deviceId"].asInt64()
        << "wifiDelay" << old["wifiDelay"].asInt64() << "loginTime " << old["loginTime"].asInt64() << std::endl;

        root["deviceId"] = old["deviceId"].asInt64();
        root["wifiSsid"] = old["wifiSsid"].asString();
        root["wifiDelay"] = lastTimeDurationMs;
        root["loginTime"] = old["loginTime"].asInt64();
        string DeviceMessage = writer.write(root);


        const char *buf = DeviceMessage.c_str();
        char *value = new char[strlen(buf) + 1];
        strcpy(value, buf);


        reply = redisfactory->RedisCommand_SET(key, value);
        redisfactory->RedisCommand_FREE(reply);
        free(value);
        buf = NULL;


        ResponseProtocol(protocol->packID(), protocol->protocolID(), MDPTS_HEARTBEAT_V2,
                         sizeof(MDPTS_HEARTBEAT_SEND_V2), (void *) &data);


    } else
        std::cout << "Switch Heartbeat:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSwitchStateSet(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_SWITCH_STATE_SET_V1 == protocol->Ver()) {
        MDPTS_SWITCH_STATE_SET_SEND_V1 *requestData = (MDPTS_SWITCH_STATE_SET_SEND_V1 *) protocol->data();
        if (requestData->keyID) { //已知KEYid，应答服务器的请求
            std::tr1::shared_ptr<MHJDeviceChild> devicechild = mDeviceManage.getDeviceChild(requestData->keyID);
            if (devicechild.get()) {
                if (requestData->newState < 0x100) {
                    devicechild->state = requestData->newState;
                    MHJDataBaseFactory *database = (MHJDataBaseFactory *) protocol->getDataBaseFactory();
                    database->updateDeviceChildState(devicechild);
//					mDeviceManage.updateDeviceChild(devicechild);
                }

                ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
                protocoltoapp->sendSetSwitchResponse(&requestData->sendMark, requestData->keyID, requestData->newState);
                releaseTransformation(protocoltoapp);
            } else {
                std::cout << "**ProtocolSwitchStateSet:找不到开关！ keyid:" << requestData->keyID << std::endl;
            }
        } else {
            //找到这个设备所有有关系的用户，发送开关状态改变事件。
            MHJSecurityManageDevice *databaseManage = (MHJSecurityManageDevice *) protocol->getDatabaseManage();
            std::tr1::shared_ptr<MHJDeviceBase> devicebase = databaseManage->getDatabaseModel(protocol->device());
            MHJDataBaseServer *database = (MHJDataBaseServer *) protocol->getDataBaseFactory();
            std::tr1::shared_ptr<MHJDeviceChild> devicechild = database->selectDeviceChildForDB(devicebase->id_,
                                                                                                requestData->switchNumber);
            if (devicechild.get() == NULL) {
                std::cout << "**ProtocolSwitchStateSet:找不到开关！ keyid:" << requestData->keyID << " switchNumber:" <<
                requestData->switchNumber << std::endl;
                return;
            }
            if (requestData->newState < 0x100) {
                devicechild->state = requestData->newState;
                MHJDataBaseFactory *database = (MHJDataBaseFactory *) protocol->getDataBaseFactory();
                database->updateDeviceChildState(devicechild);
//				mDeviceManage.updateDeviceChild(devicechild);
            }
            requestData->keyID = devicechild->id_;

            std::tr1::shared_ptr<MHJDeviceActivation> deviceActivation = database->selectDeviceMHJDeviceActivation(
                    devicebase->id_);
            if (deviceActivation.get()) {
                std::tr1::shared_ptr<MHJUserEntity> userEntity = database->selectUserEntity(deviceActivation->userID);
                MHJDeviceMark userMark;
                userMark.deviceID = userEntity->id;
                userMark.idSecurity1 = userEntity->tokenReplaceTime & 0xff;
                userMark.idSecurity2 = (userEntity->tokenReplaceTime >> 8) & 0xff;
                userMark.deviceType = MDT_APP;
                ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
                protocoltoapp->sendSetSwitchResponse(&userMark, requestData->keyID, requestData->newState);
                releaseTransformation(protocoltoapp);
            }

            std::vector<unsigned long> userIdVector;
            {
                std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> > vector;
                std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> >::iterator it;
                database->selectDeviceChildMHJDeviceAuthorize(devicechild->id_, &vector);

                for (it = vector.begin(); it != vector.end(); it++) {
                    userIdVector.push_back(it->get()->giveUserID);
                }
            }
            {
                std::vector<std::tr1::shared_ptr<MHJUserEntity> > vector;
                std::vector<std::tr1::shared_ptr<MHJUserEntity> >::iterator it;
                database->selectUserSecurityByIDList(userIdVector, &vector);

                ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
                for (it = vector.begin(); it != vector.end(); it++) {
                    std::tr1::shared_ptr<MHJUserEntity> userEntity = *it;
                    MHJDeviceMark userMark;
                    userMark.deviceID = userEntity->id;
                    userMark.idSecurity1 = userEntity->tokenReplaceTime & 0xff;
                    userMark.idSecurity2 = (userEntity->tokenReplaceTime >> 8) & 0xff;
                    userMark.deviceType = MDT_APP;
                    protocoltoapp->sendSetSwitchResponse(&userMark, requestData->keyID, requestData->newState);
                }
                releaseTransformation(protocoltoapp);

            }

        }
    } else
        std::cout << " Switch,ProtocolSwitchStateSet:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSystemUpdate(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_SYSTEM_UPDATE_V1 == protocol->Ver()) {
        MDPTS_SYSTEM_UPDATE_SEND_V1 *request = (MDPTS_SYSTEM_UPDATE_SEND_V1 *) protocol->data();
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
        protocoltoapp->sendSwitchSystemUpdate(&request->sendMark, request->isStart);
        releaseTransformation(protocoltoapp);
    } else
        std::cout << "Switch, ProtocolSystemUpdate:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSystemUpdateDownloadComplete(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_V1 == protocol->Ver()) {
        MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_SEND_V1 *request = (MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_SEND_V1 *) protocol->data();
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
        protocoltoapp->sendSwitchSystemUpdateDownloadComplete(&request->sendMark, request->isComplete);
        releaseTransformation(protocoltoapp);
    } else
        std::cout << "Switch, ProtocolSystemUpdateDownloadComplete:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSystemUpdateRestart(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_SYSTEM_UPDATE_RESTART_V1 == protocol->Ver()) {
        MDPTS_SYSTEM_UPDATE_RESTART_SEND_V1 *request = (MDPTS_SYSTEM_UPDATE_RESTART_SEND_V1 *) protocol->data();
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
        protocoltoapp->sendSwitchSystemUpdateRestart(&request->sendMark, request->isSet);
        releaseTransformation(protocoltoapp);
        protocol->setReturnHandleClose();
    } else
        std::cout << "Switch,ProtocolSystemUpdateRestart:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolInfoSync(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_INFO_SYNC_V1 == protocol->Ver()) {
        MDPTS_INFO_SYNC_RESPONSE_V1 response;

        MHJSecurityManageDevice *databaseManage = (MHJSecurityManageDevice *) protocol->getDatabaseManage();
        std::tr1::shared_ptr<MHJDeviceBase> devicebase = databaseManage->getDatabaseModel(protocol->device());
        MHJDataBaseServer *database = (MHJDataBaseServer *) protocol->getDataBaseFactory();
        //TODO 直接访问数据库，未做缓存优化。
        std::tr1::shared_ptr<MHJDeviceActivation> deviceActivation = database->selectDeviceMHJDeviceActivation(
                devicebase->id_);
        if (deviceActivation.get()) {
            std::tr1::shared_ptr<MHJUserEntity> userEntity = database->selectUserEntity(deviceActivation->userID);
            memset(response.tiedUserTitle, 0, sizeof(response.tiedUserTitle));
            memcpy(response.tiedUserTitle, userEntity->UserTitle.data(),
                   userEntity->UserTitle.size() > sizeof(response.tiedUserTitle) ? sizeof(response.tiedUserTitle) :
                   userEntity->UserTitle.size() + 1);
            tm tm1 = to_tm(deviceActivation->Date);
            time_t tt = mktime(&tm1);
            response.tiedTime = (uint32_t) tt;
        } else {
            memset(response.tiedUserTitle, 0, sizeof(response.tiedUserTitle));
            response.tiedTime = 0;
        }
        time_t now = time(NULL);
        response.serverTime = now;
        ResponseProtocol(&response);
    } else
        std::cout << "Switch,ProtocolInfoSync:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolUpdateSetup(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_INFO_SYNC_V1 == protocol->Ver()) {
        MDPTS_UPDATE_SETUP_SEND_V1 *request = (MDPTS_UPDATE_SETUP_SEND_V1 *) protocol->data();
        if (request->isSame) {
            std::cout << "Switch,ProtocolUpdateSetup:版本一致！" << std::endl;
        } else
            std::cout << "Switch,ProtocolUpdateSetup:版本不一致！" << std::endl;
    } else
        std::cout << "Switch,ProtocolUpdateSetup:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolVirtualKeyDefineSync(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_VIRTUAL_KEY_DEFINE_SYNC_V1 == protocol->Ver()) {
        MDPTS_VIRTUAL_KEY_DEFINE_SYNC_SEND_V1 *request = (MDPTS_VIRTUAL_KEY_DEFINE_SYNC_SEND_V1 *) protocol->data();
        MDPTS_VIRTUAL_KEY_DEFINE_SYNC_RESPONSE_V1 response;
        MHJDataBaseServer *database = (MHJDataBaseServer *) protocol->getDataBaseFactory();

        int index = request->switchNumber - 1;

        MHJSecurityManageDevice *databaseManage = (MHJSecurityManageDevice *) protocol->getDatabaseManage();
        std::tr1::shared_ptr<MHJDeviceBase> devicebase = databaseManage->getDatabaseModel(protocol->device());

        uint32_t updatetime[10];
        database->selectDeviceChildDefineUpdateTime(devicebase, updatetime);
        response.VirtualKeyUpdateTime = updatetime[index];

        if (request->count == 0)
            database->RemoveDeviceChildDefineListCache(devicebase->id_, request->switchNumber);
        response.count = database->selectDeviceChildDefineListCount(devicebase->id_, request->switchNumber);
        response.switchNumber = request->switchNumber;
        response.index = request->index;
        if (response.count > 0 && response.index < response.count) {
            MHJDeviceVirtualDefine *pvirtualDefine = database->selectDeviceChildDefine(devicebase->id_,
                                                                                       request->switchNumber,
                                                                                       request->index);
            if (pvirtualDefine == NULL) {
                std::cout << "ProtocolUpdateSetup:pvirtualDefine 为空！" << std::endl;
                return;
            }
            std::tr1::shared_ptr<MHJDeviceChild> targetchild = database->selectDeviceChild(pvirtualDefine->targetKeyID);
            if (targetchild.get() == NULL) {
                std::cout << "ProtocolUpdateSetup:targetchild 为空！" << std::endl;
                return;
            }
            std::tr1::shared_ptr<MHJDeviceBase> targetdevice = database->selectDeviceByDBID(targetchild->deviceID);
            if (targetdevice.get() == NULL) {
                std::cout << "ProtocolUpdateSetup:targetdevice 为空！" << std::endl;
                return;
            }
            memcpy(response.tragetDevkey, targetdevice->security, SECURITYSIZE);
            MHJDeviceMark targetMark;
            targetdevice->getMark(&targetMark);
            memcpy(response.tragetDevID, &targetMark, sizeof(MHJDeviceMark));

            //开关和蓝牙，这里传输的结构体定义有所区别。
            if (targetdevice->deviceType == MDT_BluetoothDOOR) {
                MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE_V1 *bluetoothDoorResponse =
                        (MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE_V1 *) &response;

                char tmpchar[8];
                memset(tmpchar, 0, sizeof(tmpchar));
                uint8_t tmp[8];
                unsigned int macbit;
                for (int i = 0; i < 6; i++) {
                    memcpy(tmpchar, &targetdevice->virtualKeyDefine[i * 2], 2);
                    sscanf(tmpchar, "%x", &macbit);
                    tmp[i] = macbit;
                }

                memcpy(bluetoothDoorResponse->BluetoothDoorMac, tmp, sizeof(bluetoothDoorResponse->BluetoothDoorMac));
            } else
//				if (targetdevice->deviceType == MDT_SWITCH || targetdevice->deviceType == MDT_CURTAIN)
            {
                response.tragetNewState = pvirtualDefine->targetNewState;
                response.tragetSwitchNumber = targetchild->keyID;
            }
        } else {

        }
        ResponseProtocol(&response);

    } else
        std::cout << "Switch,ProtocolVirtualKeyDefineSync:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolUpinlessSetup(boost::shared_ptr<MHJProtocol> protocol) {
    printf("\r\n 来自switch的解绑应答 \r\n");
    if (MDPTS_UPINLESS_SETUP_V1 == protocol->Ver()) {
        MDPTS_UPINLESS_SETUP_SEND_V1 *request = (MDPTS_UPINLESS_SETUP_SEND_V1 *) protocol->data();
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
        protocoltoapp->sendSwitchUpinlessResponse(&request->sendMark, request->isOk);
        releaseTransformation(protocoltoapp);

    } else
        std::cout << "Switch,ProtocolUpinlessSetup:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSoftRestart(boost::shared_ptr<MHJProtocol> protocol) {
//	printf("\r\n 来自switch的软重启应答 \r\n");
    if (MDPTS_SOFTRESTART_V1 == protocol->Ver()) {
        MDPTS_SOFTRESTART_SEND_V1 *request = (MDPTS_SOFTRESTART_SEND_V1 *) protocol->data();
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);
        protocoltoapp->sendSwitchSoftRestart(&request->sendMark, request->isOk);
        releaseTransformation(protocoltoapp);

    } else
        std::cout << "Switch,ProtocolSoftRestart:未知版本！" << std::endl;
}

void ProcessSwitch::ProtocolSetSwitchRFIRLearn(boost::shared_ptr<MHJProtocol> protocol) {

    if (MDPTS_RFIRLEARN_V1 == protocol->Ver()) {
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);

        MDPTS_RFIRLEARN_RESPONSE_RECEIVED_V1 *request = (MDPTS_RFIRLEARN_RESPONSE_RECEIVED_V1 *) protocol->data();
        MHJDeviceMark destDevice = request->deviceMark;  //保存目标ID
        memcpy(&request->deviceMark, protocol->device(), sizeof(MHJDeviceMark));  //数据区ID 改为设备ID
        if (request->type == 0 || request->type == 1) { //0表示返回的是学习命令已接收，1表示返回的是学习命令结果
            protocoltoapp->sendSwitchRFIRLearn(&destDevice, request, request->type);
        } else {
            printf("\n switch RFIR的学习命令返回未知版本，不做处理\n");
        }

        releaseTransformation(protocoltoapp);
    } else
        std::cout << "ProtocolSetSwitchRFIRLearn:未知版本！" << std::endl;

}

void ProcessSwitch::ProtocolSetSwitchRFIRTransmit(boost::shared_ptr<MHJProtocol> protocol) {
    if (MDPTS_RFIRTRANSMIT_V1 == protocol->Ver()) {
        ProtocolToApp *protocoltoapp = getTransformation<ProtocolToApp>(protocol);

        MDPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1 *request = (MDPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1 *) protocol->data();
        MHJDeviceMark destDevice = request->deviceMark;  //保存目标ID
        memcpy(&request->deviceMark, protocol->device(), sizeof(MHJDeviceMark));  //数据区ID 改为设备ID
        protocoltoapp->sendSwitchRFIRTransmit(&destDevice, request, sizeof(MDPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1));
        releaseTransformation(protocoltoapp);
    } else
        std::cout << "ProtocolSetSwitchRFIRTransmit:未知版本！" << std::endl;
}
