/*
 * SP_OnlineManager.h
 *
 *  Created on: 2015-7-7
 *      Author: waroy
 */

#ifndef SP_ONLINEMANAGER_H_
#define SP_ONLINEMANAGER_H_

#include "spresponse.hpp"
#include "spporting.hpp"
#include "sputils.hpp"
#include "ProtocolTypeDefine.h"

typedef struct tagSP_OnlineInfo {
	SP_Sid_t mSid;
	MHJDeviceMark mClientMark;
//	SOCKET mSocket;
	char mClientIP[32];
	int mClientPort;
	int mClientType;

	time_t mClientReceiveTime;
//	SP_Response* spResponse;  //a spResponse ptr releated to msid
} SP_OnlineInfo_t;

class SP_OnlineManager {
public:
	SP_OnlineManager();
	~SP_OnlineManager();

	void copy(SP_SidList * outList, SP_Sid_t * ignoreSid = NULL);
	void remove(SP_Sid_t sid);
	SP_OnlineInfo_t* newInfo();
//	void add( SP_OnlineInfo_t * info );
	MHJDeviceMark* getDeviceMark(SP_Sid_t sid);
	SP_Sid_t * getSid(const MHJDeviceMark* mark);
	void getSidList(const MHJDeviceMark* mark, SP_ArrayList* outArraylist);

	int getCount();

	SP_OnlineInfo_t* getItem(int index);
	SP_OnlineInfo_t* getClientInfo(SP_Sid_t sid);
	void setClientReceiveTime(SP_Sid_t sid);

private:
	SP_ArrayList mList;
	sp_thread_mutex_t mMutex;
};

#endif /* SP_ONLINEMANAGER_H_ */
