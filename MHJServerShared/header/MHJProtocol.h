/*
 * MHJProtocol.h
 *
 *  Created on: 2015-7-10
 *      Author: waroy
 */

#ifndef MHJPROTOCOL_H_
#define MHJPROTOCOL_H_

#include "Protocol.h"
#include "ProtocolTypeDefine.h"
#include "MHJDefine.h"
#include <spbuffer.hpp>
#include <boost/shared_ptr.hpp>
#include "MHJSecurityFactory.h"

using namespace boost;
class MHJSecurityFactory;

class MHJProtocol {
public:
	MHJProtocol(const void * inBuffer, int size, MHJSecurityFactory* security);
	MHJProtocol(MHJSecurityFactory* security);
	virtual ~MHJProtocol();
	/**
	 * 返回0长度不足，返回1正常，返回-1校验失败
	 */
	static WORD getDeviceWIFIProtocolSize(SP_Buffer * inBuffer = NULL);
	static bool isUseNetProtocol(void* inbuffer);
	static bool isUseNetProtocol(BYTE mdeviceType);
	boost::shared_ptr<SP_Buffer> getSendBuffer(BYTE mprotocolType, const MHJDeviceMark* pdeviceID, UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength,
			void *pdata);

	DWORD Time();

	BYTE protocolType();
	BYTE protocolID();
	MHJDeviceMark* device();
	uint32_t deviceID();
	WORD deviceIDSecurity();
	UINT16 packID();
	BYTE Ver();
	UINT16 length();

	BYTE* data();

	template<class T>
	T* data() {
		if (length() >= sizeof(T))
			return (T*) data();

		return NULL;
	}

	bool isValid();

	void createSessionSecurityData(char *outSecurity);

	//获取基础信息
	void* getDataBaseFactory();

	void* getDatabaseManage();
	void* getDatabaseManage(MHJDeviceMark* mark);

	void setReturnHandleClose();
	int getReturnHandle();

	void* getBuffer();
	uint16_t getBufferSize();
	MHJSecurityFactory * getSecurityFactory();
protected:
	char* mbuffer;
	uint16_t mbufferSize;
	MHJDeviceProtocol* mDeviceProtocol;
	MHJNETProtocol* mHostProtocol;
	MHJSecurityFactory * mSecurity;
	int mReturnHandle;
};

#endif /* MHJPROTOCOL_H_ */
