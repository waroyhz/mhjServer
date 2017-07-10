/*
 * ProtocolDeviceServer.h
 *	APP收发协议。
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLDEVICESERVER_H_
#define PROTOCOLDEVICESERVER_H_
#include "ProtocolTypeDefine.h"
/*
 * 协议类型
 */
enum _MHJAPPProtocolType {
	/*
	 * 通用错误返回。
	 */
	MAPT_ERROR_RESPONSE,
	/*
	 * 登录
	 * 数据区：空
	 * 服务器返回：协议号相同;数据区：返回空；如果服务器不认可，将关闭端口。
	 */
	MAPT_LOGIN,
	/*
	 * 心跳包
	 * 数据区：当前状态
	 * 服务器返回：协议号相同；数据区：空
	 */
	MAPT_HEARTBEAT,

	MAPT_TO_DEVICE_SCAN_SSID,

	MAPT_TO_DEVICE_GET_SCAN_SSID,

	MAPT_TO_DEVICE_SET_SSID,

	MAPT_TO_DEVICE_GET_KEYDEFINE,

	//app接收开关状态改变
	MAPT_SWITCH_STATE_CHANGE,
	//app发送开关状态改变要求
	MAPT_TO_SWITCH_STATE_SET,
	//开关发回的状态接受
	MAPT_SWITCH_STATE_SET_RESPONSE,

	MAPT_TO_SWITCH_SYSTEM_UPDATE,

	MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE,

	MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE,

	MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART,

	MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE,
	//更新配置
	MAPT_TO_SWITCH_UPDATE_SETUP,

	/*解除绑定*/
	MAPT_TO_SWITCH_UPINLESS_SETUP,

	MAPT_SWITCH_UPINLESS_SETUP_RESPONSE,

	/*switch软重启*/
	MAPT_TO_SWITCH_SOFTRESTART,

	MAPT_SWITCH_SOFTRESTART_RESPONSE,

	MAPT_SWITCH_GETONLINE = 0x49,   //开关上线通知

	/*
	 * 以下为APP通用协议，协议ID从0x50开始，协议ID必须和设备协议ID一致
	 */
	MAPT_TO_SWITCH_RFIRLEARN=0x50,  //红外模块学习

	MAPT_TO_SWITCH_RFIRTRANSMIT,      //红外模块发射

	MAPT_TO_SWITCH_STATE_SET_TRANSFER,       //设置开关状态

	MAPT_SWITCH_STATE_CHANGE_TRANSFER,     //开关状态改变

	MAPT_TO_SWITCH_SYSTEM_UPDATE_TRANSFER,   //系统更新

	MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_TRANSFER,  //系统更新下载完成

	MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_TRANSFER, //系统更新重启

	MAPT_TO_SWITCH_UPDATE_SETUP_TRANSFER,  //系统更新设置   该协议暂未实现，留作后用

	MAPT_TO_SWITCH_UPINLESS_SETUP_TRANSFER,  //解除绑定

	MAPT_TO_SWITCH_SOFTRESTART_TRANSFER,   //重启

	MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER,  //虚拟键同步

	MAPT_TO_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER, //按键事件触发参数设置

    MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_TRANSFER, //红外转发设备的虚拟键更新

	MDPT_TO_SENSORS_CO_PARAMETER_SET_TRANSFER,//co气体检测传感器的参数设置  （报警阀值  电机转动方向）

};

#define MAPT_HEARTBEAT_V1 1

#pragma pack(push)
#pragma pack(1)

#define MAPT_LOGIN_V1 1
typedef struct MAPT_LOGIN_RESPONSE_V1 {
	BYTE sessionSecurity[SECURITYSIZE];
	BYTE serverCount;
	IPPORTADDRESS ipPortAddress;
} MAPT_LOGIN_RESPONSE_V1;

#define MAPT_TO_DEVICE_SCAN_SSID_V1 1
typedef enum _ssid_auth_mode {
	SSID_AUTH_OPEN = 0, SSID_AUTH_WEP, SSID_AUTH_WPA_PSK, SSID_AUTH_WPA2_PSK, SSID_AUTH_WPA_WPA2_PSK, SSID_AUTH_MAX
} SSID_AUTH_MODE;

