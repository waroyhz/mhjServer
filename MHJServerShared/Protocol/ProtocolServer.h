/*
 * ProtocolDeviceServer.h
 *	设备服务器和调度服务器的协议。
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLDEVICESERVER_H_
#define PROTOCOLDEVICESERVER_H_
#include "ProtocolTypeDefine.h"
/*
 * 协议类型
 */
enum _MHJHOSTProtocolType{
	/*
	 * 通用错误返回。
	 */
	MSPT_ERROR_RESPONSE,

	/*
	 * 登录
	 * 数据区：空
	 * 服务器返回：协议号相同;数据区：返回空；如果服务器不认可，将关闭端口。
	 */
	MSPT_LOGIN,


	/*
		 * 心跳包
		 * 数据区：当前状态
		 * 服务器返回：协议号相同；数据区：空
		 */
	MSPT_HEARTBEAT,
/*
 * 状态包：
 * 数据区： 服务器IP信息， 心跳时间
 * 服务器返回: 空
 */
	MSPT_SERVERINFO,

	/*
	 * 需要转发的命令
	 * 数据区：目标设备执行的命令
	 * 服务器返回：请求是否被受理
	 */
	MSPT_CMDTRANSFER,

	/*
	 * 设备上线通知
	 * 数据区：已上线设备的ID
	 * 服务器处理：删除本机上与该设备有关的会话id
	 */
	MSPT_ONLINEINFORM,
	/*
	 * 待转发给Java服务器处理的请求
	 */
	MSPT_TOJAVASERVER
};

#define MSPT_LOGIN_V1 1



#pragma pack(push)
#pragma pack(1)

#define MSPT_HEARTBEAT_V1 1
typedef struct _MSPT_LOGIN_RESPONSE_V1{
	BYTE sessionSecurity[SECURITYSIZE];
	BYTE serverCount;
	IPPORTADDRESS ipPortAddress[4];
}MSPT_LOGIN_RESPONSE_V1;

typedef struct _MSPT_SERVERINFO_REDIS_V1{
	char beatTime[11];         					     //心跳时间
	BYTE serverCount;                               //可用的IP
	IPPORTADDRESS ipPortAddress[4];    //IP 列表
}MSPT_SERVERINFO_REDIS_V1;

//Redis发布订阅协议
typedef struct _MHJSUBSCRIBEProtocol {
	uint32_t type;                      //协议类型
	char source[32];                     //数据来源
	MHJDeviceMark srcdevice;   //源设备
	BYTE data;                             //传输数据首地址
} MHJSUBSCRIBEProtocol;

//Redis请求转发协议
typedef struct _MHJSUBSCRIBETRANSFERProtocol {
	BYTE mprotocolType;          //协议类型
	int mpackID;					  //包ID（暂时不用）
	MHJDeviceMark dstdevice;  //目标设备
	BYTE mprotocolID;			 //协议ID
	BYTE mver;						 //协议版本号
	BYTE data;                           //传输数据
} MHJSUBSCRIBETRANSFERProtocol;
#pragma pack(pop)
#endif /* PROTOCOLDEVICESERVER_H_ */
