/*
 * ProcessAPP.cpp
 *
 *  Created on: Aug 4, 2015
 *      Author: ruancongming
 */

#include "ProcessAPP.h"
#include "MHJProtocol.h"
#include "ProtocolApp.h"
#include "ProtocolToSwitch.h"
#include <stdio.h>

#define cout std::cout
#define endl std::endl

ProcessAPP::ProcessAPP(SP_Request *request, SP_Response *response, shared_ptr<MHJProtocol> protocol) :
		ProcessServerFactory(request, response, protocol, MPT_APP) {

}

ProcessAPP::~ProcessAPP() {

}

void ProcessAPP::Process() {
#ifdef DEBUG
	printf("%s,%s:%s,%d\n",__DATE__,__TIME__,__FILE__,__LINE__);
	printf("ProcessAPP::Process, protocol->protocolID = %d \r\n",protocol->protocolID());
#endif
	switch (protocol->protocolID()) {
	/*
	 * 登录
	 * 数据区：空
	 * 服务器返回：协议号相同;数据区：返回空；如果服务器不认可，将关闭端口。
	 */
	case MAPT_LOGIN:
		ProtocolLogin (protocol);
		break;
	case MAPT_HEARTBEAT:
		ProtocolHeartbeat(protocol);
		break;
	case MAPT_TO_SWITCH_STATE_SET:
		ProtocolSetSwitchState(protocol);
		break;
	case MAPT_TO_SWITCH_SYSTEM_UPDATE:
		ProtocolSetSwitchSystemUpdate(protocol);
		break;
	case MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART:
		ProtocolSetSwitchSystemUpdateRestart(protocol);
		break;
	case MAPT_TO_SWITCH_UPDATE_SETUP:
		ProtocolSetSwitchUpdateSetup(protocol);
		break;
	case MAPT_TO_SWITCH_UPINLESS_SETUP:
		ProtocolSetSwitchUpinlessSetup(protocol);
		break;
	case MAPT_TO_SWITCH_SOFTRESTART:
//		ProtocolSetSwitchSoftRestart(protocol);
		this->ProcessServerFactory::ProtocolTransferToJava(protocol); //just a test test for transfer
		break;
//	case MAPT_TO_SWITCH_RFIRLEARN:
//		ProtocolSetSwitchRFIRLearn(protocol);
//		break;
//	case MAPT_TO_SWITCH_RFIRTRANSMIT:
//		ProtocolSetSwitchRFIRTransmit(protocol);
//		break;
//				case MAPT_ADMIN_COMMAND:
//					ProtocolPhraseCommand(protocol);
//					break;
	default:
		//其他业务处理，转到java服务器
		this->ProcessServerFactory::ProtocolTransferToJava(protocol);
		break;
	}

}

//void ProcessAPP::Release() {
//}

void ProcessAPP::ProtocolLogin(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_LOGIN_V1 == protocol->Ver()) {
		this->ProcessFactory::ProtocolLogin(protocol);
		MAPT_LOGIN_RESPONSE_V1 responseData;
		char sessionOutSecurity[SECURITYSIZE];
		protocol->createSessionSecurityData(sessionOutSecurity);
		memcpy(responseData.sessionSecurity, sessionOutSecurity, sizeof(responseData.sessionSecurity));
		responseData.serverCount = 0;

		if (protocol->length() > 0) {
			//TODO 调度服务器暂不处理内网地址
			char tmp[255];
			in_addr_t * ipaddress = (in_addr_t*) protocol->data();
			sprintf(tmp, "客户端内网ip： %d.%d.%d.%d", ((BYTE*) ipaddress)[0], ((BYTE*) ipaddress)[1], ((BYTE*) ipaddress)[2], ((BYTE*) ipaddress)[3]);

			cout << tmp << endl;
		} else {
			cout << "返回地址为空" << endl;
		}

		this->ProcessServerFactory::ProtocolLogin(protocol);

		ResponseProtocol(protocol->packID(), protocol->protocolID(), MAPT_LOGIN_V1, &responseData);
	} else
		cout << "App Login:未知版本号 " << endl;
}

