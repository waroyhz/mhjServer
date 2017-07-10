/*
 * ProcessSwitch.h
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#ifndef PROCESSSWITCH_H_
#define PROCESSSWITCH_H_

#include "ProcessServerFactory.h"
#include "sprequest.hpp"
#include "spresponse.hpp"

class ProcessSwitch: public ProcessServerFactory {
public:
	ProcessSwitch(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol);
	virtual ~ProcessSwitch();

	virtual void Process();

	virtual void Release();

	void ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolHeartbeat(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSwitchStateSet(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSystemUpdate(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSystemUpdateDownloadComplete(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSystemUpdateRestart(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolInfoSync(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolUpdateSetup(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolVirtualKeyDefineSync(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolUpinlessSetup(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSoftRestart(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchRFIRLearn(boost::shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchRFIRTransmit(boost::shared_ptr<MHJProtocol> protocol);

};

#endif /* PROCESSSWITCH_H_ */
