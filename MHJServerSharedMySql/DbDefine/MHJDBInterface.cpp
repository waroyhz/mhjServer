/*
 * MHJDBInterface.cpp
 *
 *  Created on: 2015年10月16日
 *      Author: waroy
 */

#include "MHJDBInterface.h"
#include <curl/curl.h>
#include "JsonStrObjMap.h"
#include <inttypes.h>

MHJDBInterface::MHJDBInterface() {

}

MHJDBInterface::~MHJDBInterface() {
}

bool MHJDBInterface::getMhjDeviceBaseByDBID(u_int32_t deviceDBID, std::string* response) {
	char url[256];
	getUrl("getMhjDeviceBaseById", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", deviceDBID);
		params["id"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::getAdminEntityByID(u_int32_t adminDBID, std::string* response) {
	char url[256];
	getUrl("getAdminEntity", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", adminDBID);
		params["id"] = strid;
		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::postDevicelogin(u_int32_t deviceID, u_int8_t deviceType, u_int8_t protocolID, char* deviceIp, char* netIp) {
	std::string response;
	char url[256];
	getUrl("postDevicelogin", url);

	CURL *curl = getCURL(url, "", &response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", deviceID);
		params["deviceID"] = strid;
		char strdeviceType[32];
		sprintf(strdeviceType, "%d", deviceType);
		params["deviceType"] = strdeviceType;
		char strprotocolID[32];
		sprintf(strprotocolID, "%d", protocolID);
		params["protocolID"] = strprotocolID;

		params["deviceIp"] = deviceIp;
		params["netIp"] = netIp;

		setPostParam(curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::postDeviceHeartbeat(u_int32_t deviceID, u_int8_t deviceType, u_int8_t protocolID, char* postData) {
	std::string response;
	char url[256];
	getUrl("postDeviceHeartbeat", url);

	CURL *curl = getCURL(url, "", &response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", deviceID);
		params["deviceID"] = strid;

		char strdeviceType[32];
		sprintf(strdeviceType, "%d", deviceType);
		params["deviceType"] = strdeviceType;

		char strprotocolID[32];
		sprintf(strprotocolID, "%d", protocolID);
		params["protocolID"] = strprotocolID;

		params["postData"] = postData;

		setPostParam(curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::getMhjDeviceBaseByDeviceIDIdsecurityDevictType(u_int32_t deviceID, u_int16_t idsecurity, u_int8_t deviceType, std::string *response) {
	char url[256];
	getUrl("getMhjDeviceBaseByDeviceIDIdsecurityDevictType", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", deviceID);
		params["deviceID"] = strid;

		char strdeviceType[32];
		sprintf(strdeviceType, "%d", deviceType);
		params["deviceType"] = strdeviceType;

		char stridsecurity[32];
		sprintf(stridsecurity, "%d", idsecurity);
		params["idsecurity"] = stridsecurity;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::getUserEntityById(u_int32_t userID, std::string* response) {
	char url[256];
	getUrl("getUserEntityById", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", userID);
		params["userID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::getMHJDeviceChildByID(u_int32_t childID, std::string* response) {
	char url[256];
	getUrl("getMHJDeviceChildByID", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", childID);
		params["childID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::getMHJDeviceActivationByDeviceID(u_int32_t deviceDBID, std::string* response) {
	char url[256];
	getUrl("getMHJDeviceActivationByDeviceID", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", deviceDBID);
		params["deviceDBID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::selectMHJDeviceAuthorizesByDeviceChildDBID(u_int32_t childID, std::string* response) {
	char url[256];
	getUrl("selectMHJDeviceAuthorizesByDeviceChildDBID", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%d", childID);
		params["childDBID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::selectUserSecurityByIDList(const char* useridList, std::string* response) {
	char url[256];
	getUrl("selectUserSecurityByIDList", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
//			char strid[10];
//			sprintf(strid, "%d", useridlist);
		params["useridlist"] = useridList;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::selectMHJDeviceVirtualDefines(u_int32_t childID, std::string* response) {
	char url[256];
	getUrl("selectMHJDeviceVirtualDefines", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[10];
		sprintf(strid, "%d", childID);
		params["childID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::updateMHJDeviceActivationsoftVer(u_int64_t activationID, std::string& newSfotVer) {
	char url[256];
	getUrl("updateMHJDeviceActivationsoftVer", url);
	std::string response;
	CURL *curl = getCURL(url, "", &response);
	if (curl) {
		MAP_PARAMS params;
		char strid[16];
		sprintf(strid, "%lu", activationID);
		params["activationID"] = strid;
		params["newSfotVer"] = newSfotVer.c_str();

		setPostParam(curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			printf(response.c_str());
		}
		cleanupCURL(curl);
	} else {
		fprintf(stderr, "Curl init failed!\n");
	}
	if (jsonGetMHJReturnSuccess(response))
		return true;
	else {
		printf("jsonGetMHJReturnSuccess return false\n %s", response.c_str());
		return false;
	}
}

bool MHJDBInterface::selectDeviceChildByDeviceDBIDAndSwitchNumber(u_int32_t deviceDBID, u_int16_t switchNumber, std::string* response) {
	char url[256];
	getUrl("selectDeviceChildByDeviceDBIDAndSwitchNumber", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%u", deviceDBID);
		params["deviceDBID"] = strid;
		char strswitchNumber[32];
		sprintf(strswitchNumber, "%u", switchNumber);
		params["switchNumber"] = strswitchNumber;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

bool MHJDBInterface::updateDeviceChildState(u_int64_t childID, u_int32_t newState) {
	char url[256];
	getUrl("updateDeviceChildState", url);
	std::string response;
	CURL *curl = getCURL(url, "", &response);
	if (curl) {
		MAP_PARAMS params;
		char strid[16];
		sprintf(strid, "%lu", childID);
		params["childID"] = strid;
		char strnewState[16];
		sprintf(strnewState, "%d", newState);
		params["newState"] = strnewState;

		setPostParam(curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
		}
		cleanupCURL(curl);
	} else {
		fprintf(stderr, "Curl init failed!\n");
	}
	if (jsonGetMHJReturnSuccess(response))
		return true;
	else {
		printf("jsonGetMHJReturnSuccess return false\n %s", response.c_str());
		return false;
	}
}

bool MHJDBInterface::selectDeviceChildsByDeviceDBID(u_int32_t deviceDBID, std::string* response) {
	char url[256];
	getUrl("selectDeviceChildsByDeviceDBID", url);

	CURL *curl = getCURL(url, "", response);
	if (curl) {
		MAP_PARAMS params;
		char strid[32];
		sprintf(strid, "%u", deviceDBID);
		params["deviceDBID"] = strid;

		setGetParam(url, curl, &params, mpostCache);
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
			return false;
		}
		cleanupCURL(curl);
		return true;
	} else {
		fprintf(stderr, "Curl init failed!\n");
		return false;
	}
	return false;
}