void ProcessAPP::ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_HEARTBEAT_V1 == protocol->Ver()) {
		cout << "App Heartbeat " << endl;
		ResponseProtocol(protocol->packID(), protocol->protocolID(), MAPT_HEARTBEAT_V1, NULL, NULL);
	} else
		cout << "App Heartbeat:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchState(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_TO_SWITCH_STATE_SET_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_STATE_SET_SEND_V1* requestData = protocol->data<MAPT_TO_SWITCH_STATE_SET_SEND_V1>();
		if (requestData) {
			ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
			MAPT_TO_SWITCH_STATE_SET_RESPONSE_V1 responseData;
			responseData.deviceMark = requestData->deviceMark;
			responseData.keyID = requestData->keyID;
			if (protocoltoswitch->sendSetSwitchState(&mDeviceManage, &requestData->deviceMark, requestData->keyID, requestData->newState)) {
				responseData.accept = true;
			} else {
				responseData.accept = false;
			}
			ResponseProtocol(protocol->packID(), protocol->protocolID(), MAPT_TO_SWITCH_STATE_SET_V1, &responseData);
			releaseTransformation(protocoltoswitch);
		} else
			cout << "App MAPT_TO_SWITCH_STATE_SET_SEND_V1:为空 " << endl;
	} else
		cout << "App ProtocolSetSwitchState:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchSystemUpdate(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_TO_SWITCH_SYSTEM_UPDATE_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_SYSTEM_UPDATE_SEND_V1* requestData = protocol->data<MAPT_TO_SWITCH_SYSTEM_UPDATE_SEND_V1>();
		if (requestData) {
			ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
			MAPT_TO_SWITCH_SYSTEM_UPDATE_RESPONSE_V1 responseData;
			responseData.deviceMark = requestData->deviceMark;
			if (protocoltoswitch->sendSwitchSystemUpdate(&requestData->deviceMark, requestData->ipaddress, requestData->uri)) {
				responseData.accept = true;
			} else {
				responseData.accept = false;
			}
			ResponseProtocol(protocol->packID(), protocol->protocolID(), MAPT_TO_SWITCH_STATE_SET_V1, &responseData);
			releaseTransformation(protocoltoswitch);
		} else
			cout << "App MAPT_TO_SWITCH_SYSTEM_UPDATE_SEND_V1:为空 " << endl;
	} else
		cout << "App ProtocolSetSwitchSystemUpdate:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchSystemUpdateRestart(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_SEND_V1* requestData = protocol->data<MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_SEND_V1>();
		if (requestData) {
			ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
			MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_V1 responseData;
			responseData.deviceMark = requestData->deviceMark;
			if (protocoltoswitch->sendSwitchSystemUpdateRestart(&requestData->deviceMark)) {
				responseData.accept = true;
			} else {
				responseData.accept = false;
			}
			ResponseProtocol(&responseData);
			releaseTransformation(protocoltoswitch);
		} else
			cout << "App MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_SEND_V1:为空 " << endl;
	} else
		cout << "App ProtocolSetSwitchSystemUpdateRestart:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchUpdateSetup(shared_ptr<MHJProtocol> protocol) {
	if (MAPT_TO_SWITCH_UPDATE_SETUP_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_UPDATE_SETUP_SEND_V1* requestData = protocol->data<MAPT_TO_SWITCH_UPDATE_SETUP_SEND_V1>();
		if (requestData) {
			ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
			MAPT_TO_SWITCH_UPDATE_SETUP_RESPONSE_V1 responseData;
			responseData.deviceMark = requestData->deviceMark;
			if (protocoltoswitch->sendSwitchUpdateSteup(&requestData->deviceMark)) {
				responseData.accept = true;
			} else {
				responseData.accept = false;
			}
			ResponseProtocol(&responseData);
			releaseTransformation(protocoltoswitch);
		} else
			cout << "App MAPT_TO_SWITCH_UPDATE_SETUP_SEND_V1:为空 " << endl;
	} else
		cout << "App ProtocolSetSwitchUpdateSetup:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchUpinlessSetup(shared_ptr<MHJProtocol> protocol){
	if (MAPT_TO_SWITCH_UPINLESS_SETUP_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_UPINLESS_SETUP_SEND_V1* requestData = (MAPT_TO_SWITCH_UPINLESS_SETUP_SEND_V1*) protocol->data();
		ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
		MAPT_TO_SWITCH_UPINLESS_SETUP_RESPONSE_V1 responseData;
		responseData.deviceMark = requestData->deviceMark;
		if (protocoltoswitch->sendSwitchUpinlessSteup(&requestData->deviceMark)) {
			responseData.accept = true;
		} else {
			responseData.accept = false;
		}
		ResponseProtocol(&responseData);
		releaseTransformation(protocoltoswitch);
	} else
		cout << "App ProtocolSetSwitchUpinlessSetup:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchSoftRestart(shared_ptr<MHJProtocol> protocol){
	if (MAPT_TO_SWITCH_SOFTRESTART_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_SOFTRESTART_SEND_V1* requestData = (MAPT_TO_SWITCH_SOFTRESTART_SEND_V1*) protocol->data();
		ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);
		MAPT_TO_SWITCH_SOFTRESTART_RESPONSE_V1 responseData;
		responseData.deviceMark = requestData->deviceMark;
		if (protocoltoswitch->sendSwitchSoftRestart(&requestData->deviceMark)) {
			responseData.accept = true;
		} else {
			responseData.accept = false;
		}
		ResponseProtocol(&responseData);
		releaseTransformation(protocoltoswitch);
	} else
		cout << "App ProtocolSetSwitchSoftRestart:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchRFIRLearn(shared_ptr<MHJProtocol> protocol){
	if (MAPT_TO_SWITCH_RFIRLEARN_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_RFIRLEARN_FORMAT* requestData = (MAPT_TO_SWITCH_RFIRLEARN_FORMAT*)protocol->data();
		MHJDeviceMark* sourceMark = (MHJDeviceMark*)protocol->device();
		ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);

		MAPT_TO_SWITCH_RFIRLEARN_RESPONSE_V1 responseData;
		responseData.deviceMark = requestData->deviceMark;
		if (protocoltoswitch->sendSwitchRFIRLEARN(&requestData->deviceMark,requestData,sizeof(MAPT_TO_SWITCH_RFIRLEARN_FORMAT),sourceMark)) {
			responseData.isOk = true;
			printf("ToSwitch:ProtocolSetSwitchRFIRLearn发送成功\n");
		} else {
			responseData.isOk = false;
			printf("ToSwitch:ProtocolSetSwitchRFIRLearn发送失败\n");
		}

		ResponseProtocol(&responseData);
		releaseTransformation(protocoltoswitch);
	} else
		cout << "App ProtocolSetSwitchRFIRProtocol:未知版本号 " << endl;
}

