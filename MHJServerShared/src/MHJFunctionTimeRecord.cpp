/*
 * MHJFunctionTimeRecord.cpp
 *
 *  Created on: Aug 25, 2015
 *      Author: ruancongming
 */
#include "MHJFunctionTimeRecord.h"

MHJFunctionTimeRecorder::MHJFunctionTimeRecorder(FunctionTimeRecord* addr, int size){
	MemAddr_star =  addr; //
	Total = size;
	Account = 0;

	mutex_read = new pthread_mutex_t;
	mutex_write = new pthread_mutex_t;
	pthread_mutex_init(mutex_read, NULL);
	pthread_mutex_init(mutex_write, NULL);
}

MHJFunctionTimeRecorder::~MHJFunctionTimeRecorder(){
	if(MemAddr_star!=NULL){
//		free(MemAddr_star);
	}
	MemAddr_star = NULL;
	delete mutex_write;
	delete mutex_read;
}

FunctionTimeRecord* MHJFunctionTimeRecorder::getCurrent(){
	FunctionTimeRecord *ret;
	pthread_mutex_lock(mutex_read);
	if(Account< Total){
		ret = MemAddr_star + Account;
		Account++;
	}else{
//		std::cout <<"FUll" <<std::endl;
	}
	pthread_mutex_unlock(mutex_read);
	return ret;
}

void MHJFunctionTimeRecorder::MHJfunctimeRecoder_clear() {
	pthread_mutex_lock(mutex_read);
	memset(MemAddr_star,0,Account*sizeof(FunctionTimeRecord));
	Account = 0;
	pthread_mutex_unlock(mutex_read);
}

int MHJFunctionTimeRecorder::MHJfunctimeRecoder_ocupty() {
	return Account;
}

FunctionTimeRecord* MHJFunctionTimeRecorder::getStart(){
	return MemAddr_star;
}

void MHJFunctionTimeRecorder::display(){

	if(Account>0){
		printf("线程%x执行时间信息为：\n",pthread_self());
//		pthread_mutex_lock(mutex_read);
		for(int i=0; i<Account; i++){
			printf("%s\n", MemAddr_star[i].record);
		}
//		pthread_mutex_unlock(mutex_read);
//		Account = 0;
	}
	else{
		printf(" 缓存区为空！\n");
	}
}

int MHJFunctionTimeRecorder::isfull(){
	int ret = 0;
	if(Account<Total)
		ret = 0;
	else
		ret = 1;

	return ret;
}

int MHJFunctionTimeRecorder::isempty(){
	int ret = 0;
	if(Account==0)
		ret = 0;
	else
		ret = 1;
	return ret;
}

void MHJFunctionTimeRecorder::MHJfunctimeRecoder_read(FunctionTimeRecord *outBuff, int size){

	pthread_mutex_lock(mutex_read);

	if(size<Account){
		memcpy(outBuff->record,(MemAddr_star+Account-size)->record,size*sizeof(FunctionTimeRecord));
		Account -=size;
	}else{
		memcpy(outBuff->record,MemAddr_star->record,Account*sizeof(FunctionTimeRecord));
		Account =0;
	}

	pthread_mutex_unlock(mutex_read);
}

void MHJFunctionTimeRecorder::MHJfunctimeRecoder_write(char *src, int size){
	pthread_mutex_lock(mutex_read);

	if(Account < Total){
		memcpy(MemAddr_star+Account,src,size);
		Account++;
	}else{
		printf("Full\n");
	}

	pthread_mutex_unlock(mutex_read);
}

FunctionTimeRecord* MHJFunctionTimeRecorder::getBuff() {
	FunctionTimeRecord *ret;
	pthread_mutex_lock(mutex_read);
	if(Account< Total){
		ret = MemAddr_star + Account;
		Account++;
	}else{
		ret = MemAddr_star;
		Account=0;
	}
	pthread_mutex_unlock(mutex_read);
	return ret;
}

int MHJFunctionTimeRecorder::getAccount() {
	return Account;
}
