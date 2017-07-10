/*
 * MHJDBInterfaceFactory.h
 *
 *  Created on: 2015年10月31日
 *      Author: waroy
 */

#ifndef MHJDBINTERFACEFACTORY_H_
#define MHJDBINTERFACEFACTORY_H_
#include <string>
#include <sys/types.h>
#include <map>

typedef void CURL;
class MHJDBInterfaceFactory {
public:
	typedef std::map<std::string, const char*, std::less<std::string> > MAP_PARAMS;

	MHJDBInterfaceFactory();
	virtual ~MHJDBInterfaceFactory();

	/**
	 * 初始化，获取server的Token
	 * 不要在多线程模式下调用。
	 */
	static bool initDBInterface(const char* webHost, u_int16_t Port, const char* URI, const char* key, const char* token);

protected:
	static CURL* getCURL(const char* url, const char* cookie, std::string* response);
	static void getUrl(const char* action, char* outUrl);
	static void cleanupCURL(void *curl);
	static void update_cookies(void * _curl);
	static void updateCookieValue(char * jsession);

	static void setGetParam(const char* url, void * curl, MAP_PARAMS* params, char* postCache);
	static void setPostParam(void * curl, MAP_PARAMS* params, char* postCache);

	static bool setCookie(void* curl, MAP_PARAMS* params, const char* key);
	static void setCookie(void* curl, MAP_PARAMS* params);

	static const char* host;
	static const char* uri;
	static u_int16_t port;
	static const char* serverKey;
	static const char* serverToken;

	char* mpostCache;
};

#endif /* MHJDBINTERFACEFACTORY_H_ */
