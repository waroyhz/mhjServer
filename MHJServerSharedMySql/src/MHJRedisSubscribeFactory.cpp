/*
 * MHJRedisSubscribeFactory.cpp
 *
 *  Created on: 2015年11月13日
 *      Author: ruancongming
 */
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MHJRedisSubscribeFactory.h"
#include "MHJRedisBaseFactory.h"
#include <hiredis/async.h>
#include "ProtocolServer.h"
#include <pthread.h>
#include "MHJRedisBlpop.h"

#define SUBCMDBUFF_SIZE  256
using namespace boost;

void redisSubscribeCallback(redisAsyncContext* c, void* ret, void* arg) {
	MHJRedisSubscribeFactory* subscribe = (MHJRedisSubscribeFactory*) arg;

	redisReply *reply = (redisReply *) ret;
	if (reply == NULL) {
#ifdef DEBUG
		std::cout << "Redis订阅返回为空！"<<std::endl;
#endif
		return;
	}
	//Redis订阅回调字符串内容：elements[0]: message,  elements[1]: channel, elements[2]: message
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
		if (strcmp(reply->element[0]->str, subscribe->getCmd()) != 0) {
#ifdef DEBUG
//			   std::cout<< "线程："<<pthread_self() << "收到Redis订阅返回:"<<std::endl;
//			   std::cout << "通道 "<<reply->element[1]->str<<std::endl;
//			   std::cout <<"收到订阅消息 "<< reply->element[2]->str <<std::endl;
//			   std::cout <<"大小 "<<reply->element[2]->len  <<std::endl;
//			   std::cout <<"类别 "<< reply->element[0]->str<<std::endl;
#endif
			std::cout << "收到订阅数据" << reply->element[2]->str << std::endl;
			char *ptr = reply->element[2]->str;
			int reciveSize = reply->element[2]->len / 2;
			uint8_t* recive = new uint8_t[reciveSize];

			char tmp[3];
			tmp[2] = '\0';
			uint8_t tmpU8;
			for (int i = 0; i < reciveSize; i++) {
				memcpy(tmp, &ptr[i * 2], 2);
				sscanf(tmp, "%x", &tmpU8);
				recive[i] = tmpU8;
			}

			MHJSUBSCRIBEProtocol *subscribeptotocol = (MHJSUBSCRIBEProtocol *) recive;

			shared_ptr < SP_Buffer > data = make_shared<SP_Buffer>();
			data->append(((char*) (&subscribeptotocol->data)), reciveSize - (sizeof(MHJSUBSCRIBEProtocol) - sizeof(BYTE)));

			MHJRedisSubscribeProcessFactory* process = subscribe->getProcess();
			switch (subscribeptotocol->type) {
//			   	   case MSPT_LOGIN:
//					   process->ProcessLogIn(subscribeptotocol->source,data);
//					   break;
//				   case MSPT_HEARTBEAT:
//					   process->ProcessHeartBeat(subscribeptotocol->source,data);
//					   break;
			case MSPT_SERVERINFO:
				process->ProcessServerInfo(subscribeptotocol->source, data);
				break;
			case MSPT_CMDTRANSFER:
//					   printf("收到的转发命令\n");
//					   for(int i =0; i<reciveSize;i++){
//						   printf("%02x",recive[i]);
//					   	}
				process->ProcessCommand(subscribeptotocol->source, data);
				break;
			case MSPT_ONLINEINFORM:
				process->ProcessOnlineInform(subscribeptotocol->source, data); //&subscribeptotocol->device
				break;
			}

			delete[] (uint8_t*) recive;
//				freeReplyObject(reply);
		}
	}

}

void * loop_subscribe(void* arg) {
	MHJRedisSubscribeFactory* subscribe = (MHJRedisSubscribeFactory*) arg;
	subscribe->DoSubscribe();
	return NULL;
}

void asyncAuthCallback(redisAsyncContext* c, void* ret, void* arg) {
	redisReply *reply = (redisReply *) ret;
	if (reply != NULL) {
		std::cout << "Redis认证通过！" << std::endl;
	} else {
		std::cout << "Redis认证失败,Redis连接密码错误！" << std::endl;
	}
}

void * loop_blpop(void* arg) {
	MHJRedisBlpop* blpop = (MHJRedisBlpop*) arg;
	blpop->doBlpop();
	return NULL;
}

