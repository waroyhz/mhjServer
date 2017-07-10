/*
 * JsonStrObjMap.cpp
 *
 *  Created on: 2015年10月15日
 *      Author: ruancongming
 */
#include "JsonStrObjMap.h"
#include <boost/date_time/gregorian/greg_month.hpp>
#include "MoAdminEntity.h"  //the rule of the key
#include "MoMhjdevicebase.h"
#include "MoMhjUserEntity.h"
#include "MoMhjdevicechild.h"
#include "MoMhjdeviceactivation.h"
#include "MoMhjdeviceauthorize.h"
#include "MoMHJDeviceVirtualDefine.h"

void json2StrMHJAdminEntity(std::tr1::shared_ptr<MHJAdminEntity> obj, std::string *str) {
	Json::Value item;
	item[MoAdminEntity::id()] = (Json::UInt64) obj->id;
	item[MoAdminEntity::username()] = obj->UserName;
	item[MoAdminEntity::password()] = obj->Password;
	item[MoAdminEntity::admintoken()] = std::string(obj->adminToken, SECURITYSIZE);
//adminToken为可读字符，使用直接赋值。
//	for (Json::UInt64 i = 0; i < 32; i++) {
//		item[MoAdminEntity::admintoken()].append((Json::UInt64)(i + 1));
//	}

//	str->assign(item.toStyledString());
	Json::FastWriter fwriter;
	str->assign(fwriter.write(item));
//    Json::FastWriter fwriter;
//    std::string out2 = fwriter.write(item);
//	std::cout << out2<<std::endl;

}

std::tr1::shared_ptr<MHJAdminEntity> json2MHJAdminEntity(std::string *str) {

	Json::Reader reader;
	Json::Value root;
	Json::Value jdoc;

	if (!reader.parse(*str, root)) {
		printf("json2MHJAdminEntity parse false\n");
	}
	return json2MHJAdminEntity(root);

}

void json2StrMHJSubscribe(std::tr1::shared_ptr<MHJSubscribe> obj, std::string *str) {
	Json::Value item;
	item["protocolType"] = (Json::UInt64) obj->protocolType;
	item["channel"] = obj->channel;
	Json::FastWriter fwriter;
	str->assign(fwriter.write(item));
}

std::tr1::shared_ptr<MHJSubscribe> json2MHJSubscribe(std::string *str) {
	std::tr1::shared_ptr < MHJSubscribe > obj(new MHJSubscribe);
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(*str, root)) {
		obj->protocolType = root["protocolType"].asUInt64();
		obj->channel = root["channel"].asString();
	}
	return obj;
}

Json::Value jsonGetMHJReturnData(std::string& responseStr) {
	Json::Reader reader(Json::Features::strictMode());
	Json::Value root;

	bool ret = false;
	if (reader.parse(responseStr, root)) {
		ret = root["success"].asBool();
		if (ret) {
			Json::Value jdata = root["data"];
//		std::string data = jdata[MoMhjdevicebase::productionUser()].asString();
#ifdef DEBUG
			Json::FastWriter fwriter;
			std::string outData=fwriter.write(jdata);
			printf("jsonGetMHJReturnData:%s\n",outData.c_str());
#endif
			return jdata;
		} else {
			printf("return success false , response:%s\n", responseStr.c_str());
		}
	}
	return Json::nullValue;
}

bool jsonGetMHJReturnSuccess(std::string& responseStr) {
	Json::Reader reader(Json::Features::strictMode());
	Json::Value root;

	bool ret = false;
	if (reader.parse(responseStr, root)) {
		ret = root["success"].asBool();
	}
	return ret;
}

bool jsonValueIsEmpty(Json::Value& value) {
	if (value.isNull())
		return true;
	else if (value.type() == Json::stringValue) {
		const char* string = value.asCString();
		if (string[0] == 0)
			return true;
	}
	return false;
}

uint64_t jsonGetUInt64(Json::Value& value) {
	if (value.type() == Json::intValue || value.type() == Json::uintValue) {
		return value.asUInt64();
	} else {
		printf("jsonGetUInt64  type error\n");
		return 0;
	}
}
uint32_t jsonGetUInt(Json::Value & value) {
	if (value.type() == Json::intValue || value.type() == Json::uintValue) {
		return value.asUInt();
	} else {
		printf("jsonGetUInt  type error\n");
		return 0;
	}
}
int32_t jsonGetInt(Json::Value & value) {
	if (value.type() == Json::intValue || value.type() == Json::uintValue) {
		return value.asInt();
	} else {
		printf("jsonGetInt  type error\n");
		return 0;
	}
}

