/*
 * MHJDeviceServerConfig.h
 *
 *  Created on: 2016年5月27日
 *      Author: waroy
 */

#ifndef DEVICEPROCESS_MHJDEVICESERVERCONFIG_H_
#define DEVICEPROCESS_MHJDEVICESERVERCONFIG_H_

#include <MHJConfigFactory.h>

class MHJDeviceServerConfig: public MHJConfigFactory {
public:
	MHJDeviceServerConfig();
	virtual ~MHJDeviceServerConfig();

	void initParamsDBIinterface();

	char * getParamWebHost();
	int getParamWebPort();
	char * getParamWebURI();
	char * getParamWebKey();
	char* getParamWebToken();

	void initParamsServer();
	char * getParamIp();
	int getParamClientPort();
	int getParamListenPort();
	int getParamMaxConnection();
	int getParamReqQueuesize();
	char * getParamServiceNum();

	void initParamsRedisServer();
	char * getParamRedispasswd();
	char * getParamRedishost();
	int getParamRedisport();
	char * getParamRedisPublishChannel();
	char * getParamTimerecoderlist();
	char * getParamServerlist();
	char * getParamJavaBlpop();
	char * getParamLinuxBlpop();


	char *getParamReceiveDeviceInstructionList();
	char *getParamReceiveDeviceInstructionIsSuccessList();

	char * getParamDeviceMessageList();



};

#endif /* DEVICEPROCESS_MHJDEVICESERVERCONFIG_H_ */
