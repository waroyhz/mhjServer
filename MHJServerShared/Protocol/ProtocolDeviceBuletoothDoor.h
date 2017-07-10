/*
 * ProtocolDeviceBuletoothDoor.h
 *
 *  Created on: 2015年11月18日
 *      Author: waroy
 */

#ifndef PROTOCOLDEVICEBULETOOTHDOOR_H_
#define PROTOCOLDEVICEBULETOOTHDOOR_H_
#include "ProtocolTypeDefine.h"
enum _MHJBuletoothDoorProtocolType {
	/*
	 * 蓝牙开门
	 */
	MDPTBD_OPEN=0x1,
};


#pragma pack(push)
#pragma pack(1)

#define MDPTBD_OPEN_V1 1
typedef struct _MDPTBD_OPEN_SEND_V1 {
	uint16_t outTime; //开门超时 单位秒
} MDPTBD_OPEN_SEND_V1;

typedef struct _MDPTBD_OPEN_RESPONSE_V1 {
	uint16_t softVer;//软件版本
	BYTE openState; //返回1为成功
	BYTE usePower; //是否有电量值
	uint16_t powerNumber; //电量值
} MDPTBD_OPEN_RESPONSE_V1;

#pragma pack(pop)
#endif /* PROTOCOLDEVICEBULETOOTHDOOR_H_ */
