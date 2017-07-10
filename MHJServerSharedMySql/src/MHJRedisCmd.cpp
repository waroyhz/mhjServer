/*
 * MHJRedisCmd.cpp
 *
 *  Created on: 2015年11月4日
 *      Author: ruancongming
 */

#include "MHJRedisCmd.h"
#include <hiredis.h>
#include <string.h>
#include <iostream>

#define REDIS_CMD_SIZE  10240
using namespace std;
MHJRedisCmd::MHJRedisCmd(){

}

MHJRedisCmd::~MHJRedisCmd(){

}

redisReply* MHJRedisCmd::RedisCmd_AUTH(redisContext* context , char *passwd){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s", "AUTH ", passwd);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_GET(redisContext* context,char* key){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s", "GET ", key);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_DEL(redisContext* context, char* key) {
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s", "DEL ", key);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

int MHJRedisCmd::RedisCmd_SET(redisContext* context,char* key, char* value){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return 1;
//	}
		char formatcmd[REDIS_CMD_SIZE];
		memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
		sprintf(formatcmd, "%s  %s  %s", "SET ", key, value);
		redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
		return 0;

}

int  MHJRedisCmd::RedisCmd_RPUSH(redisContext* context,char* listname, char* value){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return 1;
//	}
		char formatcmd[REDIS_CMD_SIZE];
		memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
		sprintf(formatcmd, "%s  %s  %s", "RPUSH ", listname, value);
		redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
		return 0;

}

int  MHJRedisCmd::RedisCmd_LPUSH(redisContext* context,char* listname, char* value){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return 1;
//	}
		char formatcmd[REDIS_CMD_SIZE];
		memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
		sprintf(formatcmd, "%s  %s  %s", "LPUSH ", listname, value);
		redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
		return 0;

}

redisReply*  MHJRedisCmd::RedisCmd_LPOP(redisContext* context,char* listname){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
		char formatcmd[REDIS_CMD_SIZE];
		memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
		sprintf(formatcmd, "%s  %s", "LPOP ", listname);
		redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
		return reply;

}

redisReply* MHJRedisCmd::RedisCmd_LLEN(redisContext* context,char* listname){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s ", "LLEN ", listname);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_LINDEX(redisContext* context,char* listname,int index){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s  %d", "LINDEX ", listname, index);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_PUBLISH(redisContext* context,char* channel,char* message){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s  %s", "PUBLISH ", channel, message);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);

	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_PUBLISHASBINARY(redisContext* context,char* channel,size_t size1,char* message,size_t size2){
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
//#ifdef		DEBUG
//		std::cout << " #### publish cmd binary:  " << channel<< "  message:  " << message<<std::endl;
//#endif
	redisReply* reply = (redisReply*)redisCommand(context,"PUBLISH  %b  %b",channel, size1, message,size2);
	return reply;
}

void MHJRedisCmd::RedisCmd_FREEREPLY(redisReply* reply){
	if(reply!=NULL)
		freeReplyObject(reply);
}

redisReply* MHJRedisCmd::RedisCmd_LRANGE(redisContext* context,
		char* listname) {
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s  %d  %d", "LRANGE ", listname, 0,-1);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

redisReply* MHJRedisCmd::RedisCmd_LREMALL(redisContext* context, char* listname,
		char* value) {
//	if(context==NULL){
//#ifdef		DEBUG
//		std::cout << " #### NULL Pointer, Do nothing! " <<std::endl;
//#endif
//		return NULL;
//	}
	char formatcmd[REDIS_CMD_SIZE];
	memset(formatcmd, 0, REDIS_CMD_SIZE * sizeof(char));
	sprintf(formatcmd, "%s  %s  %d  %s", "LREM ", listname, 0,value);
	redisReply* reply = (redisReply*)redisCommand(context,formatcmd);
	return reply;
}

