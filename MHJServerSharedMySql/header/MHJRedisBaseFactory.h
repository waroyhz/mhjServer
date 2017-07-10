#ifndef MHJRedisBaseFactory_H_
#define MHJRedisBaseFactory_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis.h>
#include "RedisQueueManage.h"
#include "MHJRedisCmd.h"

class MHJRedisBaseFactory{
public:
	MHJRedisBaseFactory(const char *ip, int port, const char *password);
	~MHJRedisBaseFactory();

	redisReply* RedisCommand_GET(char* key);
	redisReply* RedisCommand_DEL(char* key);
	redisReply* RedisCommand_SET(char* key, char* value);
	redisReply* RedisCommand_LRANGE(char* list);
	redisReply* RedisCommand_LLEN(char* list);
	redisReply* RedisCommand_LPOP(char* list);
	redisReply* RedisCommand_RPUSH(char* list, char* value);
	redisReply* RedisCommand_LPUSH(char* list, char* value);
	redisReply* RedisCommand_LINDEX(char* list,int index);
	redisReply* RedisCommand_PUBLISH(char* channel,char* message);
	redisReply* RedisCommand_PUBLISHASBINARY(char* channel,size_t size1,char* message, size_t size2);
	redisReply* RedisCommand_LREMOVEALL(char* list,char* value);

	void  RedisCommand_FREE(redisReply *reply);
	static MHJRedisBaseFactory* GetRedisFactory();

	char * getRedisPublisChannel();
	void setRedisPublisChannel(char *list);

	char * getTimeRecodlist();
	void setTimeRecodlist(char *list);

	char * getOnlineServerList();
	void setOnlineServerList(char *serverlist);

	char * getJavaBlpop();
	void setJavaBlpop(char *devicelist);

	char * getLinuxBlpop();
	void setLinuxBlpop(char *devicelist);

	char * getServiceID();
	void setServiceID(char *serviceId);

	bool isKeyInList(char* list, char* key);

	char * getReceiveDeviceInstructionList();
	void setReceiveDeviceInstructionList(char *receiveDeviceInstructionList);

	char * getReceiveDeviceInstructionIsSuccessList();
	void setReceiveDeviceInstructionIsSuccessList(char *receiveDeviceInstructionIsSuccessList);


	char * getDeviceMessageList();
	void setDeviceMessageList(char * deviceMessageList);

private:
	pthread_mutex_t *redismutex;
	RedisQueueManage *mredispool;

	MHJRedisCmd *rediscmd;
	static MHJRedisBaseFactory* reidsbasefactory;

	char* serverList;
	char* JavaBlpop; //Java服务器阻塞的队列
	char* LinuxBlpop; //Linux服务器阻塞队列
	char* TimeRecoderlist;
	char* ServiceID;
	char* channel;

	char * receiveDeviceInstructionList;
    char * receiveDeviceInstructionIsSuccessList;

	char * deviceMessage; //设备信息存储

};

#endif
