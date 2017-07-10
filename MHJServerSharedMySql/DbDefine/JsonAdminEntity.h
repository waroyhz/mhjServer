/*
 * JsonAdminEntity.h
 *
 *  Created on: 2015年10月14日
 *      Author: ruancongming
 */

#ifndef JSONADMINENTITY_H_
#define JSONADMINENTITY_H_
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/config.h>
#include <json/forwards.h>
#include <json/autolink.h>
#include <json/features.h>
#include "MHJDatabase.hxx"
#include "MoAdminEntity.h"  //the rule of the key
class JsonAdminEntity {
public:
	void Obj2JsonStr(std::tr1::shared_ptr<MHJAdminEntity> obj, std::string *str);
	void JsonStr2Obj(std::string *str, std::tr1::shared_ptr<MHJAdminEntity> obj);

};

#endif /* JSONADMINENTITY_H_ */
