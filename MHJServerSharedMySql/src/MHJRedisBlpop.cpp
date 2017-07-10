/*
 * MHJRedisBlpop.cpp
 *
 *  Created on: 2016年1月5日
 *      Author: ruancongming
 */

#include "MHJRedisBlpop.h"
#include "ProtocolServer.h"

//以阻塞方式读取队列，返回读取的结果
MHJRedisBlpop::MHJRedisBlpop(char*ip, int port, char* passwd, MHJRedisSubscribeFactory* subscribefactory) {
	// TODO Auto-generated constructor stub
	this->ip = ip;
	this->port = port;
	this->passwd = passwd;
	this->subscribe = subscribefactory;

}

MHJRedisBlpop::~MHJRedisBlpop() {
	// TODO Auto-generated destructor stub
}

void MHJRedisBlpop::doBlpop(void) {

	MHJRedisBaseFactory* redisfactory = MHJRedisBaseFactory::GetRedisFactory(); //获取静态类

	redisContext* context = redisConnect(ip, port);  //redis 连接

	char cmdformat[REDISCMD_SIZE];
	sprintf(cmdformat, "%s  %s", "AUTH", passwd);
	redisReply* reply = (redisReply*) redisCommand(context, cmdformat);  //redis认证

	if (reply != NULL) {
		freeReplyObject(reply);

		char* queueName = redisfactory->getLinuxBlpop();   //获取列表名 TODO 获取列表名称应该是自己的名词
		sprintf(cmdformat, "%s  %s  %d", "BLPOP", queueName, 0);

		while (1) {
			reply = (redisReply*) redisCommand(context, cmdformat);  //阻塞形式获取对列的值

			if (reply) {
//	#ifdef DEBUG
//				std::cout << "\n Redis命令为:" <<  cmdformat<< std::endl;
//				printf("$$$$receive: type= %d, len= %d, str= %s, elements=%d \n",reply->type,reply->len,reply->str, reply->elements);
//				int i;
//				for(i=0;i<reply->elements;i++)
//					printf("####elements[%d] = %s\n",i,reply->element[i]->str);
//	#endif
				if (!reply->element){
					printf("订阅失败，程序退出！！！！！");
					exit(-1);
				}
				char *ptr = reply->element[1]->str;
				int reciveSize = reply->element[1]->len / 2;
				uint8_t* recive = new uint8_t[reciveSize];

				char tmp[3];
				tmp[2] = '\0';
				uint8_t tmpU8;
				for (int i = 0; i < reciveSize; i++) {
					memcpy(tmp, &ptr[i * 2], 2);
					sscanf(tmp, "%x", &tmpU8);
					recive[i] = tmpU8;
//					printf("%02x",recive[i]);
				}
				MHJSUBSCRIBEProtocol *subscribeptotocol = (MHJSUBSCRIBEProtocol *) recive;
				shared_ptr < SP_Buffer > data = make_shared<SP_Buffer>();
				data->append(((char*) (&subscribeptotocol->data)), reciveSize - (sizeof(MHJSUBSCRIBEProtocol) - sizeof(subscribeptotocol->data))); //-1是减去占位符

				MHJRedisSubscribeProcessFactory* process = subscribe->getProcess();

				switch (subscribeptotocol->type) {
				case MSPT_CMDTRANSFER:
					process->ProcessCommand(subscribeptotocol->source, data);
					break;
				default:
					printf("BLPOP收到未能识别的命令\n");
					break;
				}

				freeReplyObject(reply);
				delete[] (uint8_t*) recive;
			}
		}
	} else {
		std::cout << "BLPOP Redis认证失败！" << std::endl;
	}
}
