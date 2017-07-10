/*
 * ProtocolToSwitch.cpp
 *
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#include "ProtocolToSwitch.h"
#include "ProtocolDeviceSwitch.h"
#include <stdio.h>
#include "MHJDataBaseServer.h"
#include "MHJSecurityManageDevice.h"

ProtocolToSwitch::ProtocolToSwitch(SP_Response *response, shared_ptr<MHJProtocol> protocol) :
		ProcessTransformationRedis(response, protocol, MPT_SERVER) {

}

ProtocolToSwitch::~ProtocolToSwitch() {

}

bool ProtocolToSwitch::sendSetSwitchState(MHJDeviceManage* mDeviceManage, MHJDeviceMark *destMark, uint64_t keyID, BYTE newState) {
	std::tr1::shared_ptr < MHJDeviceChild > child = mDeviceManage->getDeviceChild(keyID);
	if (child.get()) {
		MDPTS_SWITCH_STATE_SET_RESPONSE_V1 sendData;
		sendData.sendMark = *sendMark;
		sendData.keyID = child->id_;
		sendData.switchNumber = child->keyID;
		sendData.newState = newState;
		if (sendProtocol(destMark, MDPTS_SWITCH_STATE_SET, MDPTS_SWITCH_STATE_SET_V1, &sendData)) {
//			printf("转发设备成功\n");
			return true;
		}
	}else{
		printf("****getDeviceChild NULL\n");
	}
	printf("****发送失败，ProtocolToSwitch::sendSetSwitchState\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchSystemUpdate(MHJDeviceMark* destMark, IPPORTADDRESS ipaddress, char* uri) {

	MDPTS_SYSTEM_UPDATE_RESPONSE_V1 sendData;
	sendData.sendMark = *sendMark;
	sendData.ipPortAddress = ipaddress;
	memcpy(&sendData.uri, uri, sizeof(sendData.uri));

	char tmp[32];
	sprintMark(tmp, sendMark);
	uint8_t *intip = (uint8_t*) &sendData.ipPortAddress.ip;

//	printf("发送升级请求： 设备 %s 升级地址：%d.%d.%d.%d  升级URI：%s\n", tmp, intip[0], intip[1], intip[2], intip[3], sendData.uri);
	if (sendProtocol(destMark, MDPTS_SYSTEM_UPDATE, MDPTS_SWITCH_STATE_SET_V1, &sendData)) {
//		printf("转发设备成功\n");
		return true;
	}

	printf("****发送失败，ProtocolToSwitch::sendSwitchSystemUpdate\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchSystemUpdateRestart(MHJDeviceMark* destMark) {
	MDPTS_SYSTEM_UPDATE_RESTART_RESPONSE_V1 sendData;
	sendData.sendMark = *sendMark;
	if (sendProtocol(destMark, MDPTS_SYSTEM_UPDATE_RESTART, MDPTS_SYSTEM_UPDATE_RESTART_V1, &sendData)) {
//		printf("转发设备成功\n");
		return true;
	}

	printf("****发送失败，ProtocolToSwitch::sendSwitchSystemUpdateRestart\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchUpdateSteup(MHJDeviceMark* destMark) {
	MDPTS_UPDATE_SETUP_RESPONSE_V1 sendData;
	sendData.sendMark = *sendMark;
	MHJSecurityManageDevice* databaseManage = (MHJSecurityManageDevice*) protocol->getDatabaseManage(destMark);
	std::tr1::shared_ptr < MHJDeviceBase > devicebase = databaseManage->getDatabaseModel(destMark);
	if (devicebase.get()) {
		MHJDataBaseServer* database = (MHJDataBaseServer*) protocol->getDataBaseFactory();
		database->selectDeviceChildDefineUpdateTime(devicebase, sendData.VirtualKeyUpdateTime);
		//	sendData.TimerUpdateTime = (uint32_t) now;
		if (sendProtocol(destMark, MDPTS_UPDATE_SETUP, MDPTS_SYSTEM_UPDATE_RESTART_V1, &sendData)) {
//			printf("转发设备成功\n");
			return true;
		}
	}
	printf("****发送失败，ProtocolToSwitch::sendSwitchUpdateSteup\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchUpinlessSteup(MHJDeviceMark* destMark) {
	MDPTS_UPINLESS_SETUP_RESPONSE_V1 sendData;
	sendData.sendMark = *sendMark;
	sendData.isOk = false;
	MHJSecurityManageDevice* databaseManage = (MHJSecurityManageDevice*) protocol->getDatabaseManage(destMark);
	std::tr1::shared_ptr < MHJDeviceBase > devicebase = databaseManage->getDatabaseModel(destMark);
	if (devicebase.get()) {
		//MHJDataBaseServer* database = (MHJDataBaseServer*) protocol->getDataBaseFactory();
		//database->selectDeviceChildDefineUpdateTime(devicebase->id_, sendData.VirtualKeyUpdateTime);
		//	sendData.TimerUpdateTime = (uint32_t) now;
		if (sendProtocol(destMark, MDPTS_UPINLESS_SETUP, MDPTS_UPINLESS_SETUP_V1, &sendData)) {
//			printf("sendSwitchUpinlessSteup:转发设备成功\n");
			return true;
		}
	}
	printf("****发送失败，ProtocolToSwitch::sendSwitchUpinlessSteup\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchSoftRestart(MHJDeviceMark* destMark) {
	MDPTS_SOFTRESTART_RESPONSE_V1 sendData;
	sendData.sendMark = *sendMark;
	sendData.isOk = false;
//	MHJSecurityManageDevice* databaseManage = (MHJSecurityManageDevice*) protocol->getDatabaseManage(destMark);
//	std::tr1::shared_ptr < MHJDeviceBase > devicebase = databaseManage->getDatabaseModel(destMark);
//	if (devicebase.get()) {
		//MHJDataBaseServer* database = (MHJDataBaseServer*) protocol->getDataBaseFactory();
		//database->selectDeviceChildDefineUpdateTime(devicebase->id_, sendData.VirtualKeyUpdateTime);
		//	sendData.TimerUpdateTime = (uint32_t) now;
		if (sendProtocol(destMark, MDPTS_SOFTRESTART, MDPTS_SOFTRESTART_V1, &sendData)) {
//			printf("sendSwitchSoftRestart:转发设备成功\n");
			return true;
		}
//	}
		printf("****整个发送失败，ProtocolToSwitch::sendSwitchSoftRestart\n");
	return false;
}

bool ProtocolToSwitch::sendSwitchRFIRLEARN(MHJDeviceMark* destMark,void* data,int size,MHJDeviceMark* sourcMark){
	MDPTS_RFIRLEARN_FORMAT sendData;
//	memcpy(&sendData.sourceMark,sourcMark,sizeof(MHJDeviceMark));
	memcpy(&sendData.deviceMark,data,size);
//	memcpy(&sendData.deviceMark,sourcMark,sizeof(MHJDeviceMark));
	sendData.deviceMark = *sourcMark;
	bool ret =this->ProcessTransformationFactory::sendProtocol(destMark,MDPTS_RFIRLEARN, MDPTS_RFIRLEARN_V1,sizeof(MDPTS_RFIRLEARN_FORMAT), &sendData);

	return ret;
}

bool ProtocolToSwitch::sendSwitchRFIRTRANSMIT(MHJDeviceMark* destMark,void* data,int size,MHJDeviceMark* sourcMark){
	int length = size+sizeof(MHJDeviceMark);
	char* buffer = new char[length];
	MDPTS_RFIRTRANSMIT_FORMAT* sendData = (MDPTS_RFIRTRANSMIT_FORMAT*)buffer;
//	memcpy(&sendData->sourceMark,sourcMark,sizeof(MHJDeviceMark));
	memcpy(&sendData->deviceMark,data,size);
//	memcpy(&sendData->deviceMark,sourcMark,sizeof(MHJDeviceMark));
	sendData->deviceMark = *sourcMark;
	bool ret =this->ProcessTransformationFactory::sendProtocol(destMark,MDPTS_RFIRTRANSMIT, MDPTS_RFIRTRANSMIT_V1,length, sendData);

	delete [](char*)buffer;
	return ret;
}
