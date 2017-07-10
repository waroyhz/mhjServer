/*
 * CClientASIO.h
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#ifndef CCLIENTASIO_H_
#define CCLIENTASIO_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <pthread.h>
#include "spbuffer.hpp"
#include "spthread.hpp"

using boost::asio::io_service;
using boost::asio::ip::tcp;

#define _RECV_BUF_SIZE_ 1024

typedef void(* REVICEFUNC)(SP_Buffer* inBuffer, void *arg);
class CClientASIO {
public:

	CClientASIO(const std::string & hostname, unsigned short port,
			io_service * io_service_, REVICEFUNC reviceFunc, void* arg);
	virtual ~CClientASIO();

	// methods
	virtual void PostSend(const std::string & message);
	virtual void SendData(const void* data, int size);

	//event call
	virtual bool OnRecv(const boost::system::error_code& error,
			size_t bytes_transferred);
	virtual bool OnSend(const boost::system::error_code& error,
			size_t bytes_transferred);
	virtual void OnClose(const boost::system::error_code& error);

	virtual void Close();

	bool isConnected();

private:
	void setConnect(bool);
	io_service * m_io_service;
	tcp::socket * m_socket;
	char m_data[_RECV_BUF_SIZE_];
	bool misconnected;
	sp_thread_mutex_t *mutex;
	REVICEFUNC mReviceFunc;
	void *arg;
	SP_Buffer mBuffer;
};

#endif /* CCLIENTASIO_H_ */
