/*
 * MHJSecurity.h
 *
 *  Created on: Jul 28, 2015
 *      Author: ruancongming
 */
#ifndef MHJSECURITYFACTORY_H_
#define MHJSECURITYFACTORY_H_

#include "ProcessFactory.h"
#include <boost/unordered_map.hpp>

//#include "MHJDatabase.hxx"

class ProcessFactory;

//typedef boost::unordered_map<int,std::tr1::shared_ptr<MHJDeviceBase> > MAP_Database;

class MHJSecurityFactory {
public:

	MHJSecurityFactory();
	virtual ~MHJSecurityFactory();

	virtual char* getSecurity(const MHJDeviceMark* pdevice, BYTE protocolID);
	virtual void createSessionSecurity(const MHJDeviceMark* pdevice, char *outSecurity);
	virtual void setSessionSecurityData(BYTE* SessionSecurityData);

	virtual void* getDataBaseFactory();
	virtual void* getDatabaseManage(MHJDeviceMark* mark);
//	int getMachineSecurityData(uint32_t deviceID,uint16_t idSecurity,uint8_t deviceType,char *g_SecNum);

protected:
	void createSessionSecurityData(char* outSecurityData);
	char* getServerStaticSecurity();

};

#endif /* MHJSECURITY_H_ */
