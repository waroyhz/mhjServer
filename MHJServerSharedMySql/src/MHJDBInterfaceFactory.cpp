/*
 * MHJDBInterfaceFactory.cpp
 *
 *  Created on: 2015年10月31日
 *      Author: waroy
 */

#include "MHJDBInterfaceFactory.h"

#include <curl/curl.h>
#include <string.h>
#include <vector>

#include<iostream>
#include "md5.h"

#define JSESSIONID "JSESSIONID"
#define postTime "postTime"
#define clientType "clientType"
#define service_Token "token"
#define service_account "service_account"
#define postCRC "CRC"

char JSESSIONID_Value[64];

const char* MHJDBInterfaceFactory::host;
const char* MHJDBInterfaceFactory::uri;
u_int16_t MHJDBInterfaceFactory::port;
const char* MHJDBInterfaceFactory::serverKey;
const char* MHJDBInterfaceFactory::serverToken;

MHJDBInterfaceFactory::MHJDBInterfaceFactory() {
	mpostCache = new char[4096];
}

MHJDBInterfaceFactory::~MHJDBInterfaceFactory() {
	delete[] mpostCache;
}

/* CURLOPT_WRITEFUNCTION */
static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *data) {
	size_t realsize = size * nmemb;
	std::string *buffer = (std::string*) data;
	buffer->append((char*) ptr, realsize);
//	(void) ptr;
//	(void) conn;
	return realsize;
}

CURL* MHJDBInterfaceFactory::getCURL(const char* url, const char* cookie, std::string* response) {
	CURL *curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* just to start the cookie engine */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response); //数据存储的对象指针
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
#ifdef DEBUG
#else
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5); //超时
#endif
	}

	return curl;
}
void MHJDBInterfaceFactory::cleanupCURL(void *curl) {
	update_cookies(curl);
	curl_easy_cleanup(curl);
}

bool MHJDBInterfaceFactory::initDBInterface(const char* webHost, u_int16_t Port, const char* URI, const char* key, const char* token) {
	host = webHost;
	uri = URI;
	port = Port;
	serverKey = key;
	serverToken = token;

	curl_global_init (CURL_GLOBAL_ALL);

	std::string response;
//	char nline[256];
	char url[256];
	getUrl("login", url);

	CURL *curl = getCURL(url, "", &response);
	if (curl) {
		MAP_PARAMS params;
//		params["id"] = "1";
		char postCache[1024];
		setGetParam(url, curl, &params, postCache);
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

void MHJDBInterfaceFactory::getUrl(const char* action, char* outUrl) {
	sprintf(outUrl, "http://%s:%d/%s/%s", host, port, uri, action);
}

void MHJDBInterfaceFactory::update_cookies(void* _curl) {
	CURL* curl = (CURL*) _curl;
	CURLcode res;
	struct curl_slist *cookies;
	struct curl_slist *nc;
	int i;

	printf("Cookies, curl knows:\n");
	res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
	if (res != CURLE_OK) {
		fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
//    exit(1);
	}
	nc = cookies, i = 1;
	char* jsessionvalue = NULL;
	while (nc) {
//		printf("[%d]: %s\n", i, nc->data);
		char* find = strstr(nc->data, JSESSIONID);
		if (find) {
			jsessionvalue = find;
		}
		nc = nc->next;
		i++;
	}
	updateCookieValue(jsessionvalue);
//	if (i == 1) {
//		printf("(none)\n");
//	}
	curl_slist_free_all(cookies);
}

void MHJDBInterfaceFactory::updateCookieValue(char* jsession) {
	if (jsession)
		strcpy(JSESSIONID_Value, jsession);
}

bool MHJDBInterfaceFactory::setCookie(void* curl, MAP_PARAMS* params, const char* key) {
	//生成cookie校验数据
//	"#HttpOnly_192.168.5.126\tFALSE\t/\tFALSE\t0\tJSESSIONID\tF57A481C5B8B2E2BAA8617C14C705533"
	char tmpstr[10240];
	MHJDBInterfaceFactory::MAP_PARAMS::iterator iter = params->find(key);
	if (iter != params->end()) {
		sprintf(tmpstr, "#HttpOnly_%s\tFALSE\t/\tFALSE\t0\t%s\t%s", host, (*iter).first.c_str(), (*iter).second);
		CURLcode res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, tmpstr);
		if (res != CURLE_OK) {
			fprintf(stderr, "Curl curl_easy_setopt failed: %s\n", curl_easy_strerror(res));
			return false;
		}
	}
	return true;
}

void MHJDBInterfaceFactory::setCookie(void* curl, MAP_PARAMS* params) {
	MHJDBInterfaceFactory::MAP_PARAMS tmpparam;
	tmpparam.insert(params->begin(), params->end());
	char tmpchar[32];
	sprintf(tmpchar, "%ld", (unsigned long) time(NULL));
	tmpparam[postTime] = tmpchar;
	tmpparam[clientType] = "server";
	tmpparam[service_Token] = serverToken;
	tmpparam[service_account] = serverKey;

	std::string tmpstr;
	for (MHJDBInterfaceFactory::MAP_PARAMS::iterator iter = tmpparam.begin(); iter != tmpparam.end(); ++iter) {
		if (tmpstr.length() > 0)
			tmpstr.append(",");
		tmpstr.append((*iter).first);
		tmpstr.append(":");
		tmpstr.append((*iter).second);
//		std::cout << (*iter).first << ":" << (*iter).second << std::endl;
	}
	unsigned char decrypt[16];
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, (unsigned char*) tmpstr.c_str(), tmpstr.length());
	MD5Final(&md5, decrypt);

	std::string md5str;
	char tmp[32];
	for (int i = 0; i < 16; i++) {
		sprintf(tmp, "%02x", decrypt[i]);
		md5str.append(tmp);
	}

	tmpparam[postCRC] = md5str.c_str();

//	setCookie(curl, &tmpparam, userToken);
	setCookie(curl, &tmpparam, service_account);
	setCookie(curl, &tmpparam, clientType);
	setCookie(curl, &tmpparam, postTime);
	setCookie(curl, &tmpparam, postCRC);
}