boost::posix_time::ptime jsonGetPTime(Json::Value & value) {
	boost::posix_time::ptime ptime;
	if (value.type() == Json::stringValue) {
		if (!jsonValueIsEmpty(value)) {
			Json::Value v = value.asCString();
			ptime = boost::posix_time::time_from_string(v.asString());
		} else {
			printf("jsonGetPTime  type error\n");
		}
	}
	return ptime;
}
std::string jsonGetString(Json::Value & value) {
	if (value.type() == Json::stringValue) {
		return value.asString();
	} else {
		printf("jsonGetString type error\n");
		return std::string("");
	}
}

std::tr1::shared_ptr<MHJAdminEntity> json2MHJAdminEntity(Json::Value& jsonvalue) {
	std::tr1::shared_ptr < MHJAdminEntity > obj;
	Json::Value jdoc;
	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			Json::ArrayIndex index = 0;
			jdoc = jsonvalue.get(index, Json::nullValue);
		} else
			jdoc = jsonvalue;
		if (jdoc == Json::nullValue)
			return obj;

		if (jdoc.type() == Json::objectValue) {
			obj = std::tr1::shared_ptr < MHJAdminEntity > (new MHJAdminEntity);
			obj->id = jdoc[MoAdminEntity::id()].asUInt64();
			obj->UserName = jdoc[MoAdminEntity::username()].asString();
			obj->Password = jdoc[MoAdminEntity::password()].asString();
//			memcpy(obj->adminToken, jdoc[MoAdminEntity::admintoken()].asCString(), SECURITYSIZE);
			Json::Value v = jdoc[MoAdminEntity::admintoken()];
			for (int i = 0; i < SECURITYSIZE; i++) {
				//	v = root[MoAdminEntity::admintoken()][i];
				obj->adminToken[i] = (char) v[i].asUInt();
			}
		}
	}

	return obj;
}

std::tr1::shared_ptr<MHJDeviceBase> json2MHJDeviceBase(Json::Value& jsonvalue) {
	std::tr1::shared_ptr < MHJDeviceBase > obj;
	Json::Value jdoc;

	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			Json::ArrayIndex index = 0;
			jdoc = jsonvalue.get(index, Json::nullValue);
		} else
			jdoc = jsonvalue;
		if (jdoc == Json::nullValue)
			return obj;
		if (jdoc.type() == Json::objectValue) {
			MHJDeviceBase*pMHJDeviceBase = new MHJDeviceBase;
			obj = std::tr1::shared_ptr < MHJDeviceBase > (pMHJDeviceBase);
			obj->id_ = jsonGetUInt64(jdoc[MoMhjdevicebase::id()]);
			obj->deviceID = jsonGetUInt64(jdoc[MoMhjdevicebase::deviceId()]);
			obj->idsecurity = jsonGetUInt(jdoc[MoMhjdevicebase::idsecurity()]);
			obj->deviceType = jsonGetUInt64(jdoc[MoMhjdevicebase::deviceType()]);
			memcpy(obj->security, jdoc[MoMhjdevicebase::security()].asCString(), SECURITYSIZE);
			obj->createDate = jsonGetPTime(jdoc[MoMhjdevicebase::createDate()]);
			obj->createUser = jdoc[MoMhjdevicebase::createUser()].asString();
			obj->createVerification = jdoc[MoMhjdevicebase::createVerification()].asBool();
			obj->productionDate = jsonGetPTime(jdoc[MoMhjdevicebase::productionDate()]);
			obj->productionUser = jdoc[MoMhjdevicebase::productionUser()].asString();
			obj->activationDate = jsonGetPTime(jdoc[MoMhjdevicebase::activationDate()]);
			obj->deviceFlag = jsonGetInt(jdoc[MoMhjdevicebase::deviceFlag()]);
			obj->virtualKeyCount = jsonGetInt(jdoc[MoMhjdevicebase::virtualKeyCount()]);
			if (!jsonValueIsEmpty(jdoc[MoMhjdevicebase::virtualKeyDefine()])) {
				memcpy(obj->virtualKeyDefine, jdoc[MoMhjdevicebase::virtualKeyDefine()].asCString(), sizeof(obj->virtualKeyDefine));
			}
			obj->switchKeyfCount = jsonGetInt(jdoc[MoMhjdevicebase::switchKeyfCount()]);
			obj->serviceIp = jdoc[MoMhjdevicebase::serviceIp()].asString();
			obj->sn = jsonGetString(jdoc[MoMhjdevicebase::sn()]);
			obj->serverChnnel = jsonGetString(jdoc[MoMhjdevicebase::serverChnnel()]);
		}
	}
	return obj;
}

