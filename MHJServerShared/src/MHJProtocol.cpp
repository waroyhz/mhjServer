/*
 * MHJProtocol.cpp
 *
 *  Created on: 2015-7-10
 *      Author: waroy
 */

#include "MHJProtocol.h"
#include "ProtocolTypeDefine.h"
#include <string.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <time.h>

#include <stdio.h>

using namespace boost;

MHJProtocol::MHJProtocol(const void * inBuffer, int size, MHJSecurityFactory* security) :
		mReturnHandle(0) {
	mbuffer = new char[size];
	mbufferSize = size;
	memcpy(mbuffer, inBuffer, size);

	if (isUseNetProtocol (mbuffer)) {
		mDeviceProtocol = NULL;
		mHostProtocol = (MHJNETProtocol*) mbuffer;
	} else {
		mHostProtocol = NULL;
		mDeviceProtocol = (MHJDeviceProtocol*) mbuffer;
	}

	mSecurity = security;

//	mFuncTimeRecord = new MHJFunctionTimeRecorder(20);
//	recordCount = 0;
}

MHJProtocol::MHJProtocol(MHJSecurityFactory* security) :
		mReturnHandle(0), mbuffer(NULL), mbufferSize(NULL), mHostProtocol(NULL), mDeviceProtocol(NULL) {
	mSecurity = security;
//	recordCount = 0;
}

MHJProtocol::~MHJProtocol() {
	delete[] mbuffer;
}

//char* MHJProtocol::get_timeRecordbuffer(){
//	int i;
//	if(recordCount<10){
//		i=recordCount;
//	}else{
//		recordCount=0;
//		i=recordCount;
//	}
//	recordCount++;
//	return &timeRecordBuffer[i][0];
//}
WORD MHJProtocol::getDeviceWIFIProtocolSize(SP_Buffer * inBuffer) {
	if (inBuffer == NULL)
		return sizeof(MHJNETProtocol); //最小协议长度
	else if (inBuffer->getSize() < sizeof(MHJNETProtocol)) {
		return sizeof(MHJNETProtocol); //最小协议长度
	} else {
		MHJNETProtocol*p = (MHJNETProtocol*) inBuffer->getBuffer();

		uint ret = NetHanderCheck((char*) p, inBuffer->getSize());
		if (ret == MHJ_CHECK_RETURN_ERROR)
			return ret;
		else {
			WORD size = sizeof(MHJNETProtocol) + p->length;
			return size;
		}
//		if (isUseNetProtocol((void*) inBuffer->getBuffer())) {
//			MHJNETProtocol*p = (MHJNETProtocol*) inBuffer->getBuffer();
//			WORD size = sizeof(MHJNETProtocol) + p->length;
//			return size;
//		} else {
//			return sizeof(MHJDeviceProtocol);
//		}
//		return sizeof(MHJDeviceProtocol);
	}
}

bool MHJProtocol::isUseNetProtocol(void* inbuffer) {
	MHJDeviceProtocol*p = (MHJDeviceProtocol*) inbuffer;
	return isUseNetProtocol(p->deviceType);
}

bool MHJProtocol::isUseNetProtocol(BYTE mdeviceType) {
//	if (mdeviceType == MDT_HOST) //所有协议都是走变长包。
	return true;
//	else
//		return false;
}

boost::shared_ptr<SP_Buffer> MHJProtocol::getSendBuffer(BYTE mprotocolType, const MHJDeviceMark* pdevice, UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength,
		void *pdata) {
	struct tm *intTimeDate;
	time_t rawtime;
	time(&rawtime);
	intTimeDate = localtime(&rawtime);
	if (intTimeDate->tm_year <= 69) {
		intTimeDate->tm_year = intTimeDate->tm_year + 100;
	}
	INT32 clock = (INT32) mktime(intTimeDate);

	if (isUseNetProtocol(mprotocolType)) {
		char* SecurityData = mSecurity->getSecurity(pdevice, mprotocolID);
		if (0xffL>(ulong)SecurityData) {
			printf("***获取安全码错误 device %d error , SecurityData is NULL\n", pdevice->deviceID);
			boost::shared_ptr < SP_Buffer > protocolNull;
			return protocolNull;
		}
#ifdef DEBUG
		char tmpview [32 * 3];
		memset(tmpview, 0, sizeof(tmpview));
		for (int i = 0; i < 32; i++) {
			sprintf(&tmpview[i * 2], "%02x", ((BYTE*) SecurityData)[i]);
		}
		printf("send: id = %d, type = %d,mprotocolType %d, protocol = %d, security= %s\n",pdevice->deviceID,pdevice->deviceType,mprotocolType,mprotocolID,tmpview);
#endif
		MHJNETProtocol* protocol = NetProtocolPackage(mprotocolType, pdevice, mpackID, mprotocolID, mver, clock, mlength, pdata, SecurityData);
		boost::shared_ptr < SP_Buffer > buffer = boost::make_shared<SP_Buffer>();
		buffer->append(protocol, sizeof(MHJNETProtocol) + protocol->length);
		free(protocol);
		return buffer;
	} else {
//		MHJDeviceProtocol* senddata =
//				ProtocolPackage(mdeviceType, pdeviceID, mpackID, mprotocolID,
//						mver, clock, mlength, pdata, SecurityData);
		boost::shared_ptr < SP_Buffer > mbuffer = boost::make_shared<SP_Buffer>();
//		mbuffer->append(senddata, sizeof(MHJDeviceProtocol));
//		free(senddata);
		return mbuffer;
	}
}

