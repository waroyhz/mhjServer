/*
 * ServiceNum2KeyMap.h
 *
 *  Created on: 2015年11月4日
 *      Author: ruancongming
 */

#ifndef SERVICENUM2KEYMAP_H_
#define SERVICENUM2KEYMAP_H_
class ServiceNum2KeyMap{
public:
	static  std::string iplist(char* serviceNum){
		char temp[64];
		sprintf(temp,"%s_%s",serviceNum,"IpList");
		return std::string(temp);
	}

	static  std::string port(char* serviceNum,char *ip){
		char temp[64];
		sprintf(temp,"%s_%s_%s",serviceNum,ip,"Port");
		return std::string(temp);
	}

	static  std::string subscribe(char* serviceNum){
		char temp[64];
		sprintf(temp,"%s_%s",serviceNum,"Subscribe");
		return std::string(temp);
	}

	static  std::string heartbeat(char* serviceNum){
		char temp[64];
		sprintf(temp,"%s_%s",serviceNum,"HeartBeat");
		return std::string(temp);
	}

	static  std::string devicelist(char* serviceNum){
		char temp[64];
		sprintf(temp,"%s_%s",serviceNum,"DeviceList");
		return std::string(temp);
	}
};

#endif /* SERVICENUM2KEYMAP_H_ */
