/*
 * ProtocolAdmin.h
 *	Administrator的协议。
 *  Created on: Aug 3, 2015
 *      Author: ruancongming
 */

#ifndef PROTOCOLADMIN_H_
#define PROTOCOLADMIN_H_

#include "ProtocolTypeDefine.h"
/*
 * 协议类型
 */
enum _MHJADMINProtocolType{
	/*
	 * 通用错误返回。
	 */
	MAPT_ERROR_RESPONSE,
	/*
	 * 登录
	 * 数据区：空
	 * 服务器返回：协议号相同;数据区：返回空；如果服务器不认可，将关闭端口。
	 */
	MAPT_ADMIN_LOGIN,

	/*
	 * 心跳包
	 * 数据区：当前状态
	 * 服务器返回：协议号相同；数据区：空
	 */
	MAPT_ADMIN_HEARTBEAT,
	/*
	 * Comamnds
	 * data area: related commands
	 * return: information about correct commands
	 */
	MAPT_ADMIN_COMMAND
};

#define MAPT_ADMIN_LOGIN_V1 1
#define MAPT_ADMIN_HEARTBEAT_V1 1
#define MAPT_ADMIN_COMMAND_V1 1

#define CMD_RESPONSE_SIZE 128*2
#define CMD_NAME_SIZE 16

#pragma pack(push)
#pragma pack(1)
typedef struct MAPT_ADMIN_LOGIN_RESPONSE_V1{
	BYTE sessionSecurity[SECURITYSIZE];
	BYTE serverCount;
	IPPORTADDRESS ipPortAddress;
}MAPT_ADMIN_LOGIN_RESPONSE_V1;

typedef struct MAPT_ADMIN_COMMAND_RESPONSE_V1{
	char commandRespond[CMD_RESPONSE_SIZE];
	char commandType[CMD_NAME_SIZE];
}MAPT_ADMIN_COMMAND_RESPONSE_V1;

#pragma pack(pop)

#endif /* PROTOCOLADMIN_H_ */
