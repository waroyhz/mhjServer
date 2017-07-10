/*
 * MHJDeviceServercpp
 *
 *  Created on: 2016年5月27日
 *      Author: waroy
 */

#include "MHJDeviceServerConfig.h"
#include "stdio.h"

#define SIZE 64

char mWebHost[SIZE];
char mWebPort[SIZE];
char mWebURI[SIZE];
char mWebKey[SIZE];
char mWebToken[SIZE];

MHJDeviceServerConfig::MHJDeviceServerConfig() {
	// TODO Auto-generated constructor stub

}

MHJDeviceServerConfig::~MHJDeviceServerConfig() {
	// TODO Auto-generated destructor stub
}

void MHJDeviceServerConfig::initParamsDBIinterface() {
	SetSection("DBIinterface");
	ReadProfile("webHost", mWebHost, SIZE, "127.0.0.1");
	ReadProfile("Port", mWebPort, SIZE, "8080");
	ReadProfile("webURI", mWebURI, SIZE, "DBInterfaceV1");
	ReadProfile("key", mWebKey, SIZE, "aebe95a2b276c7757d5600c450deac18");
	ReadProfile("webToken", mWebToken, SIZE, "1713644b3b9caae10a3da1b2aa6a05f3");
}

char* MHJDeviceServerConfig::getParamWebHost() {

	return mWebHost;
}

int MHJDeviceServerConfig::getParamWebPort() {

	int port;
	sscanf(mWebPort, "%d", &port);
	return port;
}

char* MHJDeviceServerConfig::getParamWebURI() {
	return mWebURI;
}

char* MHJDeviceServerConfig::getParamWebKey() {
	return mWebKey;
}

char* MHJDeviceServerConfig::getParamWebToken() {
	return mWebToken;
}

/*
 * 初始化DeviceServer配置信息
 */
char mIp[SIZE];
char mClientPort[SIZE];
char mListenPort[SIZE];
char mMaxConnection[SIZE];
char mReqQueuesize[SIZE];
char mServiceNum[SIZE];

void MHJDeviceServerConfig::initParamsServer() {
	SetSection("Server");
	ReadProfile("ip", mIp, SIZE, "127.0.0.1");
	ReadProfile("clientport", mClientPort, SIZE, "21688");

	ReadProfile("listenport", mListenPort, SIZE, "31688");

	ReadProfile("maxconnection", mMaxConnection, SIZE, "10000");

	ReadProfile("reqqueuesize", mReqQueuesize, SIZE, "10000");

	ReadProfile("serverid", mServiceNum, SIZE, "0000000100020000");
}

char* MHJDeviceServerConfig::getParamIp() {
	return mIp;
}

int MHJDeviceServerConfig::getParamClientPort() {
	int clientport;
	sscanf(mClientPort, "%d", &clientport);
	return clientport;
}

int MHJDeviceServerConfig::getParamListenPort() {
	int listenport;
	sscanf(mListenPort, "%d", &listenport);
	return listenport;
}

int MHJDeviceServerConfig::getParamMaxConnection() {
	int maxconnection;
	sscanf(mMaxConnection, "%d", &maxconnection);
	return maxconnection;
}

int MHJDeviceServerConfig::getParamReqQueuesize() {
	int queuesize;
	sscanf(mReqQueuesize, "%d", &queuesize);
	return queuesize;
}

char* MHJDeviceServerConfig::getParamServiceNum() {
	return mServiceNum;
}

char redispasswd[SIZE];
char redishost[SIZE];
char redisport[SIZE];
char redisPublishChannel[SIZE];
char timerecoderlist[SIZE];
char serverlist[SIZE];
char javaBlpop[SIZE];
char linuxBlpop[SIZE];

char receiveDeviceInstructionIsSuccessList[SIZE];
char receiveDeviceInstructionList[SIZE];
char deviceMessageList[SIZE];


void MHJDeviceServerConfig::initParamsRedisServer() {
	SetSection("RedisServer");
	ReadProfile("RedisHost", redishost, SIZE, "192.168.5.11");
	ReadProfile("RedisPasswd", redispasswd, SIZE, "mhjtest123");

	ReadProfile("RedisPort", redisport, 16, "6379");

	ReadProfile("RedisPublishChannel", redisPublishChannel, SIZE, "redischannel");

	ReadProfile("RedisTimeRecoderList", timerecoderlist, SIZE, "TimeRecoderList");

	ReadProfile("ServerList", serverlist, SIZE, "ServerList");

	ReadProfile("JavaBlockPopList", javaBlpop, SIZE, "java_app_message_queue");

	ReadProfile("LinuxBlockPopList", linuxBlpop, SIZE, "linux_transfer_message_queue");

	ReadProfile("ReceiveDeviceInstructionIsSuccessList", receiveDeviceInstructionIsSuccessList, SIZE, "ReceiveDeviceInstructionIsSuccessList");

	ReadProfile("ReceiveDeviceInstructionList", receiveDeviceInstructionList, SIZE, "ReceiveDeviceInstructionList");

	ReadProfile("DeviceMessageList",deviceMessageList,SIZE,"DeviceMessageList");



}

char* MHJDeviceServerConfig::getParamRedispasswd() {
	return redispasswd;
}

char* MHJDeviceServerConfig::getParamRedishost() {
	return redishost;
}

int MHJDeviceServerConfig::getParamRedisport() {
	int mredisport;
	sscanf(redisport, "%d", &mredisport);
	return mredisport;
}

char* MHJDeviceServerConfig::getParamRedisPublishChannel() {
	return redisPublishChannel;
}


char* MHJDeviceServerConfig::getParamTimerecoderlist() {
	return timerecoderlist;
}

char* MHJDeviceServerConfig::getParamServerlist() {
	return serverlist;
}

char* MHJDeviceServerConfig::getParamJavaBlpop() {
	return javaBlpop;
}

char* MHJDeviceServerConfig::getParamLinuxBlpop() {
	return linuxBlpop;
}

char * MHJDeviceServerConfig::getParamReceiveDeviceInstructionList(){
	return  receiveDeviceInstructionList;
}
char * MHJDeviceServerConfig::getParamReceiveDeviceInstructionIsSuccessList(){
	return receiveDeviceInstructionIsSuccessList;
}

char * MHJDeviceServerConfig::getParamDeviceMessageList(){
	return deviceMessageList;
}