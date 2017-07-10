/*
 * MHJSubscribe.cpp
 *
 *  Created on: 2015年10月22日
 *      Author: ruancongming
 */

#include "MHJSubscribe.h"

MHJSubscribe::MHJSubscribe(){

}
MHJSubscribe::MHJSubscribe(std::string channel, int protocol) {
	// TODO Auto-generated constructor stub

	this->channel = channel;
	protocolType = protocol;
//	data = pdata;
}

MHJSubscribe::~MHJSubscribe() {
	// TODO Auto-generated destructor stub
}

