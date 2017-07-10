/*
 * ProtocolDevice.h
 *设备协议定义
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLDEVICECURTAIN_H_
#define PROTOCOLDEVICECURTAIN_H_
#include "ProtocolTypeDefine.h"

/*
 * 协议类型
 */
enum _MHJCurtainProtocolType {
	/*
	 * 窗帘,共用开关协议。
	 */
	MDPTC_PROTOCOL_NONE=0x200,
};

enum _MHJCurtainKeyDefine{
	    MDKTC_GROUP1_OPEN= 0x11,
		MDKTC_GROUP1_STOP,
		MDKTC_GROUP1_CLOSE,
		MDKTC_GROUP1_POWER_OFF,
		/*
		 * POWER_ON，进入对码模式
		 */

		MDKTC_GROUP1_POWER_ON,

		MDKTC_GROUP2_OPEN= 0x21,
		MDKTC_GROUP2_STOP,
		MDKTC_GROUP2_CLOSE,
		MDKTC_GROUP2_POWER_OFF,
		/*
		 * POWER_ON，进入对码模式
		 */
		MDKTC_GROUP2_POWER_ON,
};

#define MDPTS_HEARTBEAT_V1 1

#pragma pack(push)
#pragma pack(1)



#pragma pack(pop)
#endif /* PROTOCOLDEVICE_H_ */