typedef struct _MAPT_TO_DEVICE_SCAN_SSID_RESPONSE_V1 {
	bool isStart;
} MAPT_TO_DEVICE_SCAN_SSID_RESPONSE_V1;

#define MAPT_TO_DEVICE_GET_SCAN_SSID_V1 1

typedef struct _MAPT_TO_DEVICE_GET_SCAN_SSID_RESPONSE_DATA_V1 {
	BYTE index;
	BYTE count;
	int8_t rssi;
	BYTE ssid[32];
	BYTE bssid[6];
	BYTE channel;
	SSID_AUTH_MODE authmode;
} MAPT_TO_DEVICE_GET_SCAN_SSID_RESPONSE_DATA_V1;

#define MAPT_TO_DEVICE_SET_SSID_V1 1
typedef struct _MAPT_TO_DEVICE_SET_SSID_SEND_V1 {
	BYTE ssid[32];
	BYTE password[32];
	BYTE bssid[6];
} MAPT_TO_DEVICE_SET_SSID_SEND_V1;

enum {
	wifi_STATION_IDLE = 0,
	wifi_STATION_CONNECTING,
	wifi_STATION_WRONG_PASSWORD,
	wifi_STATION_NO_AP_FOUND,
	wifi_STATION_CONNECT_FAIL,
	wifi_STATION_GOT_IP
};

typedef struct _MAPT_TO_DEVICE_SET_SSID_RESPONSE_V1 {
	uint8_t wifi_status;
} MAPT_TO_DEVICE_SET_SSID_RESPONSE_V1;

#define MAPT_TO_DEVICE_GET_KEYDEFINE_V1 1
typedef struct _MAPT_TO_DEVICE_GET_KEYDEFINE_RESPONSE_V1 {
	BYTE keyDefine[16];
} MAPT_TO_DEVICE_GET_KEYDEFINE_RESPONSE_V1;

#define MAPT_TO_EVICE_UPINLESS_V1 1
typedef struct _MAPT_TO_EVICE_UPINLESS_RESPONSE_V1 {
	BYTE keyDefine[16];
} MAPT_TO_EVICE_UPINLESS_RESPONSE_V1;

#define MAPT_SWITCH_STATE_CHANGE_V1 1

//typedef struct MAPT_SWITCH_STATE_SET_RESPONSE_RESPONSE_V1 MAPT_SWITCH_STATE_CHANGE_RESPONSE_V1;

#define MAPT_TO_SWITCH_STATE_SET_V1 1

typedef struct _MAPT_TO_SWITCH_STATE_SET_SEND_V1 {
	MHJDeviceMark deviceMark;
	uint32_t keyID;
	uint32_t newState; //0关闭，1打开
} MAPT_TO_SWITCH_STATE_SET_SEND_V1;

typedef struct _MAPT_TO_SWITCH_STATE_SET_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	uint32_t keyID;
	bool accept; //如果不成功，可能不在线，或者没有权限。
} MAPT_TO_SWITCH_STATE_SET_RESPONSE_V1;

//来自开关的应答
#define MAPT_SWITCH_STATE_SET_RESPONSE_V1 1

typedef struct _MAPT_SWITCH_STATE_SET_RESPONSE_V1 {
	MHJDeviceMark deviceMark; //当mark为空时，表示是硬件上发生类改变。
	uint32_t keyID;
	uint32_t newState; //0关闭，1打开
} MAPT_SWITCH_STATE_SET_RESPONSE_RESPONSE_V1;

#define MAPT_TO_SWITCH_SYSTEM_UPDATE_V1 1
typedef struct _MAPT_TO_SWITCH_SYSTEM_UPDATE_SEND_V1 {
	MHJDeviceMark deviceMark;
	IPPORTADDRESS ipaddress;
	char uri[32];
} MAPT_TO_SWITCH_SYSTEM_UPDATE_SEND_V1;
typedef struct _MAPT_TO_SWITCH_SYSTEM_UPDATE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool accept; //如果不成功，可能不在线，或者没有权限。
} MAPT_TO_SWITCH_SYSTEM_UPDATE_RESPONSE_V1;

