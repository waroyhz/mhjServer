/*
 * ProtocolHost.h
 *主机协议定义
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLHOST_H_
#define PROTOCOLHOST_H_
#include "ProtocolTypeDefine.h"

/*
 * 协议类型
 */
enum _MHJHOSTProtocolType {
	/*
	 * 通用错误返回。
	 */
	MHPT_ERROR_RESPONSE,

	/*
	 * 登录
	 * 数据区：内网ip，结构为in_addr_t，如果长度超出就是有多个。
	 * 服务器返回：协议号相同;数据区：<调度服务器>返回重连IP地址和端口<设备服务器>返回空；如果服务器不认可，将关闭端口。
	 */
	MHPT_LOGIN,
	/*
	 * 心跳包
	 * 数据区：当前状态
	 * 服务器返回：协议号相同；数据区：空
	 */
	MHPT_HEARTBEAT,
	/**
	 * 客户端登录协议，如果主机还没有客户端的安全码，那么客户端登录到主机，由主机向服务器转发登录
	 * 主机只有会话安全码，而没有设备安全码。
	 */
	MHPT_CLIENT_LOGIN,
	/**
	 * 主机转发给服务器收到的指令
	 */
	MHPT_COMMON_HOST_TO_SERVER=0x49,
	/**
	 * 0x50之后是服务器通用转换协议.
	 */
	MHPT_COMMON_TRANSFER=0x50

};

/*
 * 协议版本号定义，如果协议版本存在修改，需要具体说明修改及解析区别。
 * 1为初始版本，超过1的，对应的解析均需要处理。
 */
#define MHPT_HEARTBEAT_V1 1

#pragma pack(push)
#pragma pack(1)

#define MHPT_LOGIN_V1 1
typedef struct _MHPT_LOGIN_RESPONSE_V1 {
	BYTE sessionSecurity[SECURITYSIZE];
	BYTE serverCount;
	IPPORTADDRESS ipPortAddress;
} MHPT_LOGIN_RESPONSE_V1;

typedef struct _MHPT_LOGIN_SEND_V1 {
	//前三个顺序不可改变
	uint32_t ip;
	uint32_t netmask;
	uint32_t gw;
	char buildTime[16];
} MHPT_LOGIN_SEND_V1;

#define MHPT_CLIENT_LOGIN_V1 1
typedef struct MHPT_CLIENT_RESPONSE_V1 {
	BYTE clientSessionSercurity[32]; //客户端的会话安全码
	BYTE responseData; //客户端回复数据占位符
} MHPT_CLIENT_LOGIN_RESPONSE_V1;
//客户端登录协议与客户端发送一致

#pragma pack(pop)
#endif /* PROTOCOLHOST_H_ */
