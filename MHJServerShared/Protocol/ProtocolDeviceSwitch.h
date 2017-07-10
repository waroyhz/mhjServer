/*
 * ProtocolDevice.h
 *设备协议定义
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLDEVICE_H_
#define PROTOCOLDEVICE_H_
#include "ProtocolTypeDefine.h"


#define Wifi_Using_Index_Send

/*
 * 协议类型
 */
enum _MHJSwitchProtocolType {
	/*
	 * 通用错误返回。
	 */
	MDPTS_ERROR_RESPONSE,

	MDPTS_LOGIN,

	MDPTS_HEARTBEAT,

	MDPTS_SYSTEM_UPDATE,

	MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE,

	MDPTS_SYSTEM_UPDATE_RESTART,

	//开关状态改变，主动发送给服务器
	MDPTS_SWITCH_STATE_CHANGE,
	//服务器发送给开关,设置状态改变
	MDPTS_SWITCH_STATE_SET,

	//客户端登录成功后请求信息同步
	MDPTS_INFO_SYNC,
	//更新配置
	MDPTS_UPDATE_SETUP,
	//虚拟键的定义
	MDPTS_VIRTUAL_KEY_DEFINE_SYNC,
	//解除绑定@
	MDPTS_UPINLESS_SETUP,
	//switch软重启
	MDPTS_SOFTRESTART,

	MDPTS_SWITCH_GETONLINE = 0x49,   //开关上线通知

	/*
	 * 以下为设备的通用协议，协议ID从0x50开始，协议ID必须和APP协议ID一致
	 */
	MDPTS_RFIRLEARN = 0x50,    //红外模块学习

	MDPTS_RFIRTRANSMIT,           //红外模块发射

	MDPTS_SWITCH_STATE_SET_TRANSFER,   //设置开关状态

	MDPTS_SWITCH_STATE_CHANGE_TRANSFER,  //开关状态改变

	MDPTS_SYSTEM_UPDATE_TRANSFER,      //系统更新

	MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_TRANSFER,   //系统更新下载完成

	MDPTS_SYSTEM_UPDATE_RESTART_TRANSFER,    //系统更新重启

	MDPTS_UPDATE_SETUP_TRANSFER,      //更新设置  该协议暂未实现，留作后用

	MDPTS_UPINLESS_SETUP_TRANSFER,   //解除绑定

	MDPTS_SOFTRESTART_TRANSFER,         //重启

	MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER,   //虚拟键同步

	MDPTS_SWITCH_EVENTTRIGGER_PARAMETER_SET_TRANSFER, //按键事件触发参数设置

	MDPTS_RFIR_VIRTUAL_KEY_SYNC_TRANSFER,    //红外转发设备的虚拟键更新

	MDPTS_SENSORS_CO_PARAMETER_SET_TRANSFER, //co气体检测传感器的参数设置  （报警阀值  电机转动方向）

	MDPTS_THREE_WIFI_PARAMS_SAVE_TO_FLASH, //用户定义的三组wifi存储到flash中
};

enum _MHJSwitchKeyDefine {
	//硬件开关
	MDKTS_KEY_SWITCH1 = 0x01,
	MDKTS_KEY_SWITCH2,
	MDKTS_KEY_SWITCH3,
	MDKTS_KEY_SWITCH4,
	MDKTS_KEY_SWITCH5,
	MDKTS_KEY_SWITCH6,
	MDKTS_KEY_SWITCH_LIMIT, //除了实体开关，其他开关均为虚拟键

	MDKTS_VIRTUAL_KEY1 = 0x31, //虚拟件，需要用到储存
	MDKTS_VIRTUAL_KEY2,
	MDKTS_VIRTUAL_KEY3,
	MDKTS_VIRTUAL_KEY4,
	MDKTS_VIRTUAL_KEY5,
	MDKTS_VIRTUAL_KEY6,
	MDKTS_VIRTUAL_KEY_LIMIT,