#define MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE_V1 1
typedef struct _MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isStart;
} MAPT_SWITCH_SYSTEM_UPDATE_RESPONSE_RESPONSE_V1;

#define MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE_V1 1
typedef struct _MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isComplete;
} MAPT_SWITCH_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_RESPONSE_RESPONSE_V1;

#define MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_V1 1
typedef struct _MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_SEND_V1 {
	MHJDeviceMark deviceMark;
} MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_SEND_V1;
typedef struct _MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool accept; //如果不成功，可能不在线，或者没有权限。
} MAPT_TO_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_V1;

#define MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_V1 1
typedef struct _MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isSet;
} MAPT_SWITCH_SYSTEM_UPDATE_RESTART_RESPONSE_RESPONSE_V1;

#define MAPT_TO_SWITCH_UPDATE_SETUP_V1 1
typedef struct _MAPT_TO_SWITCH_UPDATE_SETUP_SEND_V1 {
	MHJDeviceMark deviceMark;
} MAPT_TO_SWITCH_UPDATE_SETUP_SEND_V1;
typedef struct _MAPT_TO_SWITCH_UPDATE_SETUP_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool accept;
} MAPT_TO_SWITCH_UPDATE_SETUP_RESPONSE_V1;


/* 解绑命令 */
#define MAPT_SWITCH_UPINLESS_SETUP_RESPONSE_V1 1
typedef struct _MAPT_TO_SWITCH_UPINLESS_SETUP_RESPONSE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isOk;
} MAPT_SWITCH_UPINLESS_SETUP_RESPONSE_RESPONSE_V1;


#define MAPT_TO_SWITCH_UPINLESS_SETUP_V1 1
/* 解绑命令 */
typedef struct _MAPT_TO_SWITCH_UPINLESS_SETUP_SEND_V1 {
	MHJDeviceMark deviceMark;
} MAPT_TO_SWITCH_UPINLESS_SETUP_SEND_V1;

typedef struct _MAPT_TO_SWITCH_UPINLESS_SETUP_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool accept; //如果不成功，可能不在线，或者没有权限。
	bool isOk; //SWITCH回复server后，server回复APP
} MAPT_TO_SWITCH_UPINLESS_SETUP_RESPONSE_V1;



/* switch软件重启 */
#define MAPT_SWITCH_SOFTRESTART_RESPONSE_V1 1
typedef struct _MAPT_SWITCH_SOFTRESTART_RESPONSE_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isOk;
} MAPT_SWITCH_SOFTRESTART_RESPONSE_RESPONSE_V1;
/* switch软件重启 */
#define MAPT_TO_SWITCH_SOFTRESTART_V1 1
typedef struct _MAPT_TO_SWITCH_SOFTRESTART_SEND_V1 {
	MHJDeviceMark deviceMark;
} MAPT_TO_SWITCH_SOFTRESTART_SEND_V1;

typedef struct _MAPT_TO_SWITCH_SOFTRESTART_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool accept; //如果不成功，可能不在线，或者没有权限。
	bool isOk; //SWITCH回复server后，server回复APP
} MAPT_TO_SWITCH_SOFTRESTART_RESPONSE_V1;



/*switch 学习命令*/
#define MAPT_TO_SWITCH_RFIRLEARN_V1 1
//server给APP的应答
typedef struct _MAPT_TO_SWITCH_RFIRLEARN_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isOk;
} MAPT_TO_SWITCH_RFIRLEARN_RESPONSE_V1;
//进入学习模式，server给APP转发设备的转态
typedef struct _MAPT_RFIRLEARN_RESPONSE_RECEIVED_V1 {
//	MHJDeviceMark sendMark;
	MHJDeviceMark deviceMark;
	uint8_t type;  //type = 0， 进入学习模式
	bool isOk;
} MAPT_RFIRLEARN_RECEIVED_RESPONSE_V1;
//学习结束，server给APP转发设备的应答
typedef struct _MAPT_RFIRLEARN_RESPONSE_FINISHED_V1 {
	 MHJDeviceMark deviceMark;
	 uint8_t type;  //type = 1， 学习结束
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度
	 WORD data; //  * 数据内容,变长，最后一个WORD为CRC校验
}MAPT_RFIRLEARN_FINISHED_RESPONSE_V1;

