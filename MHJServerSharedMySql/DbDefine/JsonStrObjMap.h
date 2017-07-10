/*
 * JsonStrObjMap.h
 *
 *  Created on: 2015年10月15日
 *      Author: ruancongming
 */

#ifndef JSONSTROBJMAP_H_
#define JSONSTROBJMAP_H_

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/config.h>
#include <json/forwards.h>
#include <json/autolink.h>
#include <json/features.h>

#include "MHJDatabase.hxx"

#include "MHJSubscribe.h"

void json2StrMHJAdminEntity(std::tr1::shared_ptr<MHJAdminEntity> obj, std::string *str);
std::tr1::shared_ptr<MHJAdminEntity> json2MHJAdminEntity(std::string *str);

void json2StrMHJSubscribe(std::tr1::shared_ptr<MHJSubscribe> obj, std::string *str);
std::tr1::shared_ptr<MHJSubscribe> json2MHJSubscribe(std::string *str);

bool jsonValueIsEmpty(Json::Value & value);
uint64_t jsonGetUInt64(Json::Value & value);
uint32_t jsonGetUInt(Json::Value & value);
int32_t jsonGetInt(Json::Value & value);
boost::posix_time::ptime jsonGetPTime(Json::Value & value);
std::string jsonGetString(Json::Value & value);

Json::Value jsonGetMHJReturnData(std::string &responstStr);
bool jsonGetMHJReturnSuccess(std::string &responseStr);

std::tr1::shared_ptr<MHJAdminEntity> json2MHJAdminEntity(Json::Value& jsonvalue);
std::tr1::shared_ptr<MHJDeviceBase> json2MHJDeviceBase(Json::Value& jsonvalue);
std::tr1::shared_ptr<MHJUserEntity> json2MHJUserEneity(Json::Value& jsonvalue);
std::tr1::shared_ptr<MHJDeviceChild> json2MHJDeviceChild(Json::Value& jsonvalue);
std::tr1::shared_ptr<MHJDeviceActivation> json2MHJDeviceActivation(Json::Value& jsonvalue);

bool json2MHJDeviceAuthorizeVector(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> > *vector);
bool json2UserEntityList(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJUserEntity> > *vector);
bool json2MHJDeviceChilds(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceChild> > *vector);
bool json2MHJDeviceVirtualDefines(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceVirtualDefine> > *vector);

#endif /* JSONSTROBJMAP_H_ */
