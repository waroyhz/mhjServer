/*
 * MoAdminEntity.h
 *
 *  Created on: 2015年10月13日
 *      Author: waroy
 */

#ifndef MoMhjdevicechild_H_
#define MoMhjdevicechild_H_

class MoMhjdevicechild {
public:
	/**
	 * Mhjdevicechild entity. @author MyEclipse Persistence Tools
	 @Entity
	 @Table(name = "mhjdevicechild", catalog = "mhj")
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
	 type:	Long
	 */
	static std::string keyId() {
		return std::string("keyId");
	}
	/**
	 type:	String
	 */
	static std::string name() {
		return std::string("name");
	}
	/**
	 type:	Integer
	 */
	static std::string keyDefine() {
		return std::string("keyDefine");
	}
	/**
	 type:	Integer
	 */
	static std::string state() {
		return std::string("state");
	}
	/**
	 type:	Timestamp
	 */
	static std::string defineChangeTime() {
		return std::string("defineChangeTime");
	}
	/**
	 type:	Set<Mhjdeviceauthorize>
	 */
	static std::string mhjdeviceauthorizes() {
		return std::string("mhjdeviceauthorizes");
	}
	/**
	 type:	Set<Mhjdevicevirtualdefine>
	 */
	static std::string mhjdevicevirtualdefinesForDeviceKey() {
		return std::string("mhjdevicevirtualdefinesForDeviceKey");
	}
	/**
	 type:	Set<Mhjdevicevirtualdefine>
	 */
	static std::string mhjdevicevirtualdefinesForTargetKey() {
		return std::string("mhjdevicevirtualdefinesForTargetKey");
	}

	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return std::string("mhjdevicechild");
	}
	/**
	 type:	Mhjdevicebase
	 */
//public static std::string mhjdevicebase(MoMhjdevicebase value) {
//return new std::string(mhjdevicebase.toString()+"." + value.toString());}
//}
//
///**
// type:	UserEntity
//*/
//public static std::string userEntity(MoUserEntity value) {
//return new std::string(userEntity.toString()+"." + value.toString());}
//}
//
///**
// type:	Set<Mhjdeviceauthorize>
//*/
//public static std::string mhjdeviceauthorizes(MoMhjdeviceauthorize value) {
//return new std::string(mhjdeviceauthorizes.toString()+"." + value.toString());}
//}
//
///**
// type:	Set<Mhjdevicevirtualdefine>
//*/
//public static std::string mhjdevicevirtualdefinesForDeviceKey(MoMhjdevicevirtualdefine value) {
//return new std::string(mhjdevicevirtualdefinesForDeviceKey.toString()+"." + value.toString());}
//}
//
///**
// type:	Set<Mhjdevicevirtualdefine>
//*/
//public static std::string mhjdevicevirtualdefinesForTargetKey(MoMhjdevicevirtualdefine value) {
//return new std::string(mhjdevicevirtualdefinesForTargetKey.toString()+"." + value.toString());}
//}
};

#endif /* MOADMINENTITY_H_ */
