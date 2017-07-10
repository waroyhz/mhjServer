//============================================================================
// Name        : MHJDeviceServer.cpp
// Author      : 设备管理服务端
// Version     :
// Copyright   : MHJ设备管理工程
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ServerProcessHandlerFactory.h"
#include "ProcessServerFactory.h"
#include "ServerClientProcess.h"
#include "MHJSecurityDeviceServer.h"
#include "MHJDataBaseServer.h"
#include "MHJDBInterface.h"
#include <MHJConfigFactory.h>
#include "MHJMqtt.h"

#include "MHJRedisServer.h"
#include "MHJRedisSubscribeProcessServer.h"
#include "MHJDeviceServerConfig.h"
#include "MHJIncludeInit.h"



using namespace std;
//#define SIZE 64
#define SERVERNUM_SIZE 32

const char* ConfigFile = "ServerConfig.ini";

int main() {
	/*
	 * 初始化DBI接口
	 */
	MHJDeviceServerConfig config;
	config.Setfile(ConfigFile);
	config.initParamsDBIinterface();

	cout << config.getParamWebHost() << " rt " << config.getParamWebPort();


	if (!MHJDBInterface::initDBInterface(config.getParamWebHost(), config.getParamWebPort(), config.getParamWebURI(), config.getParamWebKey(),
			config.getParamWebToken())) {
		printf("初始化数据接口失败！！！！\n");
		exit(0);
	}

	config.initParamsServer();

	MHJDeviceMark device;
//#ifdef DEBUG
	device.deviceID = 1;
	device.deviceType = MDT_SERVER;
	char mSecurityData[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
//#endif

	/*
	 * 初始化Redis服务信息
	 */
	config.initParamsRedisServer();

	MHJRedisBaseFactory *redisbasefactory = new MHJRedisBaseFactory(config.getParamRedishost(), config.getParamRedisport(), config.getParamRedispasswd());
	redisbasefactory->setRedisPublisChannel(config.getParamRedisPublishChannel());
	redisbasefactory->setTimeRecodlist(config.getParamTimerecoderlist());
	redisbasefactory->setServiceID(config.getParamServiceNum());
	redisbasefactory->setOnlineServerList(config.getParamServerlist());
	redisbasefactory->setJavaBlpop(config.getParamJavaBlpop());
	redisbasefactory->setLinuxBlpop(config.getParamLinuxBlpop());

	redisbasefactory->setReceiveDeviceInstructionIsSuccessList(config.getParamReceiveDeviceInstructionIsSuccessList());
	redisbasefactory->setReceiveDeviceInstructionList(config.getParamReceiveDeviceInstructionList());

	redisbasefactory->setDeviceMessageList(config.getParamDeviceMessageList());



	sp_openlog("设备服务器", LOG_CONS | LOG_PID | LOG_PERROR, LOG_USER);

	/** 条件为假，返回错误 **/assert(0 == sp_initsock());

	MHJDataBaseServer* mDataBaseFactoryServer = new MHJDataBaseServer(redisbasefactory);
	ProcessServerFactory* mProcessServerFactory = new ProcessServerFactory();
	MHJSecurityDeviceServer *mSecurityServer = new MHJSecurityDeviceServer;

	mSecurityServer->setFactory(mProcessServerFactory, mDataBaseFactoryServer);
	mProcessServerFactory->setSecurity(mSecurityServer);
	mProcessServerFactory->setDatabaseFactory(mDataBaseFactoryServer);

	SP_Server server("", config.getParamListenPort(), new MHJServerHandlerFactory(mProcessServerFactory, mSecurityServer));
	{ //初始化redis服务
		mProcessServerFactory->pspserver = &server;
		MHJRedisServer* redisServer = new MHJRedisServer(config.getParamServiceNum(), config.getParamIp(), config.getParamListenPort());
		redisServer->Start();
		MHJRedisSubscribeProcessServer* subsprocess = new MHJRedisSubscribeProcessServer(mSecurityServer, &server);
		MHJRedisSubscribeFactory *subs = new MHJRedisSubscribeFactory(config.getParamRedishost(), config.getParamRedisport(), config.getParamRedispasswd(),
				config.getParamServiceNum(), subsprocess);

		//mqtt服务
		mhj_mqtt_Client_init(&server,mSecurityServer);
	}
	server.setTimeout(80);
	server.setMaxConnections(config.getParamMaxConnection());
	//TODO 需要处理服务器忙消息
	server.setReqQueueSize(config.getParamReqQueuesize(), "Server busy!");
	server.runForever();

	//Notice：以下代码不会被执行， 添加仅仅为了编译通过
	mhj_includeInit();
}
