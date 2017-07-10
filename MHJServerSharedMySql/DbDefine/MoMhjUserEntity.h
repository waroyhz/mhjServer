/*
 * MoMhjdevicebase.h
 *
 *  Created on: 2015年10月8日
 *      Author: ruancongming
 */

#ifndef MOMHJUSERENTITY_H_
#define MOMHJUSERENTITY_H_

#include <string>

class MoMhjUserEntity {
public:
	MoMhjUserEntity() {
	}
	virtual ~MoMhjUserEntity() {
	}
public:
	/**
	 * UserEntity entity. @author MyEclipse Persistence Tools
	 @SuppressWarnings("serial")
	 @Entity
	 @Table(name = "user_entity")
	 @Required
	 @ModelProprtyName("id")
	 type:	Integer
	 */
	static std::string id() {
		return std::string("id");
	}
	/**
	 @DisplayName("用户名")
	 @ModelProprtyName("username")
	 type:	String
	 */
	static std::string username() {
		return std::string("username");
	}
	/**
	 @DisplayName("密码")
	 @ModelProprtyName("password")
	 type:	String
	 */
	static std::string password() {
		return std::string("password");
	}
	/**
	 @DisplayName("性别")
	 @ModelProprtyName("sex")
	 type:	Integer
	 */
	static std::string sex() {
		return std::string("sex");
	}
	/**
	 @DisplayName("手机号")
	 @ModelProprtyName("phone")
	 type:	String
	 */
	static std::string phone() {
		return std::string("phone");
	}
	/**
	 @DisplayName("邮箱")
	 @ModelProprtyName("email")
	 type:	String
	 */
	static std::string email() {
		return std::string("email");
	}
	/**
	 @DisplayName("用户昵称")
	 @ModelProprtyName("usertitle")
	 type:	String
	 */
	static std::string usertitle() {
		return std::string("usertitle");
	}
	/**
	 @HCMaxLength(255)
	 @HCMinLength(0)
	 @DisplayName("用户免登陆令牌")
	 @ModelProprtyName("userToken")
	 type:	String
	 */
	static std::string userToken() {
		return std::string("userToken");
	}
	/**
	 @DisplayName("令牌更换时间")
	 @ModelProprtyName("tokenReplaceTime")
	 type:	Long
	 */
	static std::string tokenReplaceTime() {
		return std::string("tokenReplaceTime");
	}
	/**
	 @DisplayName("手机验证")
	 @ModelProprtyName("phoneAffirm")
	 type:	Boolean
	 */
	static std::string phoneAffirm() {
		return std::string("phoneAffirm");
	}
	/**
	 @DisplayName("激活萤石")
	 @ModelProprtyName("activeYS")
	 type:	Boolean
	 */
	static std::string activeYs() {
		return std::string("activeYs");
	}
	/**
	 @DisplayName("设备令牌")
	 @ModelProprtyName("deviceToken")
	 type:	String
	 */
	static std::string deviceToken() {
		return std::string("deviceToken");
	}
	/**
	 @DisplayName("推送类型，1android，2IOS")
	 @ModelProprtyName("pushType")
	 type:	Integer
	 */
	static std::string pushType() {
		return std::string("pushType");
	}
	/**
	 @DisplayName("用户类型，0/null普通用户1小区管理员")
	 @ModelProprtyName("userType")
	 type:	Integer
	 */
//	static std::string userType() {
//		return std::string("userType");
//	}
	static std::string serviceIp() {
		return std::string("serviceIp");
	}

//	@DisplayName("通道")
//	@ModelProprtyName("serverChnnel")
	static std::string serverChnnel() {
		return std::string("serverChnnel");
	}
	/**
	 *数据库的表名
	 */
	static std::string getTableName() {
		return "user_entity";
	}

};

#endif /* MOMHJDEVICEBASE_H_ */
