/*
 * MHJIncludeInit.h
 *
 *  Created on: 2015年11月27日
 *      Author: waroy
 */

#ifndef MHJINCLUDEINIT_H_
#define MHJINCLUDEINIT_H_

#include "sphandler.hpp"
#include "MHJRedisCmd.h"

class MHJ_include_Handler: public SP_Handler {
public:
	MHJ_include_Handler() {
	}
	~MHJ_include_Handler() {
	}
	virtual int start(SP_Request * request, SP_Response * response) {
		return 0;
	}
	virtual int handle(SP_Request * request, SP_Response * response) {
		return 0;
	}

	virtual void error(SP_Response * response) {

	}

	virtual void timeout(SP_Response * response) {

	}

	virtual void close() {

	}
};
class MHJ_include_HandlerFactory: public SP_HandlerFactory {
public:
	MHJ_include_HandlerFactory() {
	}
	~MHJ_include_HandlerFactory() {
	}
	SP_Handler * create() const {
		return new MHJ_include_Handler;
	}
	SP_CompletionHandler * createCompletionHandler() {
		return this->SP_HandlerFactory::createCompletionHandler();
	}
};

void mhj_includeInit() {
	redisConnect(NULL, NULL);
	redisAsyncCommand(NULL, NULL, NULL, NULL);

	MHJ_include_HandlerFactory* include_HandlerFactory = new MHJ_include_HandlerFactory;
	MHJ_include_Handler* include_Handler = (MHJ_include_Handler*) include_HandlerFactory->create();
	if (include_Handler) {
		include_Handler->close();
	}
}
#endif /* MHJINCLUDEINIT_H_ */
