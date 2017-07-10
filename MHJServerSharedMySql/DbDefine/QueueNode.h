/*
 * QueueNode.h
 *
 *  Created on: 2015年10月9日
 *      Author: ruancongming
 */

#ifndef QUEUENODE_H_
#define QUEUENODE_H_

#include <hiredis.h>

typedef struct {
	redisContext  *context;
	time_t Pingtime;
}QueueElement;

//typedef struct QNode{
//	void *pdata;
//	struct QNode *next;
//}QNode, *QueuePtr;
//
//typedef struct{
//	QueuePtr head;
//	QueuePtr rear;
//}LinkQueue;


#endif /* QUEUENODE_H_ */
