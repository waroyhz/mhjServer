/*
 * ProtocolServerRedisDefine.h
 *
 *  Created on: 2015年11月5日
 *      Author: ruancongming
 */

#ifndef PROTOCOLSERVERREDISDEFINE_H_
#define PROTOCOLSERVERREDISDEFINE_H_
#include <iostream>
#include <deque>
//定义保存到Redis中的设备服务器列表信息
typedef struct _ServerOnRedis {
	std::string serverNum; //server 服务号
	std::string channel;      //server 订阅通道
	IPPORTADDRESS ipportaddr;
//	std::deque<int> deviceList;   //server 负载信息 （Dispatch服务器不用保存设备列表信息）
	std::deque<IPPORTADDRESS> iplist;
	std::string beatTime;
} ServerOnRedis;

#endif /* PROTOCOLSERVERREDISDEFINE_H_ */