typedef struct _MAPT_TO_SWITCH_RFIRLEARN_FORMAT {
	 MHJDeviceMark deviceMark;
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度 == 0 为学习指令
}MAPT_TO_SWITCH_RFIRLEARN_FORMAT;



/*switch 发射命令*/
#define MAPT_TO_SWITCH_RFIRTRANSMIT_V1 1
//server给APP的应答
typedef struct _MAPT_TO_SWITCH_RFIRTRANSMIT_RESPONSE_V1 {
	MHJDeviceMark deviceMark;
	bool isOk;
} MAPT_TO_SWITCH_RFIRTRANSMIT_RESPONSE_V1;
//发射结束，server给APP转发设备的执行结果
typedef struct _MAPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1 {
	MHJDeviceMark deviceMark;
	 BYTE DataType;       //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;               //* 版本
	 WORD length;      // * 数据长度  =  1 发射完成
} MAPT_RFIRTRANSMIT_FINISHED_RESPONSE_V1;

typedef struct _MAPT_TO_SWITCH_RFIRTRANSMIT_FORMAT {
	 MHJDeviceMark deviceMark;
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度
	 WORD data; //  * 数据内容,变长，最后一个WORD为CRC校验
}MAPT_TO_SWITCH_RFIRTRANSMIT_FORMAT;
typedef MAPT_RFIRLEARN_RECEIVED_RESPONSE_V1 MAPT_RFIRTRANSMIT_RESPONSE_RECEIVED_V1;

/*透传协议使用*/
//虚拟键的定义
#define MAPT_VIRTUAL_KEY_DEFINE_SYNC_V1 1
typedef struct _MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_SEND_V1 {
	MHJDeviceMark targeMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID 如果设备类型为蓝牙，则不使用这个返回协议。请参阅_MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE__V1
	char tragetDevkey[32]; //设备安全码
	uint32_t tragetSwitchNumber; //开关编码
	uint32_t tragetNewState; //0关闭，1打开
} MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_SEND_V1;

//当返回设备类型为蓝牙门锁时的协议
typedef struct _MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_BluetoothDOOR_SEND__V1 {
	MHJDeviceMark targeMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID
	char tragetDevkey[32]; //设备安全码
	uint8_t BluetoothDoorMac[8]; //蓝牙门锁的mac地址，蓝牙门锁mac地址为6位，末尾两位为空
} MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_TRANSFER_SEND__V1;

typedef struct _MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_RESPONSE_V1 {
	MHJDeviceMark sourceMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
} MAPT_TO_SWITCH_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_RESPONSE_V1;


// 设备触发自身按键条件的参数设置
//一条协议更新一次数据，一次数据对应一个key的触发条件
#define MAPT_TO_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER_V1 1
/* * @targetparameterType 参数的类型用于区别参数数据 0 -- 定时器闹钟参数   1 -- 传感器的参数
   * @rangeValueData      一个int[2]数组   按键触发的传感器数值范围 (int[0],int[1]])
   * @timerType           定时器的类型
   *                      0：按一礼拜七天计算的类型 用 0 表示，此时 设定的 clickTime 是 24小时中的某个时刻 clickTime
   *                        如 设定15:32:05  clickTime = 15 * 60 * 60 + 32 * 60 + 5 = 55925
   *                      1：表示未来的某个时刻 用 1 表示， 此时设定的 clickTime
   *                           表示当前距离基准时间(1970.01.01 00:00:00 GMT + 8)的时间戳 单位为秒
   *                           以东八区时间为准（因为ESP8266获取的local时间戳为 GMT + 8）
   *                           (因为ESP8266使用系统API获取的是 GMT + 8)
   *
   * @clickTime 见上文
   *
   * @repeatedData 重复的日期（按照星期计法，未来的某个时刻，不存在重复的可能性）
   *               数据结构 00000000B -  87654321
   *                   8位表示是否每周重复 0 -- 不重复  1 -- 重复
   *                 7 6 5 4 3 2 1    表示闹铃设定的日期
   *           如设置为每礼拜六触发 repeatedData 1010 0000 十六进制为 0xa0  十进制数为 32 + 128=160
   *         不重复 礼拜一 -- 0x01   礼拜二 -- 0x02   礼拜三 -- 0x04 礼拜四 -- 0x08 礼拜五 -- 0x10
   *              礼拜六 -- 0x20   礼拜日 -- 0x40
   *        重复   + 0x80
   ********************************************************************************/
