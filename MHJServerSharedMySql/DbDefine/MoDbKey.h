/*
 * MoDbKey.h
 *
 *  Created on: 2015年10月8日
 *      Author: ruancongming
 */

#ifndef MODBKEY_H_
#define MODBKEY_H_

#include <string>
#include <stdint.h>

class MoDbKey {
public:
	MoDbKey();
	virtual ~MoDbKey();

	static std::string getModelKey(std::string modelName,uint32_t dbID);
	static std::string getModelPropertyKey(std::string modelName,std::string property,uint32_t propertyID);

};

#endif /* MODBKEY_H_ */
