/*
 * MHJMsgDecoder.cpp
 *
 *  Created on: 2015年7月1日
 *      Author: waroy
 */

#include "MHJMsgDecoder.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "spbuffer.hpp"
#include "sputils.hpp"
#include "Protocol.h"
#include "QueueData.h"
#include <iostream>

//-------------------------------------------------------------------
const char * nopackheader = "没有包头";
const char * errMessage = "未知多余数据?";
const char pHander[] = { "$MHJ" };

MHJ_Device_MsgDecoder::MHJ_Device_MsgDecoder(MHJSecurityFactory *pSecurity) {
	mQueue = new SP_CircleQueue();
	mSecurity = pSecurity;
}

MHJ_Device_MsgDecoder::~MHJ_Device_MsgDecoder() {
	for (; NULL != mQueue->top();) {
		free((void*) mQueue->pop());
	}

	delete mQueue;
	mQueue = NULL;

	mSecurity = NULL;
}

int MHJ_Device_MsgDecoder::decode(SP_Buffer * inBuffer) {
	int ret = eMoreData; /** 1 **/
	std::cout << "收到数据 缓冲区大小：" << inBuffer->getSize() << std::endl;
	while (inBuffer->getSize() >= MHJProtocol::getDeviceWIFIProtocolSize()) /* PACKHEADSIZE：包头到数据长度的总长度 */
	{
		const char * pos = NULL; // (char*)inBuffer->find(&MsgHead , 2 );

		long buffervoid = (long) inBuffer->getBuffer(); /* 记录得到的数据 */

		for (int i = 0; i < (int) inBuffer->getSize() - (int) sizeof(DWORD); i++) /* WORD ：unsigned short 两个字节 */
		{
			if (*((DWORD*) (buffervoid + i)) == *((DWORD*) pHander)) {
				pos = (const char*) (buffervoid + i);
				break;
			}
		}

		if (NULL == pos) {
			QueueData * qd = new QueueData();
			qd->isValid = false;
			qd->errMessage = (char*) nopackheader;
			//			qd->Data =new char[inBuffer->getSize()];
			qd->Data = make_shared < MHJProtocol > (inBuffer->getBuffer(), inBuffer->getSize(), mSecurity);
			qd->size = inBuffer->getSize();
			//			memcpy(qd->Data.get(), inBuffer->getBuffer(),inBuffer->getSize());

			inBuffer->erase(inBuffer->getSize()); //丢弃数据

			mQueue->push(qd);
			ret = eOK;
			break;
		} else {
			int removeSize = ((long) pos) - (long) inBuffer->getBuffer();
			if (removeSize > 0) {

				QueueData * qd = new QueueData();
				qd->isValid = false;
				qd->errMessage = (char *) errMessage;
				qd->Data = make_shared < MHJProtocol > (inBuffer->getBuffer(), removeSize, mSecurity);
				qd->size = removeSize;
				mQueue->push(qd);
				ret = eOK;
				inBuffer->erase(removeSize);
			}
			//			MHJProtocol* netpackhead =(MHJProtocol*) pos;
			if (inBuffer->getSize() < sizeof(MHJNETProtocol)) {
				break;
			}
			u_int16_t waitSize = MHJProtocol::getDeviceWIFIProtocolSize(inBuffer);
			if (waitSize == MHJ_CHECK_RETURN_ERROR) {
//长度校验失败
				removeSize = sizeof(MHJNETProtocol);
				QueueData * qd = new QueueData();
				qd->isValid = false;
				qd->errMessage = (char *) errMessage;
				qd->Data = make_shared < MHJProtocol > (inBuffer->getBuffer(), removeSize, mSecurity);
				qd->size = removeSize;
				mQueue->push(qd);
				ret = eOK;
				inBuffer->erase(removeSize);
			} else if (inBuffer->getSize() >= waitSize) {
				int datasize = MHJProtocol::getDeviceWIFIProtocolSize(inBuffer);
				QueueData * qd = new QueueData();
				qd->isValid = false; /* 初始化 */
				qd->Data = make_shared < MHJProtocol > (inBuffer->getBuffer(), datasize, mSecurity);
				qd->isValid = qd->Data->isValid(); //AnalysisCheck((char*)qd->Data->data(),datasize);
				inBuffer->erase(datasize);
				mQueue->push(qd);
				ret = eOK;
			} else {
				break;
			}
		}
	}
	return ret;
}

//const char * MHJ_Device_MsgDecoder :: getMsg()
//{
//	return mLine;
//}

SP_CircleQueue * MHJ_Device_MsgDecoder::getQueue() {
	return mQueue;
}

