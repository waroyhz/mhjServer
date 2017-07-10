/*
 * CClientASIO.cpp
 *
 *  Created on: 2015-7-9
 *      Author: waroy
 */

#include "CClientASIO.h"

CClientASIO::CClientASIO(const std::string & hostname, unsigned short port,
		io_service * io_service_, REVICEFUNC reviceFunc, void* arg) {
	this->mReviceFunc = reviceFunc;
	this->arg = arg;

	mutex = new pthread_mutex_t;
	sp_thread_mutex_init(mutex, NULL);

	m_io_service = io_service_;
	m_socket = new tcp::socket(*m_io_service);

	tcp::resolver resolver(*m_io_service);
	boost::system::error_code ec;
	tcp::resolver::query query(hostname,
			boost::lexical_cast<std::string, unsigned short>(port));
	tcp::resolver::iterator iter = resolver.resolve(query, ec);
	tcp::resolver::iterator end;

	// pick the first endpoint
	if (iter != end && ec == 0) {
		tcp::endpoint endpoint = *iter;

		std::cout << "Connecting to: " << endpoint << std::endl;

		boost::system::error_code error = m_socket->connect(endpoint, ec);
		//		if (error.value()) {
		//
		//		} else {
		memset(m_data, 0, sizeof(m_data));
		m_socket->async_read_some(
				boost::asio::buffer(m_data, _RECV_BUF_SIZE_),
				boost::bind(&CClientASIO::OnRecv, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));

		//		}
		if (ec) {
			std::cerr << "Error: " << ec << std::endl;
			//            throw ec;
			setConnect(false);
		} else
			setConnect(true);
	}
}

CClientASIO::~CClientASIO() {
	if (m_socket)
		delete m_socket;
	m_socket = NULL;
	sp_thread_mutex_destroy(mutex);
	delete mutex;
	//    delete m_io_service;
}

void CClientASIO::PostSend(const std::string & message) {
	m_socket->async_write_some(
			boost::asio::buffer(message.c_str(), message.size()),
			boost::bind(&CClientASIO::OnSend, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void CClientASIO::SendData(const void* data, int size) {
	m_socket->async_write_some(
			boost::asio::buffer(data, size),
			boost::bind(&CClientASIO::OnSend, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

bool CClientASIO::OnRecv(const boost::system::error_code& error,
		size_t bytes_transferred) {
	if (!error) {
		//		std::string strRecv = (char *) m_data;
		//
		//		std::cout << strRecv << std::endl;
		mBuffer.append(m_data, bytes_transferred);
		mReviceFunc(&mBuffer, arg);
		memset(m_data, 0, sizeof(m_data));
		m_socket->async_read_some(
				boost::asio::buffer(m_data, _RECV_BUF_SIZE_),
				boost::bind(&CClientASIO::OnRecv, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	} else {
		OnClose(error);
		return false;
	}
	return true;
}

bool CClientASIO::OnSend(const boost::system::error_code& error,
		size_t bytes_transferred) {
	//	std::cout << "Send Bytes:" << bytes_transferred << std::endl;
	if (error) {
		OnClose(error);
		return false;
	}
	return true;
}

void CClientASIO::OnClose(const boost::system::error_code& error) {
	//	misconnected=false;
	setConnect(false);
	std::cout << "tcp Close ， error code:" << error.value()
			<< ", error message:" << error.message() << std::endl;
	//	if (m_socket)
	//	{
	//	m_socket->close();
	//	delete m_socket;
	//	m_socket = NULL;
	//	}
}

void CClientASIO::Close() {
	 m_socket->close();
}

bool CClientASIO::isConnected() {
	bool ret = false;
	sp_thread_mutex_lock(mutex);
	ret = misconnected;
	sp_thread_mutex_unlock(mutex);
	return ret;
}

void CClientASIO::setConnect(bool value) {
	sp_thread_mutex_lock(mutex);
	misconnected = value;
	sp_thread_mutex_unlock(mutex);
}