std::tr1::shared_ptr<MHJUserEntity> json2MHJUserEneity(Json::Value& jsonvalue) {
	std::tr1::shared_ptr < MHJUserEntity > obj;
	Json::Value jdoc;

	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			Json::ArrayIndex index = 0;
			jdoc = jsonvalue.get(index, Json::nullValue);
		} else
			jdoc = jsonvalue;
		if (jdoc == Json::nullValue)
			return obj;
		if (jdoc.type() == Json::objectValue) {
			MHJUserEntity*pMHJUserEntity = new MHJUserEntity;
			obj = std::tr1::shared_ptr < MHJUserEntity > (pMHJUserEntity);
			obj->id = jsonGetUInt64(jdoc[MoMhjUserEntity::id()]);
			obj->UserName = jdoc[MoMhjUserEntity::username()].asString();
			obj->Password = jdoc[MoMhjUserEntity::password()].asString();
//		obj->sex = jdoc[MoMhjUserEntity::sex()].asUInt64();
//		obj->phone = jdoc[MoMhjUserEntity::phone()].asUInt64();
//		obj->email = jdoc[MoMhjUserEntity::email()].asUInt64();
			obj->UserTitle = jdoc[MoMhjUserEntity::usertitle()].asString();
			memcpy(obj->userToken, jdoc[MoMhjUserEntity::userToken()].asCString(), SECURITYSIZE);
			obj->tokenReplaceTime = jsonGetUInt64(jdoc[MoMhjUserEntity::tokenReplaceTime()]);
//		obj->phoneAffirm = jdoc[MoMhjUserEntity::phoneAffirm()].asUInt64();
//		obj->activeYs = jdoc[MoMhjUserEntity::activeYs()].asUInt64();
//		obj->deviceToken = jdoc[MoMhjUserEntity::deviceToken()].asUInt64();
//		obj->pushType = jdoc[MoMhjUserEntity::pushType()].asUInt64();
//		obj->userType = jdoc[MoMhjUserEntity::userType()].asUInt64();
			obj->serviceIp = jdoc[MoMhjUserEntity::serviceIp()].asString();
			obj->serverChnnel = jsonGetString(jdoc[MoMhjUserEntity::serverChnnel()]);
		}
	}
	return obj;
}

std::tr1::shared_ptr<MHJDeviceChild> json2MHJDeviceChild(Json::Value& jsonvalue) {
	std::tr1::shared_ptr < MHJDeviceChild > obj;
	Json::Value jdoc;

	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			Json::ArrayIndex index = 0;
			jdoc = jsonvalue.get(index, Json::nullValue);
		} else
			jdoc = jsonvalue;
		if (jdoc == Json::nullValue)
			return obj;
		if (jdoc.type() == Json::objectValue) {
			MHJDeviceChild*pMHJDeviceChild = new MHJDeviceChild;
			obj = std::tr1::shared_ptr < MHJDeviceChild > (pMHJDeviceChild);
			obj->id_ = jsonGetUInt64(jdoc[MoMhjdevicechild::id()]);
			obj->deviceID = jsonGetUInt64(jdoc[MoMhjdevicechild::deviceID()]);
			obj->userID = jsonGetInt(jdoc[MoMhjdevicechild::userID()]);
			obj->keyID = jsonGetUInt(jdoc[MoMhjdevicechild::keyId()]);
			obj->Name = jdoc[MoMhjdevicechild::name()].asString();
			obj->KeyDefine = jsonGetUInt(jdoc[MoMhjdevicechild::keyDefine()]);
			obj->state = jsonGetUInt(jdoc[MoMhjdevicechild::state()]);
			obj->defineChangeTime = jsonGetPTime(jdoc[MoMhjdevicechild::defineChangeTime()]);
//		obj->defineChangeTime = jdoc[MoMhjdevicechild::defineChangeTime()].asUInt64();
		}
	}
	return obj;
}

