/*
 * MoDbQueueManage.h
 *
 *  Created on: 2015年10月9日
 *      Author: ruancongming
 */

#ifndef REDISQUEUEMANAGE_H_
#define REDISQUEUEMANAGE_H_
#include <hiredis.h>
#include "sputils.hpp"
#include "QueueNode.h"

class RedisQueueManage{
public:
	RedisQueueManage(const char *ip,unsigned int port,const char *password);
	~RedisQueueManage();

	static void* RedisLoop(void *arg);

	QueueElement * GetRedisContext();
	QueueElement * CreatRedisContext();
	void SaveRedisContext(QueueElement *  element);
	void DeletRedisContext(QueueElement *  element);
	void ClearRedis();
	void RedisContextAuth();

	QueueElement *GetFromQueue();
	void QueueTopAlive();

private:
	pthread_mutex_t *queuelock;
	const char *mhostname;
	unsigned int mport;
	const char *mpassword;
	SP_CircleQueue * queue;
	int queuecount;
	int queuemax;
	int timeout;

};

#endif /* REDISQUEUEMANAGE_H_ */
