/*
 * MHJRedisServer.h
 *
 *  Created on: 2015年11月9日
 *      Author: ruancongming
 */

#ifndef DEVICEPROCESS_MHJREDISSERVER_H_
#define DEVICEPROCESS_MHJREDISSERVER_H_
#include "MHJRedisBaseFactory.h"
#include <iostream>
#include "MHJRedisSubscribeFactory.h"

using namespace std;

class MHJRedisServer {
public:
	MHJRedisServer(char* serviceId, char* ipdefault, int portdefault);
	~MHJRedisServer();
	void Start();
	void HeartBeat2Redis();
	void PublishServerInfo();
	void PublishHeartBeat();
	void PublishLogIn();

private:
	pthread_t mHeartbeatid;
	//服务号， 定阅号， 默认IP， 默认端口
	char* serviceID;
	char* channel;
	char* ipdef;
	int portdef;
};

#endif /* DEVICEPROCESS_MHJREDISSERVER_H_ */