	MDKTS_Buletooth_Lock = 0x51, //蓝牙门锁,需要用到储存
};

enum _TIMERTYPE{
	TIMERTYPE_WEEK = 0,
	TIMERTYPE_DAY  = 1,

	TIMERTYPE_DELETE = 254
};

enum _TIMERSETRESULTS{
	TIMERS_NULL = 0, //定时器不存在

	TIMERS_DELETE_SUCESS = 1, //删除定时器成功

	TIMERS_ADDNEW_SUCESS = 2, //新增加成功

	TIMERS_CHANGE_SUCESS = 3, //修改成功

	TIMERS_TYPE_ERROR = 4 //类型错误
};



#pragma pack(push)
#pragma pack(1)

#define MDPTS_HEARTBEAT_V1 1
typedef struct _MDPTS_HEARTBEAT_SEND_V1 {
	int32_t TemperatureData;
} MDPTS_HEARTBEAT_SEND_V1;


/*
 * V2版本心跳包
 * 目的：检测当前网络环境环境，在APP上给用户已显示
 * 版本区别： 增加两个字段
 * lastTimeDurationMs：上次时间间隔(表示上次发送心跳包与接收到心跳包应答的时间间隔)
 * nowTimestampMs：当前时间戳（发送心跳包的当前时间  北京时区 东8）
 *
 * 注：由于esp8266提供的时间戳函数精确到秒，故增加一个字段表示系统运行时间，其单位为毫秒
 */

#define MDPTS_HEARTBEAT_V2 2
typedef struct _MDPTS_HEARTBEAT_SEND_V2 {
	int32_t TemperatureData;
	uint16_t lastTimeDurationMs;//时间间隔 单位毫秒
} MDPTS_HEARTBEAT_SEND_V2;

/*
 * V3版本心跳包
 * 增加用户正在使用的wifi索引
 */
#define MDPTS_HEARTBEAT_V3 3
typedef struct _MDPTS_HEARTBEAT_SEND_V3 {
	int32_t TemperatureData;
	uint16_t lastTimeDurationMs;//时间间隔 单位毫秒
	int32_t wifi_index_on_use; //正在使用的wifi
} MDPTS_HEARTBEAT_SEND_V3;


#define MDPTS_LOGIN_V1 1
typedef struct _MDPTS_LOGIN_SEND_V1 {
	//前三个顺序不可改变
	uint32_t ip;
	uint32_t netmask;
	uint32_t gw;
	//虚拟键定义
	uint8_t keyDefine[16];
	char buildTime[16];
} MDPTS_LOGIN_SEND_V1;

//登陆协议版本V2
// 在V1的基础上增加设备的SSID信息
#define MDPTS_LOGIN_V2 2
typedef struct _MDPTS_LOGIN_SEND_V2 {
	//前三个顺序不可改变
	uint32_t ip;
	uint32_t netmask;
	uint32_t gw;
	//虚拟键定义
	uint8_t keyDefine[16];
	char buildTime[16];
	uint8_t ssid[32];
} MDPTS_LOGIN_SEND_V2;





//登录 服务器返回：协议号相同;数据区：<调度服务器>返回重连IP地址和端口<设备服务器>返回空；如果服务器不认可，将关闭端口。
typedef struct _MDPTS_LOGIN_RESPONSE_V1 {
	BYTE sessionSecurity[SECURITYSIZE];
	uint8_t serverCount;
	IPPORTADDRESS ipPortAddress;
} MDPTS_LOGIN_RESPONSE_V1;

typedef struct _MDPTS_LOGIN_RESPONSE_V2 {
	BYTE sessionSecurity[SECURITYSIZE];
	uint8_t serverCount;
	IPPORTADDRESS ipPortAddress;
} MDPTS_LOGIN_RESPONSE_V2;

