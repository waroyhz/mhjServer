// file      : hello/person.hxx
// copyright : not copyrighted - public domain

#ifndef PERSON_HXX
#define PERSON_HXX

#include <string>
#include <cstddef> // std::size_t
#include <boost/date_time.hpp>
#ifdef SQLITE
#include <odb/core.hxx>
#endif
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#ifdef SQLITE
#include <odb/tr1/lazy-ptr.hxx>
#endif
#ifdef LINUX
#include "ProtocolTypeDefine.h"
#endif

#ifdef SQLITE
#include <odb/boost/date-time/sqlite/posix-time-traits.hxx>
#else
#if (defined LINUX) && (defined SQLITE)
#include <odb/boost/date-time/mysql/posix-time-traits.hxx>
#endif
#endif

class MHJDeviceActivation;
class MHJUserEntity;

#ifdef SQLITE
#pragma db model version(1, 25)
#pragma db object pointer(std::tr1::shared_ptr)
#endif

class MHJDeviceBase {
public:
#ifdef SQLITE
	friend class odb::access;
#endif

#pragma db id auto
	unsigned long id_;

#pragma db type("INT")
#pragma db index
	//设备id
	unsigned int deviceID;

	//设备ID校验码
//#pragma db type("CHAR(4)")
#pragma db null
#pragma db index
#pragma db type("INT")
	uint16_t idsecurity;
	//设备类型
#pragma db type("INT")
#pragma db index
	uint16_t deviceType;

#pragma db null
#pragma db type("INT")
	uint16_t virtualKeyCount;

#pragma db null
#pragma db type("CHAR(255)")
	char virtualKeyDefine[16];
//  const unsigned char* virtualKeyDefine;

#pragma db null
#pragma db type("INT")
	uint16_t switchKeyfCount;

	//设备安全码
#pragma db type("CHAR(32)")
#pragma db null
	char security[32];
	//创建时间,指芯片程序灌装日期
#pragma db type("DATETIME")
	boost::posix_time::ptime createDate;
	//创建时的工号
	std::string createUser;
	//程序验证写入标志。
#pragma db null
	bool createVerification;
	//生产时间,工厂生产时间
#pragma db type("DATETIME")
#pragma db null
	boost::posix_time::ptime productionDate;
	//生产工号
#pragma db null
	std::string productionUser;
	//激活时间，用户首次激活的时间（保修开始时间）
#pragma db type("DATETIME")
#pragma db null
	boost::posix_time::ptime activationDate;

////激活信息 需要处理odb的懒加载
//#pragma db inverse(device)
//  std::tr1::weak_ptr<MHJDeviceActivation> deviceActivation;
	//设备标志
#pragma db index
#pragma db null
	int deviceFlag;

	//	@DisplayName("指定服务器地址")
	//	@ModelProprtyName("ServiceIp")
	std::string serviceIp;
//	@DisplayName("整机序列号")
//	@ModelProprtyName("sn")
	std::string sn;
//	@DisplayName("通道")
//	@ModelProprtyName("serverChnnel")
	std::string serverChnnel;

#ifdef LINUX
	void getMark(MHJDeviceMark* outMark) {
		outMark->deviceID = deviceID;
		outMark->deviceType = deviceType;
		outMark->idSecurity1 = idsecurity & 0xff;
		outMark->idSecurity2 = idsecurity >> 8;
	}
#endif
};

//开关子项定义
#ifdef SQLITE
#pragma db object pointer(std::tr1::shared_ptr) //session
#endif
class MHJDeviceChild {
public:
	MHJDeviceChild() {
	}
	;
public:
#ifdef SQLITE
	friend class odb::access;
#endif

#pragma db id auto
	unsigned long id_;

#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJDeviceBase> device;
#pragma db column("device")
#pragma db type("BIGINT UNSIGNED")
	long deviceID;

#pragma db index
	unsigned long keyID;

#pragma db type("TEXT")
	std::string Name;

#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJUserEntity> user;
#pragma db column("user")
#pragma db type("INT(11)")
	int userID;

#pragma db null
	unsigned int KeyDefine;

#pragma db null
	unsigned int state;

#pragma db type("DATETIME") null
	boost::posix_time::ptime defineChangeTime;

	//用于APP记录显示的图片。
#pragma db type("TEXT") null
	std::string ImgIco;
};

