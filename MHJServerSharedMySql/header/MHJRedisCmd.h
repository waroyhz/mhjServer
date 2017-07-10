/*
 * MHJRedisCmd.h
 *
 *  Created on: 2015年11月4日
 *      Author: ruancongming
 */

#ifndef MHJREDISCMD_H_
#define MHJREDISCMD_H_
#include <hiredis.h>
//Redis操作相关函数
class MHJRedisCmd{
public:
	MHJRedisCmd();
	~MHJRedisCmd();

	redisReply* RedisCmd_AUTH(redisContext* context , char *passwd);

	redisReply* RedisCmd_GET(redisContext* context,char* key);
	redisReply* RedisCmd_DEL(redisContext* context,char* key);
	int RedisCmd_SET(redisContext* context,char* key, char* value);

	int RedisCmd_RPUSH(redisContext* context,char* listname, char* value);
	int RedisCmd_LPUSH(redisContext* context,char* listname, char* value);
	redisReply* RedisCmd_LPOP(redisContext* context,char* listname);
	redisReply* RedisCmd_LLEN(redisContext* context,char* listname);
	redisReply* RedisCmd_LINDEX(redisContext* context,char* listname,int index);

	redisReply* RedisCmd_PUBLISH(redisContext* context,char* channel,char* message);
	redisReply* RedisCmd_PUBLISHASBINARY(redisContext* context,char* channel,size_t size1,char* message,size_t size2);
	redisReply* RedisCmd_LREMALL(redisContext* context,char* listname, char* value);
	void RedisCmd_FREEREPLY(redisReply*);

	redisReply* RedisCmd_LRANGE(redisContext* context,char* listname);

private:

};

#endif /* MHJREDISCMD_H_ */
