/*
 * ProtocolTypeDefine.c
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#include "ProtocolTypeDefine.h"

#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR
#endif
#ifdef LINUX
#include <stdio.h>
#else

#endif

#ifndef ESP_DEBUG
#define ESP_DEBUG
#endif

#ifdef ESP_DEBUG
#define ESP_DBG  //os_printf_plus
#else
#define ESP_DBG
#endif

//#define PRINTF_INFO

const char pNetHander[] = { "$MHJ" };

//void testviewHex(void* txProtocolData, int size) {
//	char tmpview [size * 2+1];
//	memset(tmpview, 0, sizeof(tmpview));
//	//			memset(tmpview, txProtocolData, size);
//	UINT16 i;
//	for (i = 0; i < size; i++) {
//		sprintf(&tmpview[i * 2], "%02x", ((BYTE*) txProtocolData)[i]);
//	}
//	printf(tmpview);
//}

uint ICACHE_FLASH_ATTR NetHanderCheck(char *pdata, int size) {
#ifdef PRINTF_INFO
	//ESP_DBG("*****NetAnalysisCheck \n");
	printf("接收数据包:");
	testviewHex(pdata,size);
	printf("\n");
	printf("安全码:");
	testviewHex(SecurityData,32);
	printf("\n");
#endif

	if (size < sizeof(MHJNETProtocol)) {
		return MHJ_CHECK_RETURN_MORE_DATA; //数据长度不足
	}
	char* CurrentData = (char*) malloc(sizeof(MHJNETProtocol));
	memcpy(CurrentData, (const void*) pdata, sizeof(MHJNETProtocol));
	MHJNETProtocol * rxProtocolData = (MHJNETProtocol*) CurrentData;

	WORD handerCrc16 = NetCountCRC_16((char *) &rxProtocolData->protocolType,
			sizeof(MHJNETProtocol) - sizeof(rxProtocolData->hander) - sizeof(rxProtocolData->data) - sizeof(rxProtocolData->handerCRC));

	if (handerCrc16 == rxProtocolData->handerCRC) {
		free(CurrentData);
		return MHJ_CHECK_RETURN_SUCCESS; //校验通过
	} else {
		free(CurrentData);
		return MHJ_CHECK_RETURN_ERROR; //校验失败
	}
}

/*********************************************************************
 * 校验数据 返回 真假
 * 数据转存
 ********************************************************************/

uint ICACHE_FLASH_ATTR NetAnalysisCheck(char *pdata, int size, const char*SecurityData) {
#ifdef PRINTF_INFO
	//ESP_DBG("*****NetAnalysisCheck \n");
	printf("接收数据包:");
	testviewHex(pdata,size);
	printf("\n");
	printf("安全码:");
	testviewHex(SecurityData,32);
	printf("\n");
#endif

	char* CurrentData = (char*) malloc(size);
	memcpy(CurrentData, (const void*) pdata, size);
	MHJNETProtocol * rxProtocolData = (MHJNETProtocol*) CurrentData;

	WORD handerCrc16 = NetCountCRC_16((char *) &rxProtocolData->protocolType,
			sizeof(MHJNETProtocol) - sizeof(rxProtocolData->hander) - sizeof(rxProtocolData->data) - sizeof(rxProtocolData->handerCRC));
	if (handerCrc16 != rxProtocolData->handerCRC) {
		free(CurrentData);
		return MHJ_CHECK_RETURN_ERROR;
	}
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

	int i = 0;

	if (rxProtocolData->length > 0) {
		BYTE* dataptr = (BYTE*) &rxProtocolData->data;
		uint32_t* intHander = (uint32_t*) &rxProtocolData->hander;
		UINT16 sindex = *intHander % 32;
		for (i = 0; i < rxProtocolData->length; i++) {
			dataptr[i] = dataptr[i] ^ SecurityData[sindex];
			sindex++;
			if (sindex >= 32)
				sindex = 0;
		}
	}
#ifdef PRINTF_INFO
	printf("CRC数据:");
	testviewHex(CurrentData,size);
	printf("\n");
#endif
	CRC16Data = NetCountCRC_16(CurrentData, size - sizeof(rxProtocolData->length));
	// rxProtocolData->crc16 = 0x1402;
//	delete[] CurrentData;

	BYTE*pbytecrc16 = (BYTE*) &rxProtocolData->data + rxProtocolData->length;
	WORD pwcrc16; //=(WORD*)pbytecrc16;
	memcpy(&pwcrc16, pbytecrc16, sizeof(WORD));

	if (CRC16Data == pwcrc16) {
		//将解密的数据区写入投入的数据区
		MHJNETProtocol * outProtocolData = (MHJNETProtocol*) pdata;
		memcpy(&outProtocolData->data, &rxProtocolData->data, rxProtocolData->length);
		free(CurrentData);
		return MHJ_CHECK_RETURN_SUCCESS;
	} else {
		free(CurrentData);
		return MHJ_CHECK_RETURN_ERROR;
	}
}

UINT16 ICACHE_FLASH_ATTR NetCountCRC_16(char *pdata, int size) {
	UINT16 i, j;
	UINT16 wCRC;
	unsigned char byData;
	unsigned char cy1, cy2;
	//CRC-CCITT
	wCRC = 0x00;
	for (i = 0; i < size; i++) {
		byData = *pdata++;
		for (j = 0; j < 8; j++) {
			cy1 = (wCRC & 0x8000) ? 1 : 0;
			cy2 = (byData & 0x80) ? 1 : 0;
			wCRC <<= 1;
			byData <<= 1;
			if (cy1 ^ cy2) {
				wCRC ^= 0x1021;
				wCRC |= 0x0001;
			}
		}
	}
	return wCRC;
}

