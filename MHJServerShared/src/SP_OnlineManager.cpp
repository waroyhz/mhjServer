/*
 * SP_OnlineManager.cpp
 *
 *  Created on: 2015-7-7
 *      Author: waroy
 */

#include "SP_OnlineManager.h"
#include <stdlib.h>

SP_OnlineManager::SP_OnlineManager() {
	sp_thread_mutex_init(&mMutex, NULL);
//		DWORD errcode= GetLastError();
}

SP_OnlineManager::~SP_OnlineManager() {
	sp_thread_mutex_destroy (&mMutex);
}

void SP_OnlineManager::copy(SP_SidList * outList, SP_Sid_t * ignoreSid) {
	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);

		if (NULL != ignoreSid) {
			SP_Sid_t theSid = info->mSid;
			if (theSid.mKey == ignoreSid->mKey && theSid.mSeq == ignoreSid->mSeq) {
				continue;
			}
		}

		outList->add(info->mSid);
	}

	sp_thread_mutex_unlock(&mMutex);
}

void SP_OnlineManager::remove(SP_Sid_t sid) {
	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		SP_Sid_t theSid = info->mSid;
		if (theSid.mKey == sid.mKey && theSid.mSeq == sid.mSeq) {
			mList.takeItem(i);
			free(info);
			break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);
}

SP_OnlineInfo_t* SP_OnlineManager::newInfo() {
	SP_OnlineInfo_t* info = (SP_OnlineInfo_t *) malloc(sizeof(SP_OnlineInfo_t));
	memset(info, 0, sizeof(SP_OnlineInfo_t));
	sp_thread_mutex_lock (&mMutex);
	mList.append(info);

	sp_thread_mutex_unlock(&mMutex);
	return info;
}

//void SP_OnlineManager :: add( SP_OnlineInfo_t * info )
//{
//	sp_thread_mutex_lock( &mMutex );
//
//	mList.append( info );
//
//	sp_thread_mutex_unlock( &mMutex );
//}

int SP_OnlineManager::getCount() {
	int count = 0;

	sp_thread_mutex_lock (&mMutex);

	count = mList.getCount();

	sp_thread_mutex_unlock(&mMutex);

	return count;
}

MHJDeviceMark* SP_OnlineManager::getDeviceMark(SP_Sid_t sid) {
	MHJDeviceMark* chatMark = NULL;

	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		SP_Sid_t theSid = info->mSid;
		if (theSid.mKey == sid.mKey && theSid.mSeq == sid.mSeq) {
			chatMark = &info->mClientMark;
			break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);

	return chatMark;
}

SP_Sid_t *SP_OnlineManager::getSid(const MHJDeviceMark *mark) {
	SP_Sid_t * sid = NULL;

	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		MHJDeviceMark * theMark = &info->mClientMark;
		if (theMark->deviceID == mark->deviceID && theMark->deviceType == mark->deviceType) {

			sid = &info->mSid;
			break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);

	return sid;
}

void SP_OnlineManager::getSidList(const MHJDeviceMark* mark, SP_ArrayList* outArraylist) {
	SP_Sid_t * sid = NULL;

	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		MHJDeviceMark * theMark = &info->mClientMark;
		if (theMark->deviceID == mark->deviceID && theMark->deviceType == mark->deviceType) {

			sid = &info->mSid;
			outArraylist->append(sid);
//					break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);
}

SP_OnlineInfo_t* SP_OnlineManager::getClientInfo(SP_Sid_t sid) {
	SP_OnlineInfo_t * retinfo = NULL;

	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		SP_Sid_t theSid = info->mSid;
		if (theSid.mKey == sid.mKey && theSid.mSeq == sid.mSeq) {
			retinfo = info;
			break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);

	return retinfo;
}

SP_OnlineInfo_t* SP_OnlineManager::getItem(int index) {
	SP_OnlineInfo_t * retinfo = NULL;
	sp_thread_mutex_lock (&mMutex);

	retinfo = (SP_OnlineInfo_t*) mList.getItem(index);

	sp_thread_mutex_unlock(&mMutex);

	return retinfo;
}

void  SP_OnlineManager::setClientReceiveTime(SP_Sid_t sid){
	time_t rawtime;
	time ( &rawtime );

	sp_thread_mutex_lock (&mMutex);

	for (int i = 0; i < mList.getCount(); i++) {
		SP_OnlineInfo_t * info = (SP_OnlineInfo_t*) mList.getItem(i);
		SP_Sid_t theSid = info->mSid;
		if (theSid.mKey == sid.mKey && theSid.mSeq == sid.mSeq) {

			info->mClientReceiveTime = rawtime;
			break;
		}
	}

	sp_thread_mutex_unlock(&mMutex);
}
