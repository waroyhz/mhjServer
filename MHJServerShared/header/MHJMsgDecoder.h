/*
 * MHJMsgDecoder.h
 *
 *  Created on: 2015年7月1日
 *      Author: root
 */

#ifndef MHJMSGDECODER_H_
#define MHJMSGDECODER_H_
#include "spmsgdecoder.hpp"
#include "QueueData.h"
#include "MHJSecurityFactory.h"

class MHJ_Device_MsgDecoder : public SP_MsgDecoder {
public:
	MHJ_Device_MsgDecoder(MHJSecurityFactory *pSecurity);
	~MHJ_Device_MsgDecoder();

	// return SP_MsgDecoder::eMoreData until meet <CRLF>
	virtual int decode( SP_Buffer * inBuffer );

	SP_CircleQueue * getQueue();

//	const char* getMsg();
private:
//	char * mLine;
	SP_CircleQueue * mQueue;
	MHJSecurityFactory *mSecurity;
};


#define PACKHEADSIZE 20 /* 包头到数据长度的总长度 */
#define MsgHead  "$MHJ"  /* 包头 */
#endif /* MHJMSGDECODER_H_ */