MHJNETProtocol* ICACHE_FLASH_ATTR NetProtocolPackage(BYTE mprotocolType, const MHJDeviceMark* pdevice, INT16 mpackID, BYTE mprotocolID, BYTE mver, DWORD mtime, WORD mlength,
		void *pdata, const char*SecurityData) {

	WORD size = sizeof(MHJNETProtocol) + mlength;
	ESP_DBG("*****size value :%d \n", size);

	MHJNETProtocol* txProtocolData = (MHJNETProtocol*) malloc(size);
	ESP_DBG("*****malloc \n");
	memset((char*) txProtocolData, 0, size);
	txProtocolData->protocolType = mprotocolType;
	memcpy((char*) &txProtocolData->device, (char*) pdevice, sizeof(txProtocolData->device));
	txProtocolData->packID = mpackID;
	txProtocolData->protocolID = mprotocolID;
	txProtocolData->ver = mver;
	txProtocolData->time = mtime;
	txProtocolData->length = mlength;
	if (mlength > 0)
		memcpy(&txProtocolData->data, pdata, mlength);

	ESP_DBG("*****SecurityData \n");
	unsigned char KeysA, KeysB;
	KeysA = txProtocolData->packID & 0x000F;
	KeysB = ((txProtocolData->packID >> 4) & 0x000F) + 16;
	txProtocolData->hander[0] = SecurityData[KeysA];
	txProtocolData->hander[1] = SecurityData[KeysB];
	KeysA = SecurityData[KeysA] & 0x0F;
	KeysB = (SecurityData[KeysB] & 0x0F) + 16;
	txProtocolData->hander[2] = SecurityData[KeysA];
	txProtocolData->hander[3] = SecurityData[KeysB];

	WORD handerCrc16 = NetCountCRC_16((char *) &txProtocolData->protocolType,
			sizeof(MHJNETProtocol) - sizeof(txProtocolData->hander) - sizeof(txProtocolData->data) - sizeof(txProtocolData->handerCRC));
	txProtocolData->handerCRC = handerCrc16;

	ESP_DBG("*****NetCountCRC_16 \n");
#ifdef PRINTF_INFO
	printf("CRC数据:");
	testviewHex(txProtocolData,size);
	printf("\n");
#endif
	WORD crc16 = NetCountCRC_16((char *) txProtocolData, sizeof(MHJNETProtocol) + mlength - sizeof(txProtocolData->data));
	ESP_DBG("*****crc16 value %d: \n", crc16);
	char* pBcrc16 = (char*) txProtocolData + sizeof(MHJNETProtocol) + mlength - sizeof(txProtocolData->data);
//	ESP_DBG("*****pcrc16 %d\n",(int)pBcrc16);
//	WORD *pcrc16= (WORD*)pBcrc16;
	ESP_DBG("*****pcrc16 memcpy\n");
	memcpy(pBcrc16, &crc16, sizeof(WORD));
//	*pcrc16=crc16;
	ESP_DBG("*****memcpy \n");

	//数据区的加密
	int i = 0;
	if (txProtocolData->length > 0) {
		BYTE* dataptr = (BYTE*) &txProtocolData->data;
		uint32_t* intHander = (uint32_t*) &txProtocolData->hander;
		UINT16 sindex = *intHander % 32;
		for (i = 0; i < txProtocolData->length; i++) {
			dataptr[i] = dataptr[i] ^ SecurityData[sindex];
			sindex++;
			if (sindex >= 32)
				sindex = 0;
		}
	}

	memcpy((char *) txProtocolData->hander, (char *) pNetHander, 4);
#ifdef PRINTF_INFO
	printf("发送数据包");
	testviewHex(txProtocolData, sizeof(MHJNETProtocol) + mlength);
	printf("\n");
	printf("安全码:");
	testviewHex(SecurityData,32);
	printf("\n");
#endif

	ESP_DBG("*****return \n");
	return txProtocolData;
}

void ICACHE_FLASH_ATTR getGeneralSecurity(char* outgeneralsecuritydata) {
	int i;
	for (i = 0; i < SECURITYSIZE; i++) {
		outgeneralsecuritydata[i] = i + 1;
	}
}

void ICACHE_FLASH_ATTR sprintMark(char* strMark, MHJDeviceMark* mark) {
	uint8_t* devicemark = (uint8_t*) mark;
	sprintf(strMark, "%02x%02x%02x%02x%02x%02x%02x%02x", devicemark[0], devicemark[1], devicemark[2], devicemark[3], devicemark[4], devicemark[5],
			devicemark[6], devicemark[7]);
}
#ifdef LINUX
void sMarkprint(MHJDeviceMark* mark, char* strMark) {
	char tmp[32];
	tmp[0] = '0';
	tmp[1] = 'x';
	tmp[4] = '\0';
	BYTE* bmark = (BYTE*) mark;
	memset(bmark, 0, 8);
	int i;
	for (i = 0; i < 8; i++) {
		//		sprintf(tmp,"0x%s%s",deviceInformation[i],deviceInformation[i+1]);
		tmp[2] = strMark[i * 2];
		tmp[3] = strMark[i * 2 + 1];
		int nvalue = 0;
		sscanf(tmp, "%x", &nvalue);

		BYTE nb = nvalue;
		bmark[i] = nb;
	}
}
#endif
