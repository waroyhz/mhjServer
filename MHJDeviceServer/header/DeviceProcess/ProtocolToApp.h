/*
 * ProtocolToApp.h
 *
 *  Created on: 2015年8月22日
 *      Author: waroy
 */

#ifndef DEVICEPROCESS_PROTOCOLTOAPP_H_
#define DEVICEPROCESS_PROTOCOLTOAPP_H_

//#include <ProcessTransformationFactory.h>
#include "ProcessTransformationRedis.h"
class ProtocolToApp: public ProcessTransformationRedis {
public:
	ProtocolToApp(SP_Response *response, boost::shared_ptr<MHJProtocol> protocol);
	virtual ~ProtocolToApp();

	bool sendSetSwitchResponse(MHJDeviceMark* destMark, uint64_t keyID, BYTE newState);
	bool sendSwitchSystemUpdate(MHJDeviceMark* destMark, bool isStart);
	bool sendSwitchSystemUpdateDownloadComplete(MHJDeviceMark* destMark, bool isComplete);
	bool sendSwitchSystemUpdateRestart(MHJDeviceMark* destMark, bool isSet);
	bool sendSwitchUpinlessResponse(MHJDeviceMark* destMark, bool isOk);
	bool sendSwitchSoftRestart(MHJDeviceMark* destMark, bool isOk);
	bool sendSwitchRFIRTransmit(MHJDeviceMark* destMark,void* data,int size);
	bool sendSwitchRFIRLearn(MHJDeviceMark* destMark,void* data,int byte);
};

#endif /* DEVICEPROCESS_PROTOCOLTOAPP_H_ */
