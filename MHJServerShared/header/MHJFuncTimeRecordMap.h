/*
 * MHJFuncTimeRecordMap.h
 *
 *  Created on: 2015年11月10日
 *      Author: ruancongming
 */

#ifndef MHJFUNCTIMERECORDMAP_H_
#define MHJFUNCTIMERECORDMAP_H_

class MHJFuncTimeRecordMap{
public:
	static  std::string getTimeString(char* function_name, char* current, int time){
		char temp[128];
		sprintf(temp,"%s_%s_%d",function_name,current,time);
		return std::string(temp);
	}

};



#endif /* MHJFUNCTIMERECORDMAP_H_ */