#define MDPTS_SYSTEM_UPDATE_V1 1
typedef struct _MDPTS_SYSTEM_UPDATE_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	IPPORTADDRESS ipPortAddress;
	char uri[32];
} MDPTS_SYSTEM_UPDATE_RESPONSE_V1;
typedef struct _MDPTS_SYSTEM_UPDATE_SEND_V1 {
	MHJDeviceMark sendMark;
	bool isStart;
} MDPTS_SYSTEM_UPDATE_SEND_V1;

#define MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_V1 1
typedef struct _MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_SEND_V1 {
	MHJDeviceMark sendMark;
	bool isComplete;
} MDPTS_SYSTEM_UPDATE_DOWNLOAD_COMPLETE_SEND_V1;

#define MDPTS_SYSTEM_UPDATE_RESTART_V1 1
typedef struct _MDPTS_SYSTEM_UPDATE_RESTART_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	bool isComplete;
} MDPTS_SYSTEM_UPDATE_RESTART_RESPONSE_V1;

typedef struct _MDPTS_SYSTEM_UPDATE_RESTART_SEND_V1 {
	MHJDeviceMark sendMark;
	bool isSet;
} MDPTS_SYSTEM_UPDATE_RESTART_SEND_V1;

#define MDPTS_SWITCH_STATE_CHANGE_V1 1

typedef struct _MDPTS_SWITCH_STATE_CHANGE_SEND_V1 {
	uint32_t switchNumber;
	uint32_t newState; //0关闭，1打开
} MDPTS_SWITCH_STATE_CHANGE_SEND_V1;

#define MDPTS_SWITCH_STATE_SET_V1 1
//开关接收指令，开关应答指令一致。
typedef struct _MDPTS_SWITCH_STATE_SET_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	uint32_t keyID;
	uint32_t switchNumber;
	uint32_t newState; //0关闭，1打开
} MDPTS_SWITCH_STATE_SET_RESPONSE_V1;

typedef MDPTS_SWITCH_STATE_SET_RESPONSE_V1 MDPTS_SWITCH_STATE_SET_SEND_V1;

#define MDPTS_INFO_SYNC_V1 1
//发送数据为空

typedef struct _MDPTS_INFO_SYNC_RESPONSE_V1 {
	char tiedUserTitle[32]; //绑定的用户昵称
	uint32_t tiedTime; //绑定的时间
	uint32_t serverTime; //服务器当前时间，校时用。
} MDPTS_INFO_SYNC_RESPONSE_V1;

//TODO 需要创建记录设备虚拟见更新的记录表
#define MDPTS_UPDATE_SETUP_V1 1
typedef struct _MDPTS_UPDATE_SETUP_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	uint32_t VirtualKeyUpdateTime[6]; //虚拟键更新时间
	uint32_t TimerUpdateTime[10]; //时钟定义时间
} MDPTS_UPDATE_SETUP_RESPONSE_V1;

typedef struct _MDPTS_UPDATE_SETUP_SEND_V1 {
	MHJDeviceMark sendMark;
	bool isSame;
} MDPTS_UPDATE_SETUP_SEND_V1;

