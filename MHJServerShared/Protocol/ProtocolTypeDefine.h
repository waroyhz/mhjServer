/*
 * ProtocolTypeDefine.h
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef PROTOCOLTYPEDEFINE_H_
#define PROTOCOLTYPEDEFINE_H_
#include "MHJDefine.h"
#ifdef LINUX
#include <netinet/in.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 设备类型定义，所有系统共用这个头文件。
 */
#define SECURITYSIZE 32
enum MHJDeviceType {
	/*
	 * 预留系统指令
	 */
	MDT_NONE,
	/*
	 * app调用
	 */
	MDT_APP,
	/*
	 * 服务器指令
	 */
	MDT_SERVER,
	/*
	 * 主机，2015年7月韩工设计第一版主机。
	 */
	MDT_HOST,
	/*
	 * 管理工具
	 */
	MDT_ADMIN,
	/*
	 * 86开关面板
	 */
	MDT_SWITCH,
	/*
	 * 窗帘控制器
	 */
	MDT_CURTAIN,
	/*
	 * 蓝牙门锁
	 */
	MDT_BluetoothDOOR,
//	        转发器（无线转红外、433MHz、315Mhz、Zeebig等）
	MDT_Transponder,
//	        传感器（温度、烟雾、燃气等独立设备）
	MDT_SENSOR,
};

//键定义版本
enum MHJkeydefine_VerType {
	/*
	 * 缺省版本
	 */
	keydefine_Ver_0,
	/*
	 * 2016-05-27 添加版本控制，
	 */
	keydefine_Ver_1,

};
enum MHJProtocolType {
	/*
	 * 预留系统指令
	 */
	MPT_NONE,
	/*
	 * app调用
	 */
	MPT_APP,
	/*
	 * 服务器指令
	 */
	MPT_SERVER,
	/*
	 * 主机，2015年7月韩工设计第一版主机。
	 */
	MPT_HOST,
	/*
	 * 管理工具
	 */
	MPT_ADMIN,
	/*
	 * 86开关面板,窗帘控制
	 */
	MPT_SWITCH,
};

#define NETPROTOCOLHANDlENGTH

//获取公共安全码，仅用于app和未绑定的设备进行设置用。
void getGeneralSecurity(char* outgeneralsecuritydata);

#pragma pack(push)
#pragma pack(1)
typedef struct _MHJDeviceMark {
	BYTE idSecurity1;
	uint32_t deviceID;
	BYTE deviceType;
	BYTE deviceFlag;
	BYTE idSecurity2;
} MHJDeviceMark;
/*
 * 网络数据包，最大区别在于支持变长。
 */
typedef struct _MHJNETProtocol {
	char hander[4]; //* 包头 $MHJ     4
	BYTE protocolType; //  * 协议分类	0x4D = M    4+1=5
	MHJDeviceMark device;	// 5+8=13
	UINT16 packID; //* 包ID 0x30,0x78 = "0x"  13+2=15
	BYTE protocolID; // * 协议ID 0x54 = T  15+1=16
	BYTE ver; // * 版本		0x56 = V  16+1=17
	DWORD time; //  * 时间戳		0xAA AA AA AA 17+4=21
	WORD length; // * 数据长度	0x2A  = 42 21+2=23
	WORD handerCRC; //包头CRC校验 23+2=25
	WORD data; //  * 数据内容首地址	变长 使用UINT16是为了占位符号，如果数据长度为0时，data数据为crc16数据，如果数据长度超过0时，crc16数据在数据区后面。 25+2=27
	//	BYTE crc16;//	CRC16实际为WORD,申明为BYTE是占位	0xAA，0x55 //
} MHJNETProtocol;

typedef struct _IPPORTADDRESS {
	in_addr_t ip;
	UINT16 prot;
} IPPORTADDRESS;


//typedef struct _ALARMTIMER{
//	uint8_t    timerType; //定时器的类型 按照日期类型 按照七曜(星期)类型
//	uint32_t   clickTime; //设定的时间 确定的时间 或 一周中的某天
//	uint8_t    repeatedData; //是否重复   重复的日期（按照星期计法）
////	uint8_t    targeSwicthNumber; //达到触发时间时，触发的对象 (按键)
////	uint8_t    targeSwitchState;  //触发对象的触发状态( 实体键 0/1  虚拟键 无意义 )
//}ALARMTIMER;

#pragma pack(pop)

enum MHJCheckReturnType {
	//校验错误
	MHJ_CHECK_RETURN_ERROR,
	//校验数据长度不足
	MHJ_CHECK_RETURN_MORE_DATA,
	//校验成功
	MHJ_CHECK_RETURN_SUCCESS
};





uint NetHanderCheck(char *pdata, int size);
uint NetAnalysisCheck(char *pdata, int size, const char*SecurityData);
UINT16 NetCountCRC_16(char *pdata, int size);
MHJNETProtocol* NetProtocolPackage(BYTE mprotocolType, const MHJDeviceMark* pdeviceID, INT16 mpackID, BYTE mprotocolID, BYTE mver, DWORD mtime, WORD mlength,
		void *pdata, const char*SecurityData);

void sprintMark(char* strMark, MHJDeviceMark* mark);






#ifdef LINUX
void sMarkprint(MHJDeviceMark* mark,char* strMark);
#endif
#ifdef __cplusplus
}
#endif
#endif /* PROTOCOLTYPEDEFINE_H_ */
