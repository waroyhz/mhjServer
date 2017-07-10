/*
 * ProcessAPP.h
 *
 *  Created on: Aug 4, 2015
 *      Author: ruancongming
 */

#ifndef ProcessAPP_H_
#define ProcessAPP_H_

#include "ProcessServerFactory.h"
#include "sprequest.hpp"
#include "spresponse.hpp"

#define COMMAND_SIZE 32

class ProcessAPP: public ProcessServerFactory {
public:
	ProcessAPP(SP_Request *request, SP_Response *response, shared_ptr<MHJProtocol> protocol);
	virtual ~ProcessAPP();

	virtual void Process();

//	virtual void Release();

	void ProtocolLogin(shared_ptr<MHJProtocol> protocol);
	void ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchState(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchSystemUpdate(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchSystemUpdateRestart(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchUpdateSetup(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchUpinlessSetup(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchSoftRestart(shared_ptr<MHJProtocol> protocol);

	void ProtocolSetSwitchRFIRLearn(shared_ptr<MHJProtocol> protocol);
	void ProtocolSetSwitchRFIRTransmit(shared_ptr<MHJProtocol> protocol);
};

#endif
