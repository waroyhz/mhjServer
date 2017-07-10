/*
 * QueueData.h
 *
 *  Created on: 2015年7月4日
 *      Author: root
 */

#ifndef HEADER_QUEUEDATA_H_
#define HEADER_QUEUEDATA_H_
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include "MHJProtocol.h"

using namespace boost;

typedef struct QueueData
{
	bool isValid;
	char * errMessage;
//	char *Data;
	shared_ptr<MHJProtocol> Data;
	int size;
}QueueData;



#endif /* HEADER_QUEUEDATA_H_ */
