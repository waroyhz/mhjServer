/*
 * MoAdminEntity.h
 *
 *  Created on: 2015年10月13日
 *      Author: waroy
 */

#ifndef MoMhjdeviceauthorize_H_
#define MoMhjdeviceauthorize_H_

class MoMhjdeviceauthorize {
public:
	/**
	 * Mhjdeviceauthorize entity. @author MyEclipse Persistence Tools
	 @Entity
	 @Table(name = "mhjdeviceauthorize", catalog = "mhj")
	 type:	Long
	 */
	static std::string id() {
		return std::string("id");
	}
	/**
	 type:	UserEntity
	 */
	static std::string giveUser() {
		return std::string("giveUser");
	}
	static std::string giveUserID() {
		return std::string("giveUser_PostData");
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
	 type:	Mhjdevicechild
	 */
	static std::string child() {
		return std::string("child");
	}
	static std::string childID() {
		return std::string("child_PostData");
	}
	/**
	 type:	Timestamp
	 */
	static std::string startDate() {
		return std::string("startDate");
	}
	/**
	 type:	Timestamp
	 */
	static std::string endDate() {
		return std::string("endDate");
	}
	/**
	 type:	Integer
	 */
	static std::string giveType() {
		return std::string("giveType");
	}

	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return std::string("mhjdeviceauthorize");
	}
	/**
	 type:	UserEntity
	 */
// static std::string userEntityByGiveUser(MoUserEntity value) {
//		return new std::string(userEntityByGiveUser.toString()+"." + value.toString());}
//	}
	/**
	 type:	Mhjdevicebase
	 */
//public static std::string mhjdevicebase(MoMhjdevicebase value) {
//		return new std::string(mhjdevicebase.toString()+"." + value.toString());}
//	}
	/**
	 type:	UserEntity
	 */
//public static std::string userEntityByUser(MoUserEntity value) {
//		return new std::string(userEntityByUser.toString()+"." + value.toString());}
//	}
	/**
	 type:	Mhjdevicechild
	 */
//public static std::string mhjdevicechild(MoMhjdevicechild value) {
//		return new std::string(mhjdevicechild.toString()+"." + value.toString());}
//	}
};

#endif /* MOADMINENTITY_H_ */