DWORD MHJProtocol::Time(){
	if (mDeviceProtocol)
		return mDeviceProtocol->time;
	else
		return mHostProtocol->time;
}

BYTE MHJProtocol::protocolType() {
	if (mDeviceProtocol)
		return mDeviceProtocol->deviceType;
	else
		return mHostProtocol->protocolType;
}

BYTE MHJProtocol::protocolID() {
	if (mDeviceProtocol)
		return mDeviceProtocol->protocolID;
	else
		return mHostProtocol->protocolID;
}

MHJDeviceMark* MHJProtocol::device() {
	return &mHostProtocol->device;
}

uint32_t MHJProtocol::deviceID() {
	if (mDeviceProtocol)
		return *((uint32_t*) mDeviceProtocol->deviceID[1]);
	else
		return mHostProtocol->device.deviceID;
}

WORD MHJProtocol::deviceIDSecurity() {
	WORD ret = mHostProtocol->device.idSecurity1;
	ret = (ret << 8) + mHostProtocol->device.idSecurity2;
	return ret;
}

UINT16 MHJProtocol::packID() {
	if (mDeviceProtocol)
		return mDeviceProtocol->packID;
	else
		return mHostProtocol->packID;
}

BYTE MHJProtocol::Ver() {
	if (mDeviceProtocol)
		return mDeviceProtocol->ver;
	else
		return mHostProtocol->ver;
}

UINT16 MHJProtocol::length() {
	if (mDeviceProtocol)
		return mDeviceProtocol->length;
	else
		return mHostProtocol->length;
}

BYTE* MHJProtocol::data() {
	if (mDeviceProtocol)
		return mDeviceProtocol->data;
	else
		return (BYTE*) &mHostProtocol->data;
}

bool MHJProtocol::isValid() {
	char *mSecurityData = mSecurity->getSecurity(device(), protocolID());
//	printf("meuritydata\n");
//	for(int i=0;i<32;i++){
//		printf("%d\n", mSecurityData[i]);
//	}
#ifdef DEBUG
	if ((long)mSecurityData<0x200)
	{
		printf("安全码获取出错！安全码为空。 id:%d\n",device()->deviceID);
		mSecurityData=mSecurity->getSecurity(device(),protocolID());
		return false;
	}
#endif
#ifdef DEBUG
	char tmpview[32 * 3];
	memset(tmpview, 0, sizeof(tmpview));
	for (int i = 0; i < 32; i++) {
		sprintf(&tmpview[i * 2], "%02x", ((BYTE*) mSecurityData)[i]);
	}
	printf("isValid to id %d protocolType is %d protocol %d security:%s\n",deviceID(),protocolType(),protocolID(),tmpview);

#endif
	if (mDeviceProtocol)
		return AnalysisCheck((char*) mDeviceProtocol, mSecurityData);
	else {
		if (mbufferSize == sizeof(MHJNETProtocol) + length())
			return NetAnalysisCheck((char*) mHostProtocol, sizeof(MHJNETProtocol) + length(), mSecurityData);
		else
			return false;
	}
}

void MHJProtocol::createSessionSecurityData(char *outSecurity) {
	mSecurity->createSessionSecurity(device(), outSecurity);
}

void* MHJProtocol::getDataBaseFactory() {
	return mSecurity->getDataBaseFactory();
}

void* MHJProtocol::getDatabaseManage() {
//不同设备的设备类型管理由底层判断
	return mSecurity->getDatabaseManage(device());
}

void* MHJProtocol::getDatabaseManage(MHJDeviceMark* mark) {
//不同设备的设备类型管理由底层判断
	return mSecurity->getDatabaseManage(mark);
}

void MHJProtocol::setReturnHandleClose() {
	mReturnHandle = -1;
}

int MHJProtocol::getReturnHandle() {
	return mReturnHandle;
}

void* MHJProtocol::getBuffer() {
	return mbuffer;
}

uint16_t MHJProtocol::getBufferSize() {
	return mbufferSize;
}

MHJSecurityFactory* MHJProtocol::getSecurityFactory() {
	return mSecurity;
}