#ifdef SQLITE
#pragma db object pointer(std::tr1::shared_ptr) //session
#endif
class MHJDeviceVirtualDefine {
public:
	MHJDeviceVirtualDefine() {
	}
	;
public:
#ifdef SQLITE
	friend class odb::access;
#endif

#pragma db id auto
	unsigned long id_;

#pragma db index
#pragma db not_null
//	std::tr1::shared_ptr<MHJDeviceChild> Key;
#pragma db column("deviceKey")
	unsigned long deviceKeyID;

#pragma db index
#pragma db not_null
//	std::tr1::shared_ptr<MHJDeviceChild> targetKey;
#pragma db column("targetKey")
	unsigned long targetKeyID;

#pragma db not_null
	unsigned int targetNewState;

#pragma db type("DATETIME")
	boost::posix_time::ptime defineTime;
};

//激活表
#ifdef SQLITE
#pragma db object pointer(std::tr1::shared_ptr)
#endif

class MHJDeviceActivation {
public:
	MHJDeviceActivation() {
	}

public:
#ifdef SQLITE
	friend class odb::access;
#endif

#pragma db id auto
	unsigned long id_;

#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJDeviceBase> device;
#pragma db column("device")
#pragma db type("BIGINT UNSIGNED")
	long deviceID;

	//激活时间
#pragma db type("DATETIME")
	boost::posix_time::ptime Date;
	std::string ip;
#pragma db null
	std::string mac;

#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJUserEntity> user;
#pragma db column("user")
#pragma db type("INT(11)")
	int userID;

	//用户给设备起的名字
#pragma db null
	std::string DeviceName;

	//软件版本
#pragma db null
	std::string softVer;

	//新版本的路径
#pragma db null
	std::string newSoftUrl;

	//当前版本标识
#pragma db null
	std::string newSoftVer;
};

//设备授权表
#ifdef SQLITE
#pragma db object pointer(std::tr1::shared_ptr) //session
#endif
class MHJDeviceAuthorize {

public:
#ifdef SQLITE
	friend class odb::access;
#endif

#pragma db id auto
	unsigned long id_;

#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJDeviceBase> device;
#pragma db column("device")
#pragma db type("BIGINT UNSIGNED")
	long deviceID;

#pragma db index
#pragma db not_null
//	std::tr1::shared_ptr<MHJDeviceChild> child;
#pragma db column("child")
#pragma db type("BIGINT UNSIGNED")
	long childID;

	//开始时间
#pragma db type("DATETIME")
	boost::posix_time::ptime startDate;
	//结束时间
#pragma db type("DATETIME")
	boost::posix_time::ptime endDate;
	//拥有的用户
#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJUserEntity> user;
#pragma db column("user")
#pragma db type("INT(11)")
	int userID;

	//赋予用户
#pragma db index
#pragma db not_null
//  std::tr1::shared_ptr<MHJUserEntity> giveUser;
#pragma db column("giveUser")
#pragma db type("INT(11)")
	int giveUserID;

	//赋予的类型
	int giveType;
};

//用户表
//#pragma db object pointer(std::tr1::shared_ptr) table("user_entity") //session
#ifdef SQLITE
#pragma db view pointer(std::tr1::shared_ptr) table("user_entity")
#endif
struct MHJUserEntity {
public:
#pragma db column("id") type("INT(11)")
	unsigned long id;

#pragma db column("UserName") type("LONGTEXT")
	std::string UserName;

#pragma db column("usertitle") type("LONGTEXT")
	std::string UserTitle;

#pragma db column("password") type("LONGTEXT")
	std::string Password;

#pragma db column("userToken") type("CHAR(255)")
	char userToken[32];

#pragma db column("tokenReplaceTime") type("BIGINT(20)")
	long tokenReplaceTime;
	//	@DisplayName("指定服务器地址")
	//	@ModelProprtyName("ServiceIp")
	std::string serviceIp;
//	@DisplayName("通道")
//	@ModelProprtyName("serverChnnel")
	std::string serverChnnel;
};

//管理员表
//#pragma db object pointer(std::tr1::shared_ptr) table("admin_entity") //session
#pragma db view pointer(std::tr1::shared_ptr) table("admin_entity")
class MHJAdminEntity {
public:
#pragma db column("id") type("INT(11)")
	unsigned long id;

#pragma db column("UserName") type("LONGTEXT")
	std::string UserName;

//#pragma db column("UserTitle") type("LONGTEXT")
//	std::string UserTitle;

#pragma db column("Password") type("LONGTEXT")
	std::string Password;

#pragma db column("adminToken") type("CHAR(255)")
	char adminToken[32];

#pragma db column("tokenReplaceTime") type("DATETIME")
	boost::posix_time::ptime tokenReplaceTime;

};

//#pragma db view object(person)
//struct person_stat
//{
//  #pragma db column("count(" + person::id_ + ")")
//  std::size_t count;
//
//  #pragma db column("min(" + person::age_ + ")")
//  unsigned short min_age;
//
//  #pragma db column("max(" + person::age_ + ")")
//  unsigned short max_age;
//};

#endif // PERSON_HXX
