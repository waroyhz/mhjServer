/*
 * MoAdminEntity.h
 *
 *  Created on: 2015年10月13日
 *      Author: waroy
 */

#ifndef MoMhjdeviceactivation_H_
#define MoMhjdeviceactivation_H_

class MoMhjdeviceactivation {
public:
	/**
	 * Mhjdeviceactivation entity. @author MyEclipse Persistence Tools
	 @Entity
	 @Table(name = "mhjdeviceactivation", catalog = "mhj")
	 type:	Long
	 */
	static std::string id() {
		return std::string("id");
	}
	/**
	 type:	Mhjdevicebase
	 */
	static std::string device() {
		return std::string("device");
	}
	static std::string deviceID() {
		return std::string("device_PostData");
	}

	/**
	 type:	UserEntity
	 */
	static std::string user() {
		return std::string("user");
	}
	static std::string userID() {
		return std::string("user_PostData");
	}
	/**
	 type:	Timestamp
	 */
	static std::string date() {
		return std::string("date");
	}
	/**
	 type:	String
	 */
	static std::string ip() {
		return std::string("ip");
	}
	/**
	 type:	String
	 */
	static std::string mac() {
		return std::string("mac");
	}
	/**
	 type:	String
	 */
	static std::string deviceName() {
		return std::string("deviceName");
	}
	/**
	 type:	String
	 */
	static std::string softVer() {
		return std::string("softVer");
	}
	/**
	 type:	String
	 */
	static std::string newSoftUrl() {
		return std::string("newSoftUrl");
	}
	/**
	 type:	String
	 */
	static std::string newSoftVer() {
		return std::string("newSoftVer");
	}

	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return std::string("mhjdeviceactivation");
	}
	/**
	 type:	Mhjdevicebase
	 */
// static std::string mhjdevicebase(MoMhjdevicebase value) {
//		return new std::string(mhjdevicebase.toString()+"." + value.toString());}
//	}
	/**
	 type:	UserEntity
	 */
//public static std::string userEntity(MoUserEntity value) {
//		return new std::string(userEntity.toString()+"." + value.toString());}
//	}
};

#endif /* MOADMINENTITY_H_ */
