/*
 * ProcessBase.cpp
 *
 *  Created on: 2015-7-5
 *      Author: waroy
 */

#include "ProcessFactory.h"
//#include "ProcessSwitch.h"
#include "stddef.h"
#include "MHJProtocol.h"
#include "MHJSecurityFactory.h"

//#include "ProcessHost.h"

SP_OnlineManager* ProcessFactory::mOnlineManager = NULL;

SP_Server * ProcessFactory::pspserver = NULL;

ProcessFactory::ProcessFactory(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType classProtocolType) {
	this->classProtocolType = classProtocolType;
	this->protocol = protocol;
	this->request = request;
	this->response = response;
}

ProcessFactory::~ProcessFactory() {

}

void ProcessFactory::ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol) {
	SP_ArrayList sidlist;
	mOnlineManager->getSidList(protocol->device(), &sidlist);
	SP_Sid_t newsid = this->response->getFromSid();
	for (int i = 0; i < sidlist.getCount(); i++) {
		SP_Sid_t * sid = (SP_Sid_t*) sidlist.getItem(i);
		if (sid->mKey != newsid.mKey && sid->mSeq != newsid.mSeq) {
			//断开重复连接
			response->getToCloseList()->add(*sid);
		}
	}

	SP_OnlineInfo_t*clientinfo = mOnlineManager->getClientInfo(newsid);
	clientinfo->mClientMark = *protocol->device();
}

ProcessFactory *ProcessFactory::getProcess(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol) {
	//	ProcessFactory *process=NULL;
	//	switch (protocol->deviceType()) {
	//		case MDT_SWITCH :
	//			process=new ProcessSwitch(request, response, protocol);
	//			break;
	//		case MDT_HOST:
	//			process=new ProcessHost(request,response,protocol);
	//			break;
	//		default:
	//			break;
	//	}
	//
	//	return process;
	return NULL;
}

void ProcessFactory::initProcess() {
}

void ProcessFactory::Process() {
}

void ProcessFactory::Release(void* ptr) {
	delete this;
}

DWORD ProcessFactory::getProtocolType() {
	return classProtocolType;
}

void ProcessFactory::setOnlimeManager(SP_OnlineManager *onlineManager) {
	mOnlineManager = onlineManager;

}

SP_OnlineManager* ProcessFactory::getOnlimeManager() {
	return mOnlineManager;
}

/*
 * 响应协议
 */bool ProcessFactory::ResponseProtocol(UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata) {
	boost::shared_ptr < SP_Buffer > outBuffer = protocol->getSendBuffer(classProtocolType, protocol->device(), mpackID, mprotocolID, mver, mlength, pdata);
	response->getReply()->getMsg()->append(outBuffer->getBuffer(), outBuffer->getSize());
	return true;
}

bool ProcessFactory::ResponseProtocol(WORD mlength, void* pdata) {
	return ResponseProtocol(protocol->packID(), protocol->protocolID(), protocol->Ver(), mlength, pdata);
}

void ProcessFactory::clientClose(SP_Sid_t sid) {
	//TODO 如果是设备断开，需要处理延时，经过一段时间还没有上线，则通知用户
}

void ProcessFactory::setSecurity(MHJSecurityFactoryServer* security) {
	this->mSecurity = security;

}

bool ProcessFactory::isLogin(const MHJDeviceMark* pdevice, BYTE protocolID) {
	//所有类型的协议类型为1时均为登录协议。
//	if (pdevice->deviceType == MDT_SWITCH || pdevice->deviceType == MDT_CURTAIN || pdevice->deviceType == MDT_HOST || pdevice->deviceType == MDT_SERVER
//			|| pdevice->deviceType == MDT_ADMIN || pdevice->deviceType == MDT_APP)
	if (protocolID == 1)
		return true;
	else
		return false;
//	else
//		return false;
}

ProcessTransformationFactory *ProcessFactory::getTransformationFactory(ProcessTransformationFactory *transformation) {
	transformation->setOnlimeManager(mOnlineManager);
//	transformation->setSecurity(mSecurity);
	return transformation;
}

void ProcessFactory::releaseTransformation(ProcessTransformationFactory* transformation) {
	delete transformation;
}