MHJRedisSubscribeFactory::MHJRedisSubscribeFactory(char* ip, int port, char* passwd, char* channel, MHJRedisSubscribeProcessFactory* process) {
	eventBase = event_base_new();
	asyncContext = redisAsyncConnect(ip, port);
	redisCmd = "subscribe";
	this->channel = channel;
	this->process = process;
	this->passwd = passwd;
#ifdef DEBUG
	std::cout << "Redis订阅, IP=" <<ip << "  Port="<<port<<std::endl;
#endif
	pthread_t subscribe_t;
	pthread_create(&subscribe_t, NULL, loop_subscribe, this);

	MHJRedisBlpop* blpop = new MHJRedisBlpop(ip, port, passwd, this);
	pthread_t blpop_t;
	pthread_create(&blpop_t, NULL, loop_blpop, blpop);
}

MHJRedisSubscribeFactory::~MHJRedisSubscribeFactory() {
	// TODO    eventBase  asyncContext  如何释放
}

void MHJRedisSubscribeFactory::DoSubscribe() {
	redisLibeventAttach(asyncContext, eventBase);
	auth(this->passwd);
	subscribe(this->channel);
	event_base_dispatch (eventBase);
}

void MHJRedisSubscribeFactory::auth(char* passwd) {
	char formatcmd[SUBCMDBUFF_SIZE];
	sprintf(formatcmd, "%s  %s", "AUTH ", passwd);
	redisAsyncCommand(asyncContext, asyncAuthCallback, (void*) this, formatcmd);
}

void MHJRedisSubscribeFactory::subscribe(char* channel) {
	char formatedCmd[SUBCMDBUFF_SIZE];
	sprintf(formatedCmd, "%s %s", "subscribe", channel);
#ifdef DEBUG
	std::cout << "Redis订阅，向通道 "<<channel << "订阅消息 " <<std::endl;
#endif
	redisAsyncCommand(asyncContext, redisSubscribeCallback, (void*) this, formatedCmd);
}

void MHJRedisSubscribeFactory::publish(BYTE type, char* serverId, MHJDeviceMark* deviceMark, WORD length, void* data, char* channel) {

	shared_ptr < SP_Buffer > buffer = RedisSubscribeProtocolPackage(type, serverId, deviceMark, length, (void*) data);
	uint8_t* ptr = (uint8_t*) buffer->getRawBuffer();
	char* sent = new char[buffer->getSize() * 2 + 1];

	for (unsigned int i = 0; i < buffer->getSize(); i++) {
		sprintf(&sent[i * 2], "%02x", ptr[i]);
//		if(type ==4){
//		printf("%02x",ptr[i]);
//		}
	}
	sent[buffer->getSize() * 2] = '\0'; //Redis是以命令行的形式发送字符串的，因而在结尾添加字符串结束符

	MHJRedisBaseFactory* redisbase = MHJRedisBaseFactory::GetRedisFactory();
	redisReply* reply = redisbase->RedisCommand_PUBLISH(channel, sent);
	if (reply != NULL) {
		redisbase->RedisCommand_FREE(reply);
#ifdef DEBUG
		if(type ==4) {
			std::cout << "Redis发布Transfer，向通道 "<<channel << "发布消息 "<< sent <<std::endl;
		}
#endif
	} else {
//#ifdef DEBUG
		std::cout << "Redis发布，向通道 " << channel << "发布消息失败 " << std::endl;
//#endif
	}
	delete[] (char*) sent;
}

boost::shared_ptr<SP_Buffer> MHJRedisSubscribeFactory::RedisSubscribeProtocolPackage(BYTE mprotocolType, char* source, MHJDeviceMark* pdevice, WORD mlength,
		void* pdata) {

	WORD size = sizeof(MHJSUBSCRIBEProtocol) + mlength;

	MHJSUBSCRIBEProtocol* txProtocolData = (MHJSUBSCRIBEProtocol*) new char[size];
	memset((char*) txProtocolData, 0, size);

	txProtocolData->type = mprotocolType;
	if (source != NULL)
		memcpy((char*) &txProtocolData->source, source, sizeof(txProtocolData->source));
	if (pdevice != NULL)
		memcpy((char*) &txProtocolData->srcdevice, (char*) pdevice, sizeof(txProtocolData->srcdevice));
	if (mlength > 0)
		memcpy(&txProtocolData->data, pdata, mlength);

	shared_ptr < SP_Buffer > mbuffer = make_shared<SP_Buffer>();
	mbuffer->append(txProtocolData, size);

	delete[] (char*) txProtocolData;
	return mbuffer;
}

MHJRedisSubscribeProcessFactory* MHJRedisSubscribeFactory::getProcess() {
	return process;
}

char* MHJRedisSubscribeFactory::getCmd() {
	return redisCmd;
}

void MHJRedisSubscribeFactory::setChannel(char* channels) {
	if (channels != NULL)
		channel = channels;
}
