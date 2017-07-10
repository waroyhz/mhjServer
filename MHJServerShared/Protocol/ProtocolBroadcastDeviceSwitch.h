/*
 * ProtocolBroadcastDeviceSwitch.h
 *
 *  Created on: 2015年9月11日
 *      Author: waroy
 */

#ifndef PROTOCOLBROADCASTDEVICESWITCH_H_
#define PROTOCOLBROADCASTDEVICESWITCH_H_

/*
 * 协议类型
 */
enum _MHJBroadcastSwitchProtocolType {

	MDPTS_BROADCAST_SWITCH_STATE_SET=10,

};

#define MDPTS_BROADCAST_SWITCH_STATE_SET_V1 1

//接收者信息在发送的时候定义了
typedef struct _MDPTS_BROADCAST_SWITCH_STATE_SET_RESPONSE_V1 {
	MHJDeviceMark sendMark; //UDP广播的接受者
	uint32_t sendTimer; //触发时间
	uint32_t tragetSwitchNumber; //开关编码
	uint32_t tragetNewState; //0关闭，1打开
} MDPTS_BROADCAST_SWITCH_STATE_SET_RESPONSE_V1;




#endif /* PROTOCOLBROADCASTDEVICESWITCH_H_ */
