/*
 * ProcessBase.h
 * 基础处理过程，这个类是所有处理过程到基类，根据类型得到所有需要处理的类型
 *  Created on: 2015-7-5
 *      Author: waroy
 */

#ifndef PROCESSBASE_H_
#define PROCESSBASE_H_

#include "MHJProtocol.h"
#include "SP_OnlineManager.h"
#include "sprequest.hpp"
#include "spresponse.hpp"

#include "boost/shared_ptr.hpp"
#include "MHJDefine.h"
#include "ProtocolTypeDefine.h"
#include "ProcessTransformationFactory.h"
#include "MHJFunctionTimeRecord.h"

#include "spserver.hpp"

using namespace boost;
class MHJSecurityFactoryServer;
class MHJProtocol;
class ProcessTransformationFactory;
/*
 * 基础处理单元
 */
class ProcessFactory {
public:
	ProcessFactory(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType classProtocolType);
	virtual ~ProcessFactory();

	virtual void ProtocolLogin(boost::shared_ptr<MHJProtocol> protocol);

protected:
	MHJProtocolType classProtocolType;
	boost::shared_ptr<MHJProtocol> protocol;
	SP_Request *request;
	SP_Response *response;
	static SP_OnlineManager* mOnlineManager;
	MHJSecurityFactoryServer* mSecurity;

	ProcessTransformationFactory* getTransformationFactory(ProcessTransformationFactory* transformation);
public:

	/*
	 * 处理进程
	 */
	virtual ProcessFactory * getProcess(SP_Request *request, SP_Response *response, boost::shared_ptr<MHJProtocol> protocol);

	virtual DWORD getProtocolType();

	/*
	 * 初始化处理过程
	 */
	virtual void initProcess();

	/*
	 * 处理
	 */
	virtual void Process();

	/*
	 * 释放
	 */
	virtual void Release(void*);

	static void setOnlimeManager(SP_OnlineManager* onlineManager);
	static SP_OnlineManager* getOnlimeManager();
	virtual bool ResponseProtocol(UINT16 mpackID, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);

	template<class T>
	bool ResponseProtocol(UINT16 mpackID, BYTE mprotocolID, BYTE mver, T *pdata) {
		return ResponseProtocol(mpackID, mprotocolID, mver, sizeof(T), pdata);
	}

	virtual bool ResponseProtocol(WORD mlength, void *pdata);

	template<class T>
	bool ResponseProtocol(T *pdata) {
		return ResponseProtocol(sizeof(T), pdata);
	}

	virtual void clientClose(SP_Sid_t sid);

	void setSecurity(MHJSecurityFactoryServer* security);

	bool isLogin(const MHJDeviceMark* pdevice, BYTE protocolID);

	template<class T>
	T* getTransformation(boost::shared_ptr<MHJProtocol> protocol) {
		return (T*) getTransformationFactory(new T(response, protocol));
	}
	;

	void releaseTransformation(ProcessTransformationFactory* transformation);

	static SP_Server * pspserver;
};

#endif /* PROCESSBASE_H_ */