std::tr1::shared_ptr<MHJDeviceActivation> json2MHJDeviceActivation(Json::Value& jsonvalue) {
	std::tr1::shared_ptr < MHJDeviceActivation > obj;
	Json::Value jdoc;

	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			Json::ArrayIndex index = 0;
			jdoc = jsonvalue.get(index, Json::nullValue);
		} else
			jdoc = jsonvalue;
		if (jdoc == Json::nullValue)
			return obj;
		if (jdoc.type() == Json::objectValue) {
			MHJDeviceActivation*pMHJDeviceActivation = new MHJDeviceActivation;
			obj = std::tr1::shared_ptr < MHJDeviceActivation > (pMHJDeviceActivation);
			obj->id_ = jsonGetUInt64(jdoc[MoMhjdeviceactivation::id()]);
			obj->deviceID = jsonGetUInt64(jdoc[MoMhjdeviceactivation::deviceID()]);
			obj->userID = jsonGetInt(jdoc[MoMhjdeviceactivation::userID()]);
			obj->Date = jsonGetPTime(jdoc[MoMhjdeviceactivation::date()]);
			obj->ip = jsonGetString(jdoc[MoMhjdeviceactivation::ip()]);
			obj->mac = jsonGetString(jdoc[MoMhjdeviceactivation::mac()]);
			obj->DeviceName = jsonGetString(jdoc[MoMhjdeviceactivation::deviceName()]);
			obj->softVer = jsonGetString(jdoc[MoMhjdeviceactivation::softVer()]);
			obj->newSoftUrl = jsonGetString(jdoc[MoMhjdeviceactivation::newSoftUrl()]);
			obj->newSoftVer = jsonGetString(jdoc[MoMhjdeviceactivation::newSoftVer()]);
		}
	}
	return obj;
}

bool json2MHJDeviceAuthorizeVector(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> >* vector) {

#ifdef DEBUG
	Json::FastWriter fwriter;
#endif
	std::tr1::shared_ptr < MHJDeviceAuthorize > obj;
	bool ret = false;
	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			for (Json::ArrayIndex index = 0; index < jsonvalue.size(); index++) {
				Json::Value jdoc = jsonvalue[index];
				if (jdoc.type() == Json::objectValue) {
#ifdef DEBUG
					std::string jsonstr=fwriter.write(jdoc);
#endif
					MHJDeviceAuthorize*pMHJDeviceActivation = new MHJDeviceAuthorize;
					obj = std::tr1::shared_ptr < MHJDeviceAuthorize > (pMHJDeviceActivation);
					obj->id_ = jsonGetUInt64(jdoc[MoMhjdeviceauthorize::id()]);
					obj->giveUserID = jsonGetUInt(jdoc[MoMhjdeviceauthorize::giveUserID()]);
					obj->deviceID = jsonGetUInt64(jdoc[MoMhjdeviceauthorize::deviceID()]);
					obj->userID = jsonGetUInt(jdoc[MoMhjdeviceauthorize::userID()]);
					obj->childID = jsonGetUInt64(jdoc[MoMhjdeviceauthorize::childID()]);
					//TODO 0 时间转换会出现内存分配错误，原因未知
					obj->startDate = jsonGetPTime(jdoc[MoMhjdeviceauthorize::startDate()]);
					obj->endDate = jsonGetPTime(jdoc[MoMhjdeviceauthorize::endDate()]);
					obj->giveType = jsonGetInt(jdoc[MoMhjdeviceauthorize::giveType()]);

					ret = true;
					vector->push_back(obj);
				}
			}
		}
	}
	return ret;
}

