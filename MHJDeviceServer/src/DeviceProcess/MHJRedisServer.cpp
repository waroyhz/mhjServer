/*
 * MHJRedisServer.cpp
 *
 *  Created on: 2015年11月9日
 *      Author: ruancongming
 */

#include "MHJRedisServer.h"
#include <iostream>
//#include "this->serviceID2KeyMap.h"
#include "ProtocolServer.h"
#include "Protocol.h"
#include "HostAddress.h"
#include <arpa/inet.h>

using namespace std;

void* RedisServerThreadHeartBeat(void *arg) {
	MHJRedisServer*redisServer = (MHJRedisServer*) arg;
	redisServer->HeartBeat2Redis();
	return 0;
}

#ifdef DEBUG
#include "spresponse.hpp"
#include "ProcessServerFactory.h"
#endif

void MHJRedisServer::HeartBeat2Redis() {
	while (1) {
		PublishServerInfo();
		sleep(1);
	}
}

MHJRedisServer::MHJRedisServer(char* serviceId, char* ipdefault, int portdefault) {

	this->serviceID = serviceId;
	this->ipdef = ipdefault;
	this->portdef = portdefault;
	this->channel = MHJRedisBaseFactory::GetRedisFactory()->getRedisPublisChannel();
}

MHJRedisServer::~MHJRedisServer() {
}

void MHJRedisServer::Start() {
	pthread_create(&mHeartbeatid, NULL, RedisServerThreadHeartBeat, (void *) this);
}

void MHJRedisServer::PublishServerInfo() {
	MSPT_SERVERINFO_REDIS_V1 senddata;

	char beatTime[11];
	sprintf(beatTime, "%d", time(NULL));
	memcpy(&senddata.beatTime, beatTime, sizeof(senddata.beatTime));

	int port = this->portdef;
	char* ipdef = this->ipdef;

	char ipaddress[PACKDATAMAXSIZE];
	int size = HostAddress::getAllAddress(ipaddress, PACKDATAMAXSIZE) / sizeof(in_addr_t);
	in_addr_t* pipaddress = (in_addr_t*) ipaddress;

	if (ipdef != NULL) {
		if (pipaddress[0] != inet_addr(ipdef)) {
			//TODO: 增加设置默认IP的优先算法， 当前指定默认IP放在IP列表的第一位
			//		in_addr_t temp = pipaddress[0];
			pipaddress[0] = inet_addr(ipdef);
		}
	}

	senddata.serverCount = size;
	for (int i = 0; i < size; i++) {
		senddata.ipPortAddress[i].ip = pipaddress[i];
		senddata.ipPortAddress[i].prot = port;
	}

#ifdef DEBUG
//	cout<<"服务号:" << this->serviceID<< " 发布信息，数据大小：" << sizeof(MSPT_SERVERINFO_REDIS_V1)<<endl;
//	cout<<" 心跳包时间 " << senddata.beatTime <<endl;
//	cout<<"可用IP列表："<<endl;
	for(int i=0; i<senddata.serverCount;i++) {
		in_addr* ptr = (in_addr*)(&senddata.ipPortAddress[i].ip);
//		cout<< "IP[" << i << "]:" << inet_ntoa(*ptr) <<"  Port:"<<senddata.ipPortAddress[i].prot << endl;
	}
#endif


	//将当前服务号保存到Redis 在线服务列表
	MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* ServerList = redisfactory->getOnlineServerList();
	if (!redisfactory->isKeyInList(ServerList, this->serviceID)) {
		redisReply* reply = redisfactory->RedisCommand_RPUSH(ServerList, this->serviceID);
		redisfactory->RedisCommand_FREE(reply);
	}

	MHJRedisSubscribeFactory::publish(MSPT_SERVERINFO, this->serviceID, NULL, sizeof(MSPT_SERVERINFO_REDIS_V1), &senddata, this->channel);
}

void MHJRedisServer::PublishLogIn() {
	//Notice: this should read form Config file, Here We just use as debug
	int port = this->portdef;
	char* ipdef = this->ipdef;

	char ipaddress[PACKDATAMAXSIZE];
	int size = HostAddress::getAllAddress(ipaddress, PACKDATAMAXSIZE) / sizeof(in_addr_t);
	in_addr_t* pipaddress = (in_addr_t*) ipaddress;

	if (ipdef != NULL) {
		if (pipaddress[0] != inet_addr(ipdef)) {
			//		in_addr_t temp = pipaddress[0];
			pipaddress[0] = inet_addr(ipdef);
		}
	}

	MSPT_SERVERINFO_REDIS_V1 senddata;
	senddata.serverCount = size;
	for (int i = 0; i < size; i++) {
		senddata.ipPortAddress[i].ip = pipaddress[i];
		senddata.ipPortAddress[i].prot = port;
	}

#ifdef DEBUG
//	cout<<"服务号:" << this->serviceID<< " 登陆，数据大小：" << sizeof(MSPT_SERVERINFO_REDIS_V1)<<endl;
//	cout<<"可用IP列表："<<endl;
//	for(int i=0; i<senddata.serverCount;i++) {
//		in_addr* ptr = (in_addr*)(&senddata.ipPortAddress[i].ip);
//		cout<< "IP[" << i << "]:" << inet_ntoa(*ptr) <<"  Port:"<<senddata.ipPortAddress[i].prot << endl;
//	}
#endif
	//将当前服务号保存到Redis 在线服务列表
	MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory();
	char* ServerList = redisfactory->getOnlineServerList();
	if (!redisfactory->isKeyInList(ServerList, this->serviceID)) {
		redisReply* reply = redisfactory->RedisCommand_RPUSH(ServerList, this->serviceID);
		redisfactory->RedisCommand_FREE(reply);
	}

	MHJRedisSubscribeFactory::publish(MSPT_LOGIN, this->serviceID, NULL, sizeof(MSPT_SERVERINFO_REDIS_V1), &senddata, this->channel);
}

void MHJRedisServer::PublishHeartBeat() {
	char beatTime[32];
	int size = sprintf(beatTime, "%d", time(NULL));
#ifdef DEBUG
//	cout<<"服务号:" << this->serviceID<< " 发布心跳包 " << beatTime <<"  大小 " << size<<endl;
#endif
	MHJRedisSubscribeFactory::publish(MSPT_HEARTBEAT, this->serviceID, NULL, size, beatTime, this->channel);
}

