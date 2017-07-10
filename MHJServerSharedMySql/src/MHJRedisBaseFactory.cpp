/*
 * MHJRedisBaseFactory.cpp
 *
 *  Created on: 2015年9月22日
 *      Author: ruancongming
 */
#include "MHJRedisBaseFactory.h"
#include <iostream>
#include <assert.h>
using namespace std;

#define COMMAND_SIZE  128

MHJRedisBaseFactory* MHJRedisBaseFactory::reidsbasefactory = NULL;

MHJRedisBaseFactory::MHJRedisBaseFactory(const char *hostname, int port, const char *password) {
	redismutex = new pthread_mutex_t;
	pthread_mutex_init(redismutex, NULL);
	mredispool = new RedisQueueManage(hostname, port, password);
	rediscmd = new MHJRedisCmd();

	if (reidsbasefactory == NULL)
		reidsbasefactory = this;
	else {
		cout << "重复定义静态指针变量！" << endl;
	}

	this->channel = NULL;
	this->TimeRecoderlist = NULL;
	this->ServiceID = NULL;

}

MHJRedisBaseFactory::~MHJRedisBaseFactory() {
	pthread_mutex_destroy (redismutex);
	delete mredispool;
	delete rediscmd;
}

redisReply* MHJRedisBaseFactory::RedisCommand_GET(char* key) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = rediscmd->RedisCmd_GET(element->context, key);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}
redisReply* MHJRedisBaseFactory::RedisCommand_DEL(char* key) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = rediscmd->RedisCmd_DEL(element->context, key);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}
redisReply* MHJRedisBaseFactory::RedisCommand_SET(char* key, char* value) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_SET(element->context, key, value);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_LRANGE(char* list) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LRANGE(element->context, list);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}
	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_LPOP(char* list) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LPOP(element->context, list);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_LLEN(char* list) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LLEN(element->context, list);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_RPUSH(char* list, char* value) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_RPUSH(element->context, list, value);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_LPUSH(char* list, char* value) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LPUSH(element->context, list, value);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}

	return reply;
}
redisReply* MHJRedisBaseFactory::RedisCommand_LINDEX(char* list, int index) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LINDEX(element->context, list, index);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}
	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_PUBLISH(char* channel, char* message) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_PUBLISH(element->context, channel, message);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}
	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_PUBLISHASBINARY(char* channel, size_t size1, char* message, size_t size2) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_PUBLISHASBINARY(element->context, channel, size1, message, size2);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}
	return reply;
}

redisReply* MHJRedisBaseFactory::RedisCommand_LREMOVEALL(char* list, char* value) {
	redisReply* reply = NULL;
	QueueElement *element = NULL;

	element = mredispool->GetRedisContext();
	if (element != NULL) {
		reply = (redisReply*) rediscmd->RedisCmd_LREMALL(element->context, list, value);
		if(reply!=NULL)
			mredispool->SaveRedisContext(element);
		else
			mredispool->DeletRedisContext(element);
	}
	return reply;
}

void MHJRedisBaseFactory::RedisCommand_FREE(redisReply *reply) {
	rediscmd->RedisCmd_FREEREPLY(reply);
	reply = NULL;
}

MHJRedisBaseFactory* MHJRedisBaseFactory::GetRedisFactory() {
	return reidsbasefactory;
}

char* MHJRedisBaseFactory::getRedisPublisChannel() {
	return this->channel;
}

void MHJRedisBaseFactory::setRedisPublisChannel(char* channel) {
	this->channel = channel;
}

char* MHJRedisBaseFactory::getTimeRecodlist() {
	return TimeRecoderlist;
}

void MHJRedisBaseFactory::setTimeRecodlist(char* timerecoderlist) {
	TimeRecoderlist = timerecoderlist;
}

char* MHJRedisBaseFactory::getServiceID() {
	return ServiceID;
}

void MHJRedisBaseFactory::setServiceID(char* serviceId) {
	ServiceID = serviceId;
}

char* MHJRedisBaseFactory::getJavaBlpop() {
	return this->JavaBlpop;
}
void MHJRedisBaseFactory::setJavaBlpop(char* javablpop) {
	this->JavaBlpop = javablpop;
}

char* MHJRedisBaseFactory::getLinuxBlpop() {
	return this->LinuxBlpop;
}
void MHJRedisBaseFactory::setLinuxBlpop(char* linuxblpop) {
	this->LinuxBlpop = linuxblpop;
}

char* MHJRedisBaseFactory::getOnlineServerList() {
	return this->serverList;
}
void MHJRedisBaseFactory::setOnlineServerList(char* serverlist) {
	this->serverList = serverlist;
}

bool MHJRedisBaseFactory::isKeyInList(char* list, char* key) {
	bool ret = false;
	redisReply* reply = this->RedisCommand_LRANGE(list);
	if (reply) {
		for (unsigned int i = 0; i < reply->elements; i++) {
			if (strcmp(key, reply->element[i]->str) == 0) {
				ret = true;
				break;
			}
		}
		this->RedisCommand_FREE(reply);
	} else {
		printf("MHJRedisBaseFactory::isKeyInList  redis 服务不可用！！！！\n");
	}
	return ret;
}

char *  MHJRedisBaseFactory::getReceiveDeviceInstructionList(){
	return  this->receiveDeviceInstructionList;
}
void MHJRedisBaseFactory::setReceiveDeviceInstructionList(char *receiveDeviceInstructionList){
	this->receiveDeviceInstructionList = receiveDeviceInstructionList;
}

char * MHJRedisBaseFactory::getReceiveDeviceInstructionIsSuccessList(){
	return this->receiveDeviceInstructionIsSuccessList;
}
void MHJRedisBaseFactory::setReceiveDeviceInstructionIsSuccessList(char *receiveDeviceInstructionIsSuccessList){
	this->receiveDeviceInstructionIsSuccessList = receiveDeviceInstructionIsSuccessList;
}

char * MHJRedisBaseFactory::getDeviceMessageList(){
	return this->deviceMessage;
}
void MHJRedisBaseFactory::setDeviceMessageList(char * deviceMessageList){
	this->deviceMessage = deviceMessageList;
}