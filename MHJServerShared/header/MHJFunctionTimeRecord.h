/*
 * MHJFunctionTimeRecord.h
 *
 *  Created on: Aug 24, 2015
 *      Author: ruancongming
 */
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#ifndef MHJFUNCTIONTIMERECORD_H_
#define MHJFUNCTIONTIMERECORD_H_

#define _FUNCTION_TIME_RECORD
#ifdef _FUNCTION_TIME_RECORD

#define RECORD_SIZE 128
struct FunctionTimeRecord{
	char record[RECORD_SIZE];
};

//no paramters
#define FUNCTION_TIME_RECORD_00(record,function) { \
	struct timeval start, end;	\
    int interval;				\
    gettimeofday(&start, NULL); \
    function();					\
    gettimeofday(&end, NULL);   \
    interval = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);\
    snprintf( record, RECORD_SIZE,"Time:%d us, Func:%s, File:%s, Line:%d", interval,#function,__FILE__, __LINE__);\
}
//1 paramter
#define FUNCTION_TIME_RECORD_10(function,record,size,arg0) { \
	struct timeval start, end;	\
    int interval;				\
    gettimeofday(&start, NULL); \
    function(arg0);					\
    gettimeofday(&end, NULL);   \
    interval = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);\
    snprintf( record, RECORD_SIZE,"Time:%d us, Func:%s, File:%s, Line:%d", interval,#function,__FILE__, __LINE__);\
}
//2 paramters
#define FUNCTION_TIME_RECORD_20(function,record,size,arg0,arg1) { \
	struct timeval start, end;	\
    int interval;				\
    gettimeofday(&start, NULL); \
    function(arg0,arg1);					\
    gettimeofday(&end, NULL);   \
    interval = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);\
    snprintf( record, RECORD_SIZE,"Time:%d us, Func:%s, File:%s, Line:%d", interval,#function,__FILE__, __LINE__);\
}

#define FUNCTION_TIME_RECORD_31(record,function,ret,arg0,arg1,arg2) { \
	struct timeval start, end;	\
    int interval;				\
    gettimeofday(&start, NULL); \
    ret = function(arg0,arg1,arg2);					\
    gettimeofday(&end, NULL);   \
    interval = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);\
    snprintf( record, RECORD_SIZE,"Time:%d us, Func:%s, File:%s, Line:%d", interval,#function,__FILE__, __LINE__);\
}


#else
//no paramters
#define FUNCTION_TIME_RECORD_0(function,moduleName,record,size) { \
    function();					\
}
//1 paramter
#define FUNCTION_TIME_RECORD_1(function,moduleName,record,size,arg0) { \
    function(arg0);					\
}
//2 paramters
#define FUNCTION_TIME_RECORD_2(function,moduleName,record,size,arg0,arg1) { \
    function(arg0,arg1);					\
}
#endif

#define TIMERECORDER_SIZE 1024
class MHJFunctionTimeRecorder {
public:
	MHJFunctionTimeRecorder(FunctionTimeRecord* addr, int size);
//	MHJFunctionTimeRecorder();
	~MHJFunctionTimeRecorder();
	FunctionTimeRecord* getCurrent();
	void MHJfunctimeRecoder_clear();
	int MHJfunctimeRecoder_ocupty();

	FunctionTimeRecord* getBuff();
	void MHJfunctimeRecoder_read(FunctionTimeRecord *outBuff, int size);
	void MHJfunctimeRecoder_write(char *src, int size);
	FunctionTimeRecord* getStart();
	int getAccount();
	void display();
	int isfull();
	int isempty();
private:
	int Total;
	int Account;
	FunctionTimeRecord *MemAddr_star;
	pthread_mutex_t *mutex_read;
	pthread_mutex_t *mutex_write;

};

#endif /* MHJFUNCTIONTIMERECORD_H_ */
