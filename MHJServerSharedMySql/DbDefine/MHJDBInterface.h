/*
 * MHJDBInterface.h
 *
 *  Created on: 2015年10月16日
 *      Author: waroy
 */

#ifndef MHJDBINTERFACE_H_
#define MHJDBINTERFACE_H_
#include "MHJDBInterfaceFactory.h"

//struct CmpByKey {
//	bool operator()(const std::string& k1, const std::string& k2) {
//		return k1.length() < k2.length();
//	}
//};

class MHJDBInterface: public MHJDBInterfaceFactory {
public:

	MHJDBInterface();
	virtual ~MHJDBInterface();

	bool postDevicelogin(u_int32_t deviceID, u_int8_t deviceType, u_int8_t protocolID, char* deviceIp, char* netIp);
	bool postDeviceHeartbeat(u_int32_t deviceID, u_int8_t deviceType, u_int8_t protocolID, char* postData);
	/*
	 * 获取设备基础资料
	 */
	bool getMhjDeviceBaseByDBID(u_int32_t deviceDBID, std::string* response);
	bool getMhjDeviceBaseByDeviceIDIdsecurityDevictType(u_int32_t deviceID, u_int16_t idsecurity, u_int8_t deviceType, std::string *response);
	bool getAdminEntityByID(u_int32_t adminDBID, std::string* response);
	bool getUserEntityById(u_int32_t userID, std::string* response);
	bool getMHJDeviceChildByID(u_int32_t childID, std::string* response);
	bool getMHJDeviceActivationByDeviceID(u_int32_t deviceDBID, std::string* response);

	bool selectMHJDeviceAuthorizesByDeviceChildDBID(u_int32_t childID, std::string* response);
	bool selectUserSecurityByIDList(const char* useridList, std::string* response);
	bool selectDeviceChildByDeviceDBIDAndSwitchNumber(u_int32_t deviceDBID, u_int16_t switchNumber, std::string* response);
	bool selectDeviceChildsByDeviceDBID(u_int32_t deviceDBID, std::string* response);
	bool selectMHJDeviceVirtualDefines(u_int32_t childID, std::string* response);

	bool updateMHJDeviceActivationsoftVer(u_int64_t activationID, std::string& newSfotVer);
	bool updateDeviceChildState(u_int64_t childID, u_int32_t newState);
};

#endif /* MHJDBINTERFACE_H_ */
