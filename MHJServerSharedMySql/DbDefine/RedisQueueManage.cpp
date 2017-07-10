/*
 * MoDbQueueManage.cpp
 *
 *  Created on: 2015年10月9日
 *      Author: ruancongming
 */
#include <iostream>
#include "RedisQueueManage.h"
#include <unistd.h>
#include <string.h>

using namespace std;
RedisQueueManage::RedisQueueManage(	const char *ip,unsigned int port,const char *password){
	mhostname  = ip;
	mport = port;
	mpassword = password;

	queuelock = new  pthread_mutex_t;
	pthread_mutex_init(queuelock, NULL);
	queue = new SP_CircleQueue ();

	queuecount = 0;
	queuemax = 30;
	timeout = 3;

	pthread_t commandSend_t;
	pthread_create(&commandSend_t, NULL, RedisLoop, this);
}

RedisQueueManage::~RedisQueueManage(){
	pthread_mutex_lock(queuelock);
	delete queue;
	pthread_mutex_unlock(queuelock);

	pthread_mutex_destroy(queuelock);
	delete queuelock;
}

void* RedisQueueManage::RedisLoop(void *arg){
	RedisQueueManage *temp = (RedisQueueManage*)arg;
//	static time_t lastInfoTime = time(NULL);

	while(1){
//		if ( time(NULL) - lastInfoTime >2) {
//			time(&lastInfoTime);
//			cout<< "******RedisPool，已缓存的句柄："<<temp->queue->getLength()<<endl;
			temp->QueueTopAlive();
//		}
		sleep(280);
	}
	return NULL;
}

QueueElement * RedisQueueManage::GetRedisContext(){
	QueueElement *element = NULL;

	element = GetFromQueue();
	if(element!=NULL){
//#ifdef DEBUG
//		cout<< "获取Redis连接句柄成功，来自队列！" <<endl;
//#endif
		return element;
	}

	if(queuecount<queuemax){
//#ifdef DEBUG
//		cout<< "新建Redis连接句柄！" <<endl;
//#endif
		element = CreatRedisContext();
		return element;
	}else{
		time_t time_cur;
		time_t time_start = time(NULL);
		do{
			element = GetFromQueue();
			if(element!=NULL){
//#ifdef DEBUG
//				cout<< "等待Redis连接句柄成功，来自队列！" <<endl;
//#endif
				return element;
			}

			time_cur = time(NULL);
			sleep(1);
		}while(time_cur-time_start<timeout);

		cout<< "等待队列中可用的Redis句柄超时！" <<endl;
		return NULL;
	}
}

QueueElement * RedisQueueManage::CreatRedisContext(){
	QueueElement *element = new QueueElement;
	element->context = redisConnect( mhostname, mport);
	if(element->context!=NULL){
		char cmdformat[81];
		sprintf(cmdformat,"%s  %s","AUTH",mpassword);
		if(redisCommand((redisContext*)element->context,cmdformat)){
//			pthread_mutex_lock(queuelock);
			queuecount++;
//			pthread_mutex_unlock(queuelock);
			element ->Pingtime = time(NULL);
			return element;
		}else{
			DeletRedisContext(element);
		}
	}else{
		delete element;
	}

	cout<< "ip:" <<mhostname<<"  port:"<<mport << " password:" << mpassword<<",连接到Redis失败，请检查Redis配置！"<<endl;
	return NULL;
}

void RedisQueueManage::SaveRedisContext(QueueElement *  element){
//	if(element!=NULL){
		pthread_mutex_lock(queuelock);
		queue->push((void*)element);
		pthread_mutex_unlock(queuelock);
//	}
}

void RedisQueueManage::DeletRedisContext(QueueElement *  element){
//	if(element!=NULL){
		if(element->context!=NULL){
//			pthread_mutex_lock(queuelock);
			if(queuecount>0)
				queuecount --;
//			pthread_mutex_unlock(queuelock);
			redisFree(element->context);
		}
		delete element;
//	}
//	redisFree(element->context);
//	delete element;
}

QueueElement * RedisQueueManage::GetFromQueue(){
	QueueElement *element =NULL;

	pthread_mutex_lock(queuelock);
	element = (QueueElement *)queue->pop();
	pthread_mutex_unlock(queuelock);

	return element;
}

void RedisQueueManage::QueueTopAlive(){
//	 cout<<" 总共申请句柄：" <<queuecount<<endl;
	pthread_mutex_lock(queuelock);
	int length = queue->getLength();
	int counter = 0;
	QueueElement * element = NULL;
	QueueElement** buffer =(QueueElement**) new char(length*sizeof(void*));
	while(length--){
		element = (QueueElement *)queue->pop();

		if(element!=NULL){
			if(redisCommand(element->context,"PING")){
				element->Pingtime = time(NULL);
				buffer[counter] = element;
				counter++;
			}else{
				DeletRedisContext(element);
			}
		}else{
				DeletRedisContext(element);
		}
	}
	for(int i=0;i<counter;i++){
		queue->push(buffer[i]);
	}
	pthread_mutex_unlock(queuelock);
	delete [](char*)buffer;
}

void RedisQueueManage::ClearRedis() {

	pthread_mutex_lock(queuelock);
	while(queue->getLength()){
	QueueElement  *element = (QueueElement *)queue->pop();
	if(element!=NULL){
		if(element->context!=NULL)
			redisFree(element->context);
		delete element;
		queuecount --;
		}

	}
	pthread_mutex_unlock(queuelock);

}
