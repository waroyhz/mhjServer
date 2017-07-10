/*
 * ProtocolToSwitch.h
 *
 *  Created on: 2015-8-20
 *      Author: waroy
 */

#ifndef PROTOCOLTOSWITCH_H_
#define PROTOCOLTOSWITCH_H_

//#include <ProcessTransformationFactory.h>
#include "MHJDeviceManage.h"
#include "ProcessTransformationRedis.h"
class ProtocolToSwitch: public ProcessTransformationRedis {
public:
	ProtocolToSwitch(SP_Response *response, shared_ptr<MHJProtocol> protocol);
	virtual ~ProtocolToSwitch();

	bool sendSetSwitchState(MHJDeviceManage* mMHJDeviceManage, MHJDeviceMark* destMark, uint64_t keyID, BYTE newState);
	bool sendSwitchSystemUpdate(MHJDeviceMark* destMark, IPPORTADDRESS ipaddress, char* uri);
	bool sendSwitchSystemUpdateRestart(MHJDeviceMark* destMark);
	bool sendSwitchUpdateSteup(MHJDeviceMark* destMark);
	bool sendSwitchUpinlessSteup(MHJDeviceMark* destMark);
	bool sendSwitchSoftRestart(MHJDeviceMark* destMark);

	bool sendSwitchRFIRLEARN(MHJDeviceMark* destMark,void* data, int size,MHJDeviceMark* sourcMark);
	bool sendSwitchRFIRTRANSMIT(MHJDeviceMark* destMark,void* data, int size,MHJDeviceMark* sourcMark);
private:

};

#endif /* PROTOCOLTOSWITCH_H_ */
