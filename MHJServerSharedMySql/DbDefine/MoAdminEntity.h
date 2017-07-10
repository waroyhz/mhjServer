/*
 * MoAdminEntity.h
 *
 *  Created on: 2015年10月13日
 *      Author: ruancongming
 */

#ifndef MOADMINENTITY_H_
#define MOADMINENTITY_H_

class MoAdminEntity {
public:

	static  std::string id(){ return std::string("id"); }
	static   std::string username(){ return  std::string("username"); }
//	static   std::string usertitle(){ return  std::string("UserTitle"); }
	static   std::string password(){ return  std::string("password"); }
	static   std::string admintoken(){ return  std::string("admintoken"); }
	static   std::string tokenreplacetime(){ return  std::string("tokenreplacetime"); }
	static  std::string sessionid(){ return std::string("sessionid"); }

	static  std::string authorityList(){ return  std::string("authorityList"); }


};



#endif /* MOADMINENTITY_H_ */
