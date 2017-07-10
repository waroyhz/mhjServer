/*
 * MoAdminEntity.h
 *
 *  Created on: 2015年11月05日
 *      Author: waroy
 */

#ifndef MoMHJDeviceVirtualDefine_H_
#define MoMHJDeviceVirtualDefine_H_

class MoMHJDeviceVirtualDefine {
public:
	/**
	 * Mhjdevicevirtualdefine entity. @author MyEclipse Persistence Tools
	 @Entity
	 @Table(name = "mhjdevicevirtualdefine", catalog = "mhj")
	 type:	Long
	 */
	static std::string id() {
		return std::string("id");
	}
	/**
	 type:	Mhjdevicechild
	 */
	static std::string targetKey() {
		return std::string("targetKey");
	}
	static std::string targetKeyID() {
		return std::string("targetKey_PostData");
	}
	/**
	 type:	Mhjdevicechild
	 */
	static std::string deviceKey() {
		return std::string("deviceKey");
	}
	static std::string deviceKeyID() {
		return std::string("deviceKey_PostData");
	}
	/**
	 type:	Integer
	 */
	static std::string targetNewState() {
		return std::string("targetNewState");
	}
	/**
	 type:	Timestamp
	 */
	static std::string defineTime() {
		return std::string("defineTime");
	}

	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return std::string("mhjdevicevirtualdefine");
	}
	/**
	 type:	Mhjdevicechild
	 */
// static std::string mhjdevicechildByTargetKey(MoMhjdevicechild value) {
//return new std::string(mhjdevicechildByTargetKey.toString()+"." + value.toString());}
//}
	/**
	 type:	Mhjdevicechild
	 */
//public static std::string mhjdevicechildByDeviceKey(MoMhjdevicechild value) {
//return new std::string(mhjdevicechildByDeviceKey.toString()+"." + value.toString());}
//}
};

#endif /* MOADMINENTITY_H_ */
