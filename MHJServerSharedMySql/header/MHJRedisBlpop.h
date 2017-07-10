/*
 * MHJRedisBlpop.h
 *
 *  Created on: 2016年1月5日
 *      Author: ruancongming
 */

#ifndef MHJREDISBLPOP_H_
#define MHJREDISBLPOP_H_
#include <spbuffer.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <hiredis/async.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MHJRedisBaseFactory.h"
#include "MHJRedisSubscribeFactory.h"


#define REDISCMD_SIZE  128
class MHJRedisBlpop {
public:
	MHJRedisBlpop(char*ip, int port,char* passwd,MHJRedisSubscribeFactory* subscribefactory);
	virtual ~MHJRedisBlpop();
	void doBlpop(void);
private:
	char* ip;
	int port;
	char* passwd;
	MHJRedisSubscribeFactory* subscribe;
};

#endif /* MHJREDISBLPOP_H_ */
