
#include "TcpClient.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <time.h>


//由于客户端,main.cpp不能启动loop.loop()循环,还得干别的事，所以只能在io线程池里面启动main IO loop.
//这就要求线程池至少有一个线程,即构造函数threadnum不能等于0

//工作线程
TcpClient::TcpClient(EventLoop * loop, const std :: string & ip, const int & port, const std :: string & name)
		: clientSocket_(), loop_(loop), addr_(ip, port), name_(name), isConnected(false), mutex_(),
		eventLoopThreadPool_(loop_, 1), th_() {

	eventLoopThreadPool_.start();
	usleep(500000);			//等待新开的线程(线程池中的线程)完成启动.

	//业务函数先注册,避免空指针
	this->setConnectionCallback(std::bind(&TcpClient::onConnection, this, std::placeholders::_1));
	this->setSendCompleteCallback(std::bind(&TcpClient::onSendComplete, this, std::placeholders::_1));
	this->setErrorCallback(std::bind(&TcpClient::onError, this, std::placeholders::_1));
	this->setCloseCallback(std::bind(&TcpClient::onClose, this, std::placeholders::_1));
	//std::cout << "loop *p: " << loop_ << std::endl;

}

TcpClient::~TcpClient() {
	loop_->quitLoop();
	th_.join();
}

//工作线程
void TcpClient::start() {
	//一定要先将任务投递到loop_中, 才能启动 loop_->loop(),不然会崩溃,我也不知道为什么

	std::cout << "point loop_: " << loop_ << std::endl;
	loop_->addTask(std::bind(&TcpClient::startOnLoop, shared_from_this()));		//connect 要在IO loop中执行,所以是跨线程,使用 shared_from_this()

	th_ = std::thread(&TcpClient::startMainLoop, this);
	usleep(500000);			//等待新开的线程完成启动.
	
}

//工作线程
void TcpClient::close() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		clientSocket_.closeSocket();
		isConnected = false;
	}
}

void TcpClient::startMainLoop() {
	std::cout << "startMainLoop point loop_:" << loop_ << std::endl;
	loop_->loop();
}


//IO线程
void TcpClient::startOnLoop() {
	std::cout << "now connect..." << std::endl;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		int pt = addr_.port();
		if (!isConnected) {
			if (clientSocket_.connectSocket(addr_.ip(), pt)  == 0) {
				newConnection();
				isConnected = true;
			}
		}
	}
}

void TcpClient::newConnection() {
	//**************
	std::cout << "connected to server " << std::endl;
	
	EventLoop* loop = eventLoopThreadPool_.getNextLoop();
	conn_ = std::make_shared<TcpConnection>(loop, clientSocket_.fd(), (struct sockaddr_in)(addr_));
	//std::cout << "con_ count: " << conn_.use_count() << std::endl;
	//clientSocket_.setReuseAddr();
	clientSocket_.setNonblocking();


	conn_->setCloseCallback(closeCallback_);
	conn_->setErrorCallback(errorCallback_);
	conn_->setMessageCallback(messageCallback_);
	conn_->setSendCompleteCallback(sendCompleteCallback_);
	conn_->setConnectionCleanUp(std::bind(&TcpClient::closeConnection, this, std::placeholders::_1));
	//std::cout << "tcpclient.cpp line 42: setConnectionCleanUp" << std::endl;
	conn_->addChannelToLoop();
	//***************
}


////IO线程
void TcpClient::closeConnection(const TcpConnectionPtr& conn) {
	
	{
		std::lock_guard<std::mutex> lock(mutex_);
		clientSocket_.closeSocket();
		isConnected = false;
	}
	
}

////IO线程
void TcpClient::onConnection(const TcpConnectionPtr& conn) {
	std::cout << "connected socketfd->" << conn->fd() << std::endl;
	
}
void TcpClient::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete socketfd->" << conn->fd()
		<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}
void TcpClient::onClose(const TcpConnectionPtr& conn) {
	std::cout << "close socketfd->" << conn->fd()
		<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}
void TcpClient::onError(const TcpConnectionPtr& conn) {
	std::cout << "error socketfd->" << conn->fd()
			<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}










