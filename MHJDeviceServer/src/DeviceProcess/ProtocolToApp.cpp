/*
 * ProtocolToApp.cpp
 *
 *  Created on: 2015年8月22日
 *      Author: waroy
 */

#include <ProtocolToApp.h>
#include "ProtocolApp.h"
#include <stdio.h>

ProtocolToApp::ProtocolToApp(SP_Response *response, boost::shared_ptr<MHJProtocol> protocol) :
		ProcessTransformationRedis(response, protocol, MPT_SERVER) {

}

ProtocolToApp::~ProtocolToApp() {
}

bool ProtocolToApp::sendSetSwitchResponse(MHJDeviceMark* destMark, uint64_t keyID, BYTE newState) {
//TODO 开关状态发生改变，需要通知所有有关的终端。
	MAPT_SWITCH_STATE_SET_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.keyID = keyID;
	sendData.newState = newState;

	if (sendProtocol(destMark, MAPT_SWITCH_STATE_SET_RESPONSE, MAPT_SWITCH_STATE_SET_RESPONSE_V1, sizeof(MAPT_SWITCH_STATE_SET_RESPONSE_RESPONSE_V1),
			&sendData)) {
		printf("sendSetSwitchResponse转发app成功\n");
		return true;
	}
	printf("sendSetSwitchResponse转发app失败\n");
	return false;
}

bool ProtocolToApp::sendSwitchSystemUpdate(MHJDeviceMark* destMark, bool isStart) {
	MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.isStart = isStart;

	if (sendProtocol(destMark, MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE, MAPT_TO_SWITCH_SYSTEM_UPDATE_V1, sizeof(MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE_RESPONSE_V1),
			&sendData)) {
		printf("sendSwitchSystemUpdate 转发app成功\n");
		return true;
	}
	printf("sendSwitchSystemUpdate转发app失败\n");
	return false;
}

bool ProtocolToApp::sendSwitchSystemUpdateDownloadComplete(MHJDeviceMark* destMark, bool isComplete) {
	MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.isComplete = isComplete;

	if (sendProtocol(destMark, MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE, MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE_V1, &sendData)) {
		printf("sendSwitchSystemUpdateDownloadComplete 转发app成功\n");
		return true;
	}
	printf("sendSwitchSystemUpdateDownloadComplete 转发app失败\n");
	return false;
}

bool ProtocolToApp::sendSwitchSystemUpdateRestart(MHJDeviceMark* destMark, bool isSet) {
	MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.isSet = isSet;

	if (sendProtocol(destMark, MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE, MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_V1, &sendData)) {
		printf("sendSwitchSystemUpdateRestart 转发app成功\n");
		return true;
	}
	printf("sendSwitchSystemUpdateRestart 转发app失败\n");
	return false;
}

bool ProtocolToApp::sendSwitchUpinlessResponse(MHJDeviceMark* destMark, bool isOk){
	MAPT_SWITCH_UPINLESS_SETUP_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.isOk = isOk;

	if (sendProtocol(destMark, MAPT_SWITCH_UPINLESS_SETUP_RESPONSE, MAPT_SWITCH_UPINLESS_SETUP_RESPONSE_V1, &sendData)) {
		printf("sendSwitchUpinlessResponse 转发app成功\n");
		return true;
	}
	printf("sendSwitchUpinlessResponse 转发app失败\n");
	return false;

}

bool ProtocolToApp::sendSwitchSoftRestart(MHJDeviceMark* destMark, bool isOk){
	MAPT_SWITCH_SOFTRESTART_RESPONSE_RESPONSE_V1 sendData;
	sendData.deviceMark = *sendMark;
	sendData.isOk = isOk;

	if (sendProtocol(destMark, MAPT_SWITCH_SOFTRESTART_RESPONSE, MAPT_SWITCH_SOFTRESTART_RESPONSE_V1, &sendData)) {
		printf("sendSwitchSoftRestart 转发app成功\n");
		return true;
	}
	printf("sendSwitchSoftRestart 转发app失败\n");
	return false;
}

bool ProtocolToApp::sendSwitchRFIRTransmit(MHJDeviceMark* destMark,void* data, int size) {
	bool ret;
	MAPT_RFIRTRANSMIT_FINISHED_RESPONSE_V1 recived;
	memcpy(&recived,data,sizeof(MAPT_RFIRTRANSMIT_FINISHED_RESPONSE_V1));
	ret =this->ProcessTransformationFactory::sendProtocol(destMark,MAPT_TO_SWITCH_RFIRTRANSMIT, MAPT_TO_SWITCH_RFIRTRANSMIT_V1,sizeof(MAPT_RFIRTRANSMIT_FINISHED_RESPONSE_V1), data);
	return ret;
}

bool ProtocolToApp::sendSwitchRFIRLearn(MHJDeviceMark* destMark,void* data,int byte){
	bool ret;
	if(byte==0){
		MAPT_RFIRLEARN_RECEIVED_RESPONSE_V1 recived;
		memcpy(&recived,data,sizeof(MAPT_RFIRLEARN_RECEIVED_RESPONSE_V1));
		ret =this->ProcessTransformationFactory::sendProtocol(destMark,MAPT_TO_SWITCH_RFIRLEARN, MAPT_TO_SWITCH_RFIRLEARN_V1,sizeof(MAPT_RFIRLEARN_RECEIVED_RESPONSE_V1), data);
	}else if(byte==1){
		MAPT_RFIRLEARN_FINISHED_RESPONSE_V1* pdata = (MAPT_RFIRLEARN_FINISHED_RESPONSE_V1*)data;
		int length = pdata->length + sizeof(MAPT_RFIRLEARN_FINISHED_RESPONSE_V1);
//#ifdef DEBUG
//		printf("dataType:%d, ver:%d, length:%d \n",pdata->DataType,pdata->ver,pdata->length);
//		WORD* ptrData = &pdata->data;
//		int i;
//		for(i=0;i<pdata->length/2;i++){
//			printf("%d: %d\n",i,*ptrData);
//			ptrData++;
//		}
//#endif
		ret =this->ProcessTransformationFactory::sendProtocol(destMark,MAPT_TO_SWITCH_RFIRLEARN, MAPT_TO_SWITCH_RFIRLEARN_V1,length, data);
	}
	return ret;
}

