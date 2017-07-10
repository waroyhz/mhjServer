/*
 * HostClientProcess.h
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef SERVERCLIENTPROCESS_H_
#define SERVERCLIENTPROCESS_H_

#include "HostClientFactory.h"
#include "MHJProtocol.h"
#include <boost/shared_ptr.hpp>

using namespace boost;

class ServerClientProcess: public HostClientFactory {
public:
	ServerClientProcess(const char* ip, uint16_t port, MHJDeviceMark *device);
	virtual ~ServerClientProcess();

	virtual void sendLogin(MHJHostClient *client);
	virtual void disConnect(MHJHostClient *client);
	virtual void sendheartbeat(MHJHostClient *client);
	virtual void revice(SP_MsgDecoder* msgDecoder, MHJHostClient* client);

	void ProtocolLogin(shared_ptr<MHJProtocol> protocol);
	void ProtocolHeartbeat(shared_ptr<MHJProtocol> protocol);
};

#endif /* HOSTCLIENTPROCESS_H_ */