//虚拟键的定义
#define MDPTS_VIRTUAL_KEY_DEFINE_SYNC_V1 1
typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_RESPONSE_V1 {
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID 如果设备类型为蓝牙，则不使用这个返回协议。请参阅_MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE__V1
	char tragetDevkey[32]; //设备安全码
	uint32_t tragetSwitchNumber; //开关编码
	uint32_t tragetNewState; //0关闭，1打开
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_RESPONSE_V1;

//当返回设备类型为蓝牙门锁时的协议
typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE_V1 {
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID
	char tragetDevkey[32]; //设备安全码
	uint8_t BluetoothDoorMac[8]; //蓝牙门锁的mac地址，蓝牙门锁mac地址为6位，末尾两位为空
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE_V1;

typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_SEND_V1 {
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_SEND_V1;

/*透传协议使用*/
//虚拟键的定义
#define MDPTS_VIRTUAL_KEY_DEFINE_SYNC_V1 1
typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_RESPONSE_V1 {
	MHJDeviceMark sourceMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID 如果设备类型为蓝牙，则不使用这个返回协议。请参阅_MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_RESPONSE__V1
	char tragetDevkey[32]; //设备安全码
	uint32_t tragetSwitchNumber; //开关编码
	uint32_t tragetNewState; //0关闭，1打开
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_RESPONSE_V1;

/*透传协议使用*/
//当返回设备类型为蓝牙门锁时的协议
typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_BluetoothDOOR_RESPONSE_V1 {
	MHJDeviceMark sourceMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
	uint8_t tragetDevID[8]; //设备ID
	char tragetDevkey[32]; //设备安全码
	uint8_t BluetoothDoorMac[8]; //蓝牙门锁的mac地址，蓝牙门锁mac地址为6位，末尾两位为空
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_BluetoothDOOR_TRANSFER_RESPONSE_V1;

/*透传协议使用*/
typedef struct _MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_SEND_V1 {
	MHJDeviceMark targeMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t switchNumber;
	uint8_t index;
	uint8_t count;
} MDPTS_VIRTUAL_KEY_DEFINE_SYNC_TRANSFER_SEND_V1;



#define MDPTS_UPINLESS_SETUP_V1 1
//解除绑定接收指令，应答指令一致
typedef struct _MDPTS_UPINLESS_SETUP_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	bool isOk;
} MDPTS_UPINLESS_SETUP_RESPONSE_V1;

typedef MDPTS_UPINLESS_SETUP_RESPONSE_V1 MDPTS_UPINLESS_SETUP_SEND_V1;

#define MDPTS_SOFTRESTART_V1 1
//软重启接收指令，应答指令一致
typedef struct _MDPTS_SOFTRESTART_RESPONSE_V1 {
	MHJDeviceMark sendMark;
	bool isOk;
} MDPTS_SOFTRESTART_RESPONSE_V1;

typedef MDPTS_SOFTRESTART_RESPONSE_V1 MDPTS_SOFTRESTART_SEND_V1;



//学习命令
#define MDPTS_RFIRLEARN_V1 1
//server发给设备的数据，表示学习命令
typedef struct _MDPTS_RFIRLEARN_FORMAT {
	 MHJDeviceMark deviceMark;
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度 == 0 为学习指令
}MDPTS_RFIRLEARN_FORMAT;



struct MDPTS_RFIRLEARN_FORMAT_MCU{
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度 == 0 为学习指令
	 WORD data[]; //  * 数据内容 MAX 512
};


//来自设备给服务启的接收应答， 表示已进入学习模式
typedef struct _MDPTS_RFIRLEARN_RESPONSE_RECEIVED_V1 {
	 MHJDeviceMark deviceMark;
	uint8_t type;  //type = 0， 进入学习模式
	bool isOk;
} MDPTS_RFIRLEARN_RESPONSE_RECEIVED_V1;

//来自设备给服务启的接收， 表示学习模式结束
typedef struct _MDPTS_RFIRLEARN_RESPONSE_FINISHED_V1 {
	 MHJDeviceMark deviceMark;
	uint8_t type;  //type = 1， 学习模式结束
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度
	 WORD data; //  * 数据内容
} MDPTS_RFIRLEARN_RESPONSE_FINISHED_V1;

//发射命令
#define MDPTS_RFIRTRANSMIT_V1 1
//server 发给设备，表示发射命令
typedef struct _MDPTS_RFIRTRANSMIT_FORMAT {
	 MHJDeviceMark deviceMark;
	 BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;            //* 版本
	 WORD length;    //* 数据长度
	 WORD data;      //最后一个WORD为CRC
}MDPTS_RFIRTRANSMIT_FORMAT;

//设备回给server的发射结果
typedef struct _MDPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1 {
	MHJDeviceMark deviceMark;
	 BYTE DataType;       //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	 BYTE ver;               //* 版本
	 WORD length;      // * 数据长度  =  1 发射完成  0表示发射失败
} MDPTS_RFIRTRANSMIT_RESPONSE_FINISHED_V1;


//按键事件触发参数设置
#define MDPTS_EVENTTRIGGER_PARAMETER_SET_TRANSFER_V1 1

//8266接受参数设置的数据结构
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
typedef  struct _MDPTS_EVENTTRIGGER_PARAMETER_SET_TRANSFER_RESPONSE_V1 {
/* data */
	MHJDeviceMark tragetDeviceMark;   /*需要设置参数的目标设备*/

	uint32_t parameterUpdateTime;     /*参数更新时间*/

	uint32_t  evemtId;

	uint8_t  isOpen;

//	uint8_t index;                    /*当前设定参数的对象编号*/
//
//	uint8_t count;                    /*需要设定参数的对象总数*/

	uint8_t  tragetSwicthNumber;      /*目标设备的按键编号*/

	uint8_t tragetSwitchState;        /*触发按键的新状态：虚拟键无意义  实体键有意义*/

	uint8_t  targetType;    /*目标参数的类型  用于区别参数数据*/

	uint16_t  rangeValueData_1;      /*参数数据：对于 TYPE_9    区间范围值 ( [ xx,xxx ) 前闭后开 )  */
	                                  /*类型为 TYPE_9时。取该数据，[0]  <=  data  < [1]*/
	uint16_t  rangeValueData_2;
	/*参数数据：对于 TYPE_9 定时时间  采用; 1970-01-01 到触发时间的秒数  */
	uint8_t    timerType; //定时器的类型 按照日期类型 按照七曜(星期)类型 0-星期 1-日期

	uint32_t   clickTime; //设定的时间 确定的时间 或 一周中的某天

	uint8_t    repeatedData; //是否重复   重复的日期（按照星期计法）0-不重复  1 重复
}MDPTS_EVENTTRIGGER_PARAMETER_SET_TRANSFER_RESPONSE_V1;



//来自开关的应答
#define MAPT_SWITCH_EVENTTRIGGER_PARAMETER_SET_V1 1
typedef struct  _MDPTS_EVENTTRIGGER_PARAMETER_SET_TRANSFER_SEND_V1{
/* data */
MHJDeviceMark deviceMark;                        /*目标设备 */

uint32_t eventId;                                /* 服务器分配的id*/

//uint8_t index;                                  /*当前设定参数的对象编号*/
//
//uint8_t count;                                 /* 需要设定参数的对象总数*/

uint8_t  tragetSwicthNumber;                  /*触发的对象 -- 自身 的对应按键*/

uint8_t  resultsNumber;                        /*开关执行结果     0 -- sucess   other:  失败原因*/

}MDPTS_EVENTTRIGGER_PARAMETER_SET_TRANSFER_SEND_V1;

/*
 * yexiaopeng 20160823
 * 定时器数据结构体的设定
 * 说明：将原先的混合结构分开独立
 */
#define MDPTS_TIMER_PARAMETER_SER_TRANSFER_V1 1
typedef struct _MDPTS_TIMER_PARAMER_SET_TRANSFER_RESPONSE_V1{
	MHJDeviceMark tragetDeviceMark;   /*需要设置参数的目标设备*/

	uint32_t parameterUpdateTime;     /*参数更新时间*/

	uint32_t  timerId;               /*定时器编号id*/

	uint8_t  isOpen;                /*是否启用*/

	uint8_t  tragetSwicthNumber;      /*目标设备的按键编号*/

	uint8_t tragetSwitchState;        /*触发按键的新状态：虚拟键无意义  实体键有意义*/

	uint8_t    timerType; //定时器的类型 按照日期类型 按照七曜(星期)类型 0-星期 1-日期  223 - 删除该定时器

	uint32_t   clickTime; //设定的时间 确定的时间 或 一周中的某天

	uint8_t    repeatedData; //是否重复   重复的日期（按照星期计法）0-不重复  1 重复
}MDPTS_TIMER_PARAMER_SET_TRANSFER_RESPONSE_V1;

//定时器设置的开关回复
typedef struct _MDPTS_TIMER_PARAMER_SET_TRANSFER_SEND_V1{
	/* data */
	MHJDeviceMark deviceMark;       /*目标设备 */

	uint32_t timerId;              /* 服务器分配的id*/

	uint8_t  tragetSwicthNumber;  /*触发的对象 -- 自身 的对应按键*/

	uint8_t  resultsNumber;      /*开关执行结果     0 -- sucess   other:  失败原因*/
}MDPTS_TIMER_PARAMER_SET_TRANSFER_SEND_V1;


//红外转发器的虚拟键设置
#define  MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_V1 1
typedef struct _MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_TRANSFER_SEND_V1{
	MHJDeviceMark targeMark;
	uint32_t VirtualKeyUpdateTime; //虚拟键更新时间
	uint32_t keyDefine;
	BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	BYTE ver;            //* 版本
	WORD length;    //* 数据长度
	WORD data; //  * 数据内容,变长，最后一个WORD为CRC校验

}MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_TRANSFER_SEND_V1;

typedef struct _RFIR_VIRTUAL_SAVE_DATA_V1{
	BYTE DataType;  //* （开始位 高或低电平）数据分类  红外、315射频、433射频数据标志
	BYTE ver;            //* 版本
	WORD length;    //* 数据长度
	WORD data; //  * 数据内容,变长，最后一个WORD为CRC校验
}RFIR_VIRTUAL_SAVE_DATA_V1;


typedef  struct _MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_TRANSFER_RESPONSE_V1{
	MHJDeviceMark deviceMark;
	bool isOk; // 0--ok 1--no
}MAPT_TO_TRANSPONDER_RFIR_VIRTUAL_KEY_SYNC_TRANSFER_RESPONSE_V1;


/*
 * MDPT_TO_SENSORS_CO_PARAMETER_SET_TRANSFER_SEND_V1
 * co气体传感装置
 *
 * 修改　2016/09／05　yexiaopeng
 * 增加新的结构体成员变量
 *
 */
//co气体检测传感器的参数设置
#define MDPT_TO_SENSORS_CO_PARAMETER_SET_TRANSFER_V1 1 //co气体检测传感器的参数设置  （报警阀值  电机转动方向）
typedef struct _MDPT_TO_SENSORS_CO_PARAMETER_SET_TRANSFER_SEND_V1{
	MHJDeviceMark tragetDeviceMark;   /*需要设置参数的目标设备*/
	uint32_t parameterUpdateTime;     /*参数更新时间*/
	uint16_t  rangeValueData;        //报警值  达到报警
	uint8_t   motorRunDirection;      //电机转动方向
}MDPT_TO_SENSORS_CO_PARAMETER_SET_TRANSFER_SEND_V1;


/*添加 2016/12/20  guoshuangsheng
 * 存储用户wifi信息
 */
//用户定义的三组WIFI结构体
typedef struct set_wifi_custom_info{
	uint8_t ssid[32];
	uint8_t password[64];
} PARAM_WIFI_SAVE_TO_FLASH;

//存储用户WIFI信息协议结构体
#define MDPT_TO_FLASH_SET_WIFI_INFO_V1 1
typedef struct _MDPT_TO_FLASH_WIFI_INFO_SET_V1{
	MHJDeviceMark tragetDeviceMark;
	PARAM_WIFI_SAVE_TO_FLASH WIFI_INFO[3];
}MDPT_TO_FLASH_SET_WIFI_INFO_RESPONSE_V1;

//响应用户WIFI信息存储是否成功的协议结构体
typedef struct _MDPT_TO_FLASH_SET_WIFI_INFO_SEND_V1{
	MHJDeviceMark tragetDeviceMark;
	PARAM_WIFI_SAVE_TO_FLASH WIFI_INFO[3];
	bool writeok; // 0--ok 1--no
}MDPT_TO_FLASH_SET_WIFI_INFO_SEND_V1;







#pragma pack(pop)
#endif /* PROTOCOLDEVICE_H_ */