bool json2UserEntityList(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJUserEntity> >* vector) {
	bool ret = false;
	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			for (Json::ArrayIndex index = 0; index < jsonvalue.size(); index++) {
				Json::Value jdoc = jsonvalue[index];
				if (jdoc.type() == Json::objectValue) {
					MHJUserEntity*pMHJUserEntity = new MHJUserEntity;
					std::tr1::shared_ptr < MHJUserEntity > obj(pMHJUserEntity);
					obj->id = jsonGetUInt64(jdoc[MoMhjUserEntity::id()]);
					obj->UserName = jdoc[MoMhjUserEntity::username()].asString();
					obj->Password = jdoc[MoMhjUserEntity::password()].asString();
//		obj->sex = jdoc[MoMhjUserEntity::sex()].asUInt64();
//		obj->phone = jdoc[MoMhjUserEntity::phone()].asUInt64();
//		obj->email = jdoc[MoMhjUserEntity::email()].asUInt64();
					obj->UserTitle = jdoc[MoMhjUserEntity::usertitle()].asString();
					memcpy(obj->userToken, jdoc[MoMhjUserEntity::userToken()].asCString(), SECURITYSIZE);
					obj->tokenReplaceTime = jsonGetUInt64(jdoc[MoMhjUserEntity::tokenReplaceTime()]);
//		obj->phoneAffirm = jdoc[MoMhjUserEntity::phoneAffirm()].asUInt64();
//		obj->activeYs = jdoc[MoMhjUserEntity::activeYs()].asUInt64();
//		obj->deviceToken = jdoc[MoMhjUserEntity::deviceToken()].asUInt64();
//		obj->pushType = jdoc[MoMhjUserEntity::pushType()].asUInt64();
//		obj->userType = jdoc[MoMhjUserEntity::userType()].asUInt64();
					vector->push_back(obj);
					ret = true;
				}
			}
		}
	}
	return ret;
}

bool json2MHJDeviceChilds(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceChild> >* vector) {

	bool ret = false;
	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			for (Json::ArrayIndex index = 0; index < jsonvalue.size(); index++) {
				Json::Value jdoc = jsonvalue[index];
				if (jdoc.type() == Json::objectValue) {
					MHJDeviceChild*pMHJDeviceChild = new MHJDeviceChild;
					std::tr1::shared_ptr < MHJDeviceChild > obj(pMHJDeviceChild);
//								obj = std::tr1::shared_ptr < MHJDeviceChild > (pMHJDeviceChild);
					obj->id_ = jsonGetUInt64(jdoc[MoMhjdevicechild::id()]);
					obj->deviceID = jsonGetUInt64(jdoc[MoMhjdevicechild::deviceID()]);
					obj->userID = jsonGetInt(jdoc[MoMhjdevicechild::userID()]);
					obj->keyID = jsonGetUInt(jdoc[MoMhjdevicechild::keyId()]);
					obj->Name = jdoc[MoMhjdevicechild::name()].asString();
					obj->KeyDefine = jsonGetUInt(jdoc[MoMhjdevicechild::keyDefine()]);
					obj->state = jsonGetUInt(jdoc[MoMhjdevicechild::state()]);
					obj->defineChangeTime = jsonGetPTime(jdoc[MoMhjdevicechild::defineChangeTime()]);
					vector->push_back(obj);
					ret = true;
				}
			}
		}
	}
	return ret;
}

bool json2MHJDeviceVirtualDefines(Json::Value& jsonvalue, std::vector<std::tr1::shared_ptr<MHJDeviceVirtualDefine> >* vector) {
#ifdef DEBUG
	Json::FastWriter fwriter;
#endif
	bool ret = false;
	if (!jsonvalue.isNull()) {
		if (jsonvalue.isArray()) {
			for (Json::ArrayIndex index = 0; index < jsonvalue.size(); index++) {
				Json::Value jdoc = jsonvalue[index];
				if (jdoc.type() == Json::objectValue) {
#ifdef DEBUG
					std::string jsonstr=fwriter.write(jdoc);
#endif
					MHJDeviceVirtualDefine*pMHJDeviceVirtualDefine = new MHJDeviceVirtualDefine;
					std::tr1::shared_ptr < MHJDeviceVirtualDefine > obj(pMHJDeviceVirtualDefine);
//								obj = std::tr1::shared_ptr < MHJDeviceChild > (pMHJDeviceChild);
					obj->id_ = jsonGetUInt64(jdoc[MoMHJDeviceVirtualDefine::id()]);
					obj->targetKeyID = jsonGetUInt64(jdoc[MoMHJDeviceVirtualDefine::targetKeyID()]);
					obj->deviceKeyID = jsonGetUInt64(jdoc[MoMHJDeviceVirtualDefine::deviceKeyID()]);
					obj->targetNewState = jsonGetUInt(jdoc[MoMHJDeviceVirtualDefine::targetNewState()]);
					obj->defineTime = jsonGetPTime(jdoc[MoMHJDeviceVirtualDefine::defineTime()]);

					vector->push_back(obj);
					ret = true;
				}
			}
		}
	}
	return ret;
}
