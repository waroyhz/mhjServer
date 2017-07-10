/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __spserver_hpp__
#define __spserver_hpp__

#include <sys/types.h>
#include "spthread.hpp"

class SP_HandlerFactory;
class SP_Session;
class SP_Executor;
class SP_IOChannelFactory;
class SP_Response;

struct event;

// half-sync/half-async thread pool server
class SP_Server {
public:
	SP_Server(const char * bindIP, int port, SP_HandlerFactory * handlerFactory);
	~SP_Server();

	void setTimeout(int timeout);
	void setMaxConnections(int maxConnections);
	void setMaxThreads(int maxThreads);
	void setReqQueueSize(int reqQueueSize, const char * refusedMsg);
	void setIOChannelFactory(SP_IOChannelFactory * ioChannelFactory);

	void pushResponseQueue(SP_Response* response);

	void shutdown();
	int isRunning();
	int run();
	void runForever();

private:
	SP_HandlerFactory * mHandlerFactory;
	SP_IOChannelFactory * mIOChannelFactory;

	//消息列队
	void * peventArg;

	char mBindIP[64];
	int mPort;
	int mIsShutdown;
	int mIsRunning;

	int mTimeout;
	int mMaxThreads;
	int mMaxConnections;
	int mReqQueueSize;
	char * mRefusedMsg;

	static sp_thread_result_t SP_THREAD_CALL eventLoop( void * arg );

	int start();

	static void sigHandler(int, short, void * arg);

	static void outputCompleted(void * arg);
};

#endif

