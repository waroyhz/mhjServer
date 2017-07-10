/*
 * ProcessTransformationRedis.h
 *
 *  Created on: 2015年11月25日
 *      Author: ruancongming
 */

#ifndef PROCESSTRANSFORMATIONREDIS_H_
#define PROCESSTRANSFORMATIONREDIS_H_

#include "ProcessTransformationFactory.h"


class ProcessTransformationRedis: public ProcessTransformationFactory {
public:
	ProcessTransformationRedis(SP_Response *response, boost::shared_ptr<MHJProtocol> protocol, MHJProtocolType protocolType);
	virtual ~ProcessTransformationRedis();

	template<class T>
		bool sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, T *pdata) {
			return sendProtocol(destDevice, mprotocolID, mver, sizeof(T), pdata);
		}
	virtual bool sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);

//	virtual bool sendProtocol(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);
	virtual bool sendProtocolRedis(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, WORD mlength, void *pdata);
	template<class T>
	bool sendProtocolRedis(const MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver, T *pdata) {
		return sendProtocolRedis(destDevice, mprotocolID, mver, sizeof(T), pdata);
	}

	boost::shared_ptr<SP_Buffer> RedisSubscribeTransferProtocolPackage(MHJDeviceMark* destDevice, BYTE mprotocolID, BYTE mver,
				WORD mlength, void* pdata);
};

#endif /* PROCESSTRANSFORMATIONREDIS_H_ */
