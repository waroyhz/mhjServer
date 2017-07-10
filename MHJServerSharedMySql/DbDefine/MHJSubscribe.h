/*
 * MHJSubscribe.h
 *
 *  Created on: 2015年10月22日
 *      Author: ruancongming
 */

#ifndef MHJSUBSCRIBE_H_
#define MHJSUBSCRIBE_H_
#include <iostream>
class MHJSubscribe {
public:
	MHJSubscribe();
	MHJSubscribe(std::string channel, int protocol);
	virtual ~MHJSubscribe();
//private:
	std::string channel;
	int protocolType;
//	void data;
};

#endif /* MHJSUBSCRIBE_H_ */