void ProcessAPP::ProtocolSetSwitchRFIRTransmit(shared_ptr<MHJProtocol> protocol){
	if (MAPT_TO_SWITCH_RFIRTRANSMIT_V1 == protocol->Ver()) {
		MAPT_TO_SWITCH_RFIRTRANSMIT_FORMAT* requestData = (MAPT_TO_SWITCH_RFIRTRANSMIT_FORMAT*)protocol->data();
		int length = sizeof(MAPT_TO_SWITCH_RFIRTRANSMIT_FORMAT)-sizeof(WORD)+requestData->length;
//#ifdef DEBUG
//		printf("dataType:%d, ver:%d, length:%d \n",requestData->DataType,requestData->ver,requestData->length);
//		WORD* ptrData = &requestData->data;
//		int i;
//		for(i=0;i<requestData->length/2;i++){
//			printf("%d: %d\n",i,*ptrData);
//			ptrData++;
//		}
//#endif
		MHJDeviceMark* sourceMark = protocol->device();
		ProtocolToSwitch* protocoltoswitch = this->ProcessFactory::getTransformation < ProtocolToSwitch > (protocol);

		MAPT_TO_SWITCH_RFIRTRANSMIT_RESPONSE_V1 responseData;
		responseData.deviceMark = requestData->deviceMark;
		if (protocoltoswitch->sendSwitchRFIRTRANSMIT(&requestData->deviceMark,requestData,length,sourceMark)) {
			responseData.isOk = true;
			printf("ToSwitch:ProtocolSetSwitchRFIRTransmit发送成功\n");
		} else {
			responseData.isOk = false;
			printf("ToSwitch:ProtocolSetSwitchRFIRTransmit发送失败\n");
		}
		ResponseProtocol(&responseData);
		//释放转发对象
		releaseTransformation(protocoltoswitch);
	} else
		cout << "App ProtocolSetSwitchRFIRProtocol:未知版本号 " << endl;
}
