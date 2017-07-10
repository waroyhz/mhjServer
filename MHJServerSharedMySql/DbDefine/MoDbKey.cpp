/*
 * MoDbKey.cpp
 *
 *  Created on: 2015年10月8日
 *      Author: ruancongming
 */

#include <MoDbKey.h>
#include <stdio.h>

MoDbKey::MoDbKey() {
	// TODO Auto-generated constructor stub

}

MoDbKey::~MoDbKey() {
	// TODO Auto-generated destructor stub
}

std::string MoDbKey::getModelKey(std::string modelName, uint32_t dbID) {
	char tmp[255];
	sprintf(tmp,"%s_%d_idmap",modelName.c_str(),dbID);
	return std::string(tmp);
}

std::string MoDbKey::getModelPropertyKey(std::string modelName,
		std::string property, uint32_t propertyID) {
	char tmp[255];
		sprintf(tmp,"%s_%s_%d_idlist",modelName.c_str(),property.c_str(),propertyID);
		return std::string(tmp);
}
