/*
 * ProcessSwitch.h
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#ifndef PROCESSSHOST_H_
#define PROCESSSHOST_H_

#include "ProcessServerFactory.h"
#include "sprequest.hpp"
#include "spresponse.hpp"

class ProcessHost: public ProcessServerFactory {
public:
	ProcessHost(SP_Request *request, SP_Response *response, shared_ptr<MHJProtocol> protocol);
	virtual ~ProcessHost();

	virtual void Process();

	virtual void Release();

	void ProtocolLogin(shared_ptr<MHJProtocol> protocol);
	void ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol);
	void ProtocolClientLogin(shared_ptr<MHJProtocol> protocol);
	void ProcessClientProtocol(shared_ptr<MHJProtocol> protocol);

	void ProtocolHostClientProtocolLogin(shared_ptr<MHJProtocol> clientprotocol);
};

#endif /* PROCESSSWITCH_H_ */
