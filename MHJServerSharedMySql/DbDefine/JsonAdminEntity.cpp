/*
 * JsonAdminEntity.cpp
 *
 *  Created on: 2015年10月14日
 *      Author: ruancongming
 */

#include "JsonAdminEntity.h"

void JsonAdminEntity::Obj2JsonStr(std::tr1::shared_ptr<MHJAdminEntity>obj,std::string *JsonStr){
	Json::Value item;

//	char tmp[81];
//	sprintf(tmp,"%d",obj->id);
//	item[MoAdminEntity::id()] = tmp;
	item[MoAdminEntity::id()] = (Json::UInt64)obj->id;
	item[MoAdminEntity::username()] = obj->UserName;
	item[MoAdminEntity::password()] = obj->Password;
//	sprintf(tmp,"%s",obj->adminToken);
//	item[MoAdminEntity::usertoken()] = tmp;
//	item[MoAdminEntity::tokenreplacetime()] = obj->tokenReplaceTime;

	JsonStr->assign(item.toStyledString());  //save to string

}

void JsonAdminEntity::JsonStr2Obj(std::string *str,std::tr1::shared_ptr<MHJAdminEntity>obj){

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(str->c_str(), root))  // reader将Json格式字符串解析到root，root将包含Json格式字符串里所有子元素
    {
    	obj->id = root[MoAdminEntity::id()].asUInt64();
    	obj->UserName = root[MoAdminEntity::username()].asString();
    	obj->Password = root[MoAdminEntity::password()].asString();
    }

}