typedef  struct _MAPT_TO_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER_SEND_V1 {
/* data */
	/* data */
	MHJDeviceMark tragetDeviceMark;   /*需要设置参数的目标设备*/

	uint32_t parameterUpdateTime;     /*参数更新时间*/

	uint8_t index;                    /*当前设定参数的对象编号*/

	uint8_t count;                    /*需要设定参数的对象总数*/

	uint8_t  tragetSwicthNumber;      /*目标设备的按键编号*/

	uint8_t tragetSwitchState;        /*触发按键的新状态：虚拟键无意义  实体键有意义*/

	uint8_t  targetparameterType;    /*目标参数的类型  用于区别参数数据*/

	uint16_t  rangeValueData_1;      /*参数数据：对于 TYPE_9    区间范围值 ( [ xx,xxx ) 前闭后开 )  */
	                                  /*类型为 TYPE_9时。取该数据，[0]  <=  data  < [1]*/

  uint16_t  rangeValueData_2;

	uint8_t    timerType; //定时器的类型 按照日期类型 按照七曜(星期)类型 0-星期 1 -日期

	uint32_t   clickTime; //设定的时间 见注释

	uint8_t    repeatedData; //是否重复  见注释
}MAPT_TO_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER_SEND_V1;


//来自开关的应答
#define MAPT_SWITCH_EVENTTRIGGER_PARAMETER_SET_V1 1

typedef struct  _MAPT_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER_RESPONSE_V1{
/* data */
uint8_t index;                    /*当前更新完毕的对象编号*/

uint8_t count;                    /*需要设定参数的对象总数*/

uint8_t  tragetSwicthNumber;      /*参数对应的开关编号 -- 自身 的对应按键*/

uint8_t  resultsNumber;           /*参数设置的结果     0 -- sucess   other:  失败原因*/

}MAPT_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER_RESPONSE_V1;



/*APP 获取 8266设备上传感器的数值*/
#define MAPT_TO_SWITCH_RETURN_SENSORSNOWVALUE_V1 1
typedef struct _MAPT_TO_SWITCH_RETURN_SENSORSNOWVALUE_TRANSFER_V1{

}MAPT_TO_SWITCH_RETURN_SENSORSNOWVALUE_TRANSFER_V1;

//用于对互控键中的实体键所在的设备进行设置
//虚拟键使用 场景键更新指令




#pragma pack(pop)

#endif /* PROTOCOLDEVICESERVER_H_ */



/*
 * 通用协议
// */

typedef struct _COMMON_RESPONSE{
	MHJDeviceMark deviceMark;
	bool accept; //如果不成功，可能不在线，或者没有权限。
} COMMON_RESPONSE;

/*
 * 开关上线通知
 */
typedef struct _MDPTS_SWITCH_GETONLINE_V1 {
	MHJDeviceMark deviceMark;
	//前三个顺序不可改变
	uint32_t ip;
	uint32_t netmask;
	uint32_t gw;
	//虚拟键定义
	uint8_t keyDefine[16];
	char buildTime[16];
} MDPTS_SWITCH_GETONLINE_V1;
