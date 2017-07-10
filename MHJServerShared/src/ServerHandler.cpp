/*
 * DeviceHandler.cpp
 *
 *  Created on: 2015-7-6
 *      Author: waroy
 */

#include "ServerHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <string.h>
#include "spbuffer.hpp"

//#include "spmsgdecoder.hpp"
#include "spporting.hpp"
#include "sprequest.hpp"
#include "spresponse.hpp"

#include "sputils.hpp"
#include "MHJMsgDecoder.h"
#include "Protocol.h"

#include "ProcessFactory.h"
#include "MHJSecurityFactory.h"
#include <iostream>


#include "MHJFuncTimeRecordMap.h"

#include "../../MHJDeviceServer/header/DeviceProcess/MhjDeviceServerHander.h"

//#include "MHJRedisBaseFactory.h"
//#include "ProcessServerFactory.h"

pthread_key_t pthread_key;
pthread_once_t once = PTHREAD_ONCE_INIT;
void once_routine(void) {
	int status;

	status = pthread_key_create(&pthread_key, NULL);
//        printf("Initializing key, thread num %x, %d\n", pthread_self(), key);
	if (status != 0) {
		printf("pthread_key_create failed \n");
	}
}

MHJServerHandler::MHJServerHandler(SP_OnlineManager* mOnlineManager, ProcessFactory *processFactory, MHJSecurityFactory *pSecurity) {
	this->mOnlineManager = mOnlineManager;
	mprocessFactory = processFactory;
	mSecurity = pSecurity;
}

MHJServerHandler::~MHJServerHandler() {

}

inline int MHJServerHandler::start(SP_Request *request, SP_Response *response) {
	{

		request->setMsgDecoder(new MHJ_Device_MsgDecoder(mSecurity));
		response->getReply()->getMsg()->append(
				"Welcome to MHJ server, please send Login security number, nor this connection will disconnect in 60 seconds.\r\n");

		mSid = response->getFromSid();
		char buffer[128] = { 0 };
		snprintf(buffer, sizeof(buffer), "Welcome %d to MHJ Device server, enter 'quit' to quit.\r\n", mChatID);
		response->getReply()->getMsg()->append(buffer);
		//		 	response->getReply()->setCompletionKey( ++mMsgSeq );

		SP_OnlineInfo_t * info = mOnlineManager->newInfo();
		info->mSid = mSid;
//		info->spResponse = response;  // pointer to a response
		//			info->mChatID = mChatID;
		//			info->mSocket=mSocket;

		strcpy(info->mClientIP, request->getClientIP());
		info->mClientPort = request->getClientPort();

		//		QueueData * qd = new QueueData;
		//		qd->clientSid=response->getFromSid();
		mChatID++;

		return 0;
	}
}

inline int MHJServerHandler::handle(SP_Request *request, SP_Response *response) {
	//		SP_MultiLineMsgDecoder * decoder = (SP_MultiLineMsgDecoder*)request->getMsgDecoder();
	MHJ_Device_MsgDecoder * decoder = (MHJ_Device_MsgDecoder*) request->getMsgDecoder();
	SP_CircleQueue * queue = decoder->getQueue();
	int ret = 0;
	mOnlineManager->setClientReceiveTime(response->getFromSid());

#ifdef _FUNCTION_TIME_RECORD
	setkey(1024*sizeof(FunctionTimeRecord)); //为该线程设置私有数据空间, 在获取key值关联的内存之前，先设置，确保本线程的key有效
	FunctionTimeRecord* memptr = (FunctionTimeRecord*)pthread_getspecific(pthread_key);
	MHJFunctionTimeRecorder *recoder=new MHJFunctionTimeRecorder(memptr,1024);
#endif

	while (queue->top()) {

		QueueData * qd = (QueueData *) queue->pop();
		shared_ptr < MHJProtocol > reviceProtocolData = qd->Data;
		/* 数据校验: ture/fail */
		if (qd->isValid == true) {

			//用于记录函数执行开始和结束的时间
			struct timeval timer_start;
			struct timeval timer_finish;
			ProcessFactory *process = mprocessFactory->getProcess(request, response, reviceProtocolData);
			gettimeofday(&timer_start, NULL);
			process->Process();
			gettimeofday(&timer_finish, NULL);

			int sec, microsec;
			sec = timer_finish.tv_sec - timer_start.tv_sec;
			if (timer_finish.tv_usec > timer_start.tv_usec) {
				microsec = timer_finish.tv_usec - timer_start.tv_usec;
			} else {
				microsec = timer_finish.tv_usec + 1000000 - timer_start.tv_usec;
				sec = sec > 0 ? sec - 1 : 0;
			}
			snprintf(recoder->getCurrent()->record, sizeof(struct FunctionTimeRecord),
					"{\"Time\":%d,\"funName\":\"%s\",\"Type\":%d,\"ID\":%d,\"seconds\":%d,\"useconds\":%d}", timer_start.tv_sec, "ProcessFactory::Process",
					reviceProtocolData->protocolType(), reviceProtocolData->protocolID(), sec, microsec);

			//直接传输对象的指针到release
			process->Release((void*) recoder);
			recoder->MHJfunctimeRecoder_clear();

			ret = reviceProtocolData->getReturnHandle();
		} else {
			std::cout << "协议错误！" << std::endl;
#ifdef DEBUG
			response->getReply()->getMsg()->append("protocol error!\n", strlen("protocol error!\n"));
#else
			ret = -1;
#endif
			break;
		}

		//		delete[] qd->Data;
		/* 释放数组类型的内存 */
		delete qd;
		/* 释放的先后顺序 */
	}
#ifdef _FUNCTION_TIME_RECORD
	delete recoder;
#endif
	return ret;
}

void MHJServerHandler::timeout(SP_Response *response) {
	std::cout << "MHJServerHandler::timeout" << std::endl;
    //获取离线的设备的deviceId 和 deviceType
	MHJDeviceMark *offIneDeviceMark = mOnlineManager->getDeviceMark(mSid);
	//给MhjdeviceServer使用，为了MhjDispatch编译通过，在MhjDispatch中也要实现MhjDeviceServerHander
    MhjDeviceServerHander mhjDeviceServerHander(offIneDeviceMark->deviceID,offIneDeviceMark->deviceType);

    mprocessFactory->clientClose(mSid);
	mOnlineManager->remove(mSid);
}

void MHJServerHandler::close() {
	mprocessFactory->clientClose(mSid);
	mOnlineManager->remove(mSid);
}

void MHJServerHandler::error(SP_Response *response) {
	std::cout << "MHJServerHandler::error" << std::endl;
	mprocessFactory->clientClose(mSid);
	mOnlineManager->remove(mSid);
}

void MHJServerHandler::setkey(int size) {

	int status = pthread_once(&once, once_routine);
	if (status != 0) {
//         std::cout<<" Setting 'phread_once' failed, Thread ID= " << pthread_self()<<std::endl;
	} else {
		char *Status = (char *) pthread_getspecific(pthread_key);
		if (Status == NULL) {
//			std::cout << "Alloc Memory for Thread " << pthread_self() << std::endl;

			status = pthread_setspecific(pthread_key, calloc(size, sizeof(char)));
			if (status != 0) {
//				std::cout << " Setting key failed on Thread " << pthread_self() << std::endl;
			}
		} else {
//    		std::cout<<"Key already setted, THread ID= "<<pthread_self()<<std::endl;
		}

	}
}

