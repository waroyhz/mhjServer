/******************************************************************
 * 数据协议打包
 *
 * 协议格式
 * 包头 $MHJ
 * 包ID 0x30,0x78 = "0x"
 * 协议ID 0x54 = T
 * 版本		0x56 = V
 * 设备类型	0x4D = M
 * 设备ID{ 5, 7, 1, 0, 0, 1 };
 *时间戳		0xAA AA AA AA
 * 数据长度	0x2A = 42
 * 数据内容	42字节
 * CRC16		0xAA，0x55
 * 测试协议{ $MHJ0xTVM571001TTTTB123456789012345678901234567890123456789012CRrn }
 *CRC = 0x0214
 *
 * 4个加密字节替换包头
 * CRC 包括4个加密字节
 * 加密数据区 32 字节
 *******************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "MHJDefine.h"

#if !((defined WIN32) || (defined LINUX))
#include <xc.h>
#include <stdio.h>
#include <time.h>
#include <RTCC.h>
#endif

#include <stdbool.h>
#ifdef WIN32
#include <Windows.h>
#include <time.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*数据位置，已经作废
 //#define PackID_index        4 //包ID
 //#define ProtocolID_index    6 //协议ID
 //#define ProtocolVer_index   7 //协议版本
 //#define DeviceType_index    8 //设备类型
 //#define DeviceID_index      9 //设备ID
 //#define UNIXTIME_index      15 //时间戳
 //#define DataLength_index    16 //数据长度

 */
#define CRC16_index         62
#define PACKDATAMAXSIZE 42


const char MedeviceID[] = { "A1234B" };
const char MeDeviceType = 'M';
const char pVer = 'V';

//const char mSecurityData[] =
//		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//				20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

#if !((defined WIN32) || (defined LINUX))
void setMCUTime(struct tm &intTimeDate);
#endif

#pragma pack(push)
#pragma pack(1)
/*
 * 基础数据收发协议
 */
typedef struct _MHJDeviceWIFIProtocol {
	char hander[4]; //* 包头 $MHJ
	BYTE deviceType;//  * 设备类型	0x4D = M
	char deviceID[6];//  // * 设备ID		{5,7,1,0,0,1};
	UINT16 packID;//* 包ID 0x30,0x78 = "0x"
	BYTE protocolID;// * 协议ID 0x54 = T
	BYTE ver;// * 版本		0x56 = V
	INT32 time;//  * 时间戳		0xAA AA AA AA
	BYTE length;// * 数据长度	0x2A  = 42
	BYTE data[42];//  * 数据内容	42字节
	UINT16 crc16;//		0xAA，0x55
} MHJDeviceProtocol;
#pragma pack(pop)

/*校验*/
bool AnalysisCheck(char *pdata,const char*SecurityData);
/*CRC16*/
UINT16 CountCRC_16(char *pdata);
/*
 协议打包
 */
MHJDeviceProtocol* ProtocolPackage(BYTE mdeviceType,const char* pdeviceID,INT16 mpackID,BYTE mprotocolID,BYTE mver,INT32 mtime,BYTE mlength,void *pdata,const char* SecurityData);

#ifdef __cplusplus
}
#endif
#endif // PROTOCOL_H