void MHJDBInterfaceFactory::setGetParam(const char* url, void* curl, MAP_PARAMS* params, char* postCache) {
	postCache[0] = 0;
	setCookie(curl, params);

	std::string tmpstr;
	for (MHJDBInterfaceFactory::MAP_PARAMS::iterator iter = params->begin(); iter != params->end(); ++iter) {
		if (tmpstr.length() > 0)
			tmpstr.append("&");
		tmpstr.append((*iter).first);
		tmpstr.append("=");
		tmpstr.append((*iter).second);
//		std::cout << (*iter).first << ":" << (*iter).second << std::endl;
	}

	strcat(postCache, url);
	if (tmpstr.length() > 0) {
		strcat(postCache, "?");
		strcat(postCache, tmpstr.c_str());
	}
//	std::string strurl;
//	strurl.append(url);
//	if (tmpstr.length() > 0) {
//		strurl.append("?");
//		strurl.append(tmpstr);
//	}
	printf("curl get url:%s\n", postCache);
	curl_easy_setopt(curl, CURLOPT_URL, postCache);
}

void MHJDBInterfaceFactory::setPostParam(void* curl, MAP_PARAMS* params, char* postCache) {
	postCache[0] = 0;
	setCookie(curl, params);

//	std::string tmpstr;
	int count = 0;
	for (MHJDBInterfaceFactory::MAP_PARAMS::iterator iter = params->begin(); iter != params->end(); ++iter) {
		if (count > 0)
			strcat(postCache, "&");
		strcat(postCache, (*iter).first.c_str());
		strcat(postCache, "=");
		strcat(postCache, (*iter).second);
		count++;
//		if (tmpstr.length() > 0)
//			tmpstr.append("&");
//		tmpstr.append((*iter).first);
//		tmpstr.append("=");
//		tmpstr.append((*iter).second);
	}
	printf("curl post url:%s\n", postCache);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postCache);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
//	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
}

