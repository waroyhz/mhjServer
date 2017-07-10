/*
 * MoMhjdevicebase.h
 *
 *  Created on: 2015年10月8日
 *      Author: ruancongming
 */

#ifndef MOMHJDEVICEBASE_H_
#define MOMHJDEVICEBASE_H_

#include <string>

class MoMhjdevicebase {
public:
	MoMhjdevicebase() {
	}
	virtual ~MoMhjdevicebase() {
	}

	/**
	 * Mhjdevicebase entity. @author MyEclipse Persistence Tools
	 @Entity
	 @Table(name="mhjdevicebase"
	 type:	Long
	 */
	static std::string id() {
		return std::string("id");
	}
	/**
	 type:	Integer
	 */
	static std::string deviceId() {
		return std::string("deviceId");
	}
	/**
	 type:	Integer
	 */
	static std::string idsecurity() {
		return std::string("idsecurity");
	}
	/**
	 type:	Integer
	 */
	static std::string deviceType() {
		return std::string("deviceType");
	}
	/**
	 type:	String
	 */
	static std::string security() {
		return std::string("security");
	}
	/**
	 type:	Timestamp
	 */
	static std::string createDate() {
		return std::string("createDate");
	}
	/**
	 type:	String
	 */
	static std::string createUser() {
		return std::string("createUser");
	}
	/**
	 type:	Boolean
	 */
	static std::string createVerification() {
		return std::string("createVerification");
	}
	/**
	 type:	Timestamp
	 */
	static std::string productionDate() {
		return std::string("productionDate");
	}
	/**
	 type:	String
	 */
	static std::string productionUser() {
		return std::string("productionUser");
	}
	/**
	 type:	Timestamp
	 */
	static std::string activationDate() {
		return std::string("activationDate");
	}
	/**
	 type:	Integer
	 */
	static std::string deviceFlag() {
		return std::string("deviceFlag");
	}
	/**
	 type:	Integer
	 */
	static std::string virtualKeyCount() {
		return std::string("virtualKeyCount");
	}
	/**
	 type:	String
	 */
	static std::string virtualKeyDefine() {
		return std::string("virtualKeyDefine");
	}
	/**
	 type:	Integer
	 */
	static std::string switchKeyfCount() {
		return std::string("switchKeyfCount");
	}
	/**
	 @DisplayName("指定服务器地址")
	 @ModelProprtyName("ServiceIp")
	 type:	String
	 */
	static std::string serviceIp() {
		return std::string("serviceIp");
	}

	static std::string sn() {
		return std::string("sn");
	}

//	@DisplayName("通道")
//	@ModelProprtyName("serverChnnel")
	static std::string serverChnnel() {
		return std::string("serverChnnel");
	}

	/**
	 type:	Set<Mhjdevicechild>
	 */
	static std::string mhjdevicechilds() {
		return std::string("mhjdevicechilds");
	}
	/**
	 type:	Set<Mhjdeviceauthorize>
	 */
	static std::string mhjdeviceauthorizes() {
		return std::string("mhjdeviceauthorizes");
	}
	/**
	 type:	Set<Mhjdeviceactivation>
	 */
	static std::string mhjdeviceactivations() {
		return std::string("mhjdeviceactivations");
	}
	/**
	 *初始化函数
	 */
// std::string(String value) {
//super(value);
//}
	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return std::string("Mhjdevicebase");
	}
	/**
	 type:	Set<Mhjdevicechild>
	 */
	static std::string mhjdevicechilds(std::string value) {
		return std::string(mhjdevicechilds() + "." + value);
	}

	/**
	 type:	Set<Mhjdeviceauthorize>
	 */
	static std::string mhjdeviceauthorizes(std::string value) {
		return std::string(mhjdeviceauthorizes() + "." + value);

	}

	/**
	 type:	Set<Mhjdeviceactivation>
	 */
	static std::string mhjdeviceactivations(std::string value) {
		return std::string(mhjdeviceactivations() + "." + value);
	}

};

#endif /* MOMHJDEVICEBASE_H_ */
