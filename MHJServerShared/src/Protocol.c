#include "Protocol.h"


#if !((defined WIN32) || (defined LINUX))
#include "system.h"
#include "system_config.h"
#endif

#include <stdio.h>
#include <time.h>
WORD LastPackID, SendPackID;
const char pHander[] = { "$MHJ" };

/*********************************************************************
* 校验数据 返回 真假
* 数据转存
********************************************************************/
bool AnalysisCheck(char *pdata,const char*SecurityData)
{
	char CurrentData[64];
	memcpy((char *)CurrentData, (char *)pdata, sizeof(MHJDeviceProtocol));
	MHJDeviceProtocol * rxProtocolData = (MHJDeviceProtocol*)(char *)CurrentData;

	UINT16 CRC16Data;
	unsigned char KeysA, KeysB;
	KeysA = rxProtocolData->packID & 0x000F; //低四位
	KeysB = ((rxProtocolData->packID >> 4) & 0x000F) + 16; //第五位-第八位
	rxProtocolData->hander[0] = SecurityData[KeysA];
	rxProtocolData->hander[1] = SecurityData[KeysB];
	KeysA = SecurityData[KeysA] & 0x0F;
	KeysB = (SecurityData[KeysB] & 0x0F) + 16;
	rxProtocolData->hander[2] = SecurityData[KeysA];
	rxProtocolData->hander[3] = SecurityData[KeysB];
	CRC16Data = CountCRC_16((char *)CurrentData);
	// rxProtocolData->crc16 = 0x1402;
	if (CRC16Data == rxProtocolData->crc16)
		return true;
	return false;
}
UINT16 CountCRC_16(char *pdata)
{
	unsigned char	i, j;
	UINT16	wCRC;
	unsigned char	byData;
	unsigned char	cy1, cy2;
	//CRC-CCITT
	wCRC = 0x00;
	for (i = 0; i < CRC16_index; i++){
		byData = *pdata++;
		for (j = 0; j < 8; j++){
			cy1 = (wCRC & 0x8000) ? 1 : 0;
			cy2 = (byData & 0x80) ? 1 : 0;
			wCRC <<= 1;
			byData <<= 1;
			if (cy1 ^ cy2){
				wCRC ^= 0x1021;
				wCRC |= 0x0001;
			}
		}
	}
	return wCRC;
}

MHJDeviceProtocol* ProtocolPackage(BYTE mdeviceType,const char* pdeviceID,INT16 mpackID,BYTE mprotocolID,BYTE mver,INT32 mtime,BYTE mlength,void *pdata,const char* SecurityData)
{
//	INT32 clock;
//	struct tm intTimeDate;
	MHJDeviceProtocol* txProtocolData=malloc(sizeof(MHJDeviceProtocol));
	memset(txProtocolData, 0, sizeof(MHJDeviceProtocol));
	txProtocolData->deviceType = mdeviceType;
	memcpy(txProtocolData->deviceID, pdeviceID, sizeof(txProtocolData->deviceID));
	txProtocolData->packID = mpackID;
	txProtocolData->protocolID = mprotocolID;
	txProtocolData->ver = mver;
	txProtocolData->time = mtime;
	txProtocolData->length = mlength;
	memcpy(txProtocolData->data, pdata, mlength);


	unsigned char KeysA, KeysB;
	KeysA = txProtocolData->packID & 0x000F;
	KeysB = ((txProtocolData->packID >> 4) & 0x000F) + 16;
	txProtocolData->hander[0] = SecurityData[KeysA];
	txProtocolData->hander[1] = SecurityData[KeysB];
	KeysA = SecurityData[KeysA] & 0x0F;
	KeysB = (SecurityData[KeysB] & 0x0F) + 16;
	txProtocolData->hander[2] = SecurityData[KeysA];
	txProtocolData->hander[3] = SecurityData[KeysB];

//#if !((defined WIN32) || (defined LINUX))
//	setMCUTime(intTimeDate);
//#else
//	time_t rawtime;
//	time(&rawtime);
//	intTimeDate = *localtime(&rawtime);
//#endif
//	if (intTimeDate.tm_year <= 69){
//		intTimeDate.tm_year = intTimeDate.tm_year + 100;
//	}
//	//    if(intTimeDate.tm_year>=70){
//	//        intTimeDate.tm_year=intTimeDate.tm_year;
//	//    }
//	intTimeDate.tm_isdst = -1; // Is DST on? 1 = yes, 0 = no, -1 = unknown
//	clock = (INT32)mktime(&intTimeDate);
	txProtocolData->crc16 = CountCRC_16((char *)txProtocolData);
	memcpy((char *)txProtocolData->hander, (char *)pHander, 4);
	SendPackID++;
	return txProtocolData;
}

#if !((defined WIN32) || (defined LINUX))
void setMCUTime(struct tm &intTimeDate)
{
	rtccTimeDate meRtccTimeDate;
	RtccReadTimeDate(&meRtccTimeDate);
	intTimeDate.tm_hour = BCD2DEC(meRtccTimeDate.f.hour);
	intTimeDate.tm_min = BCD2DEC(meRtccTimeDate.f.min);
	intTimeDate.tm_sec = BCD2DEC(meRtccTimeDate.f.sec);
	intTimeDate.tm_mday = BCD2DEC(meRtccTimeDate.f.mday);
	intTimeDate.tm_mon = BCD2DEC(meRtccTimeDate.f.mon);
	intTimeDate.tm_year = BCD2DEC(meRtccTimeDate.f.year);
	intTimeDate.tm_wday = BCD2DEC(meRtccTimeDate.f.wday);
}
#endif // WIN32
