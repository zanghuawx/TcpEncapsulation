
#include "TcpServer.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>


//设置非阻塞
void setNonBlock(int fd) {
	int opts = fcntl(fd, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(fd,GETFL)");
        exit(1);
    }
    if (fcntl(fd, F_SETFL, opts | O_NONBLOCK) < 0)
    {
        perror("fcntl(fd,SETFL,opts)");
        exit(1);
    }
}

TcpServer::TcpServer(EventLoop * loop, const int port, const int threadNum) 
	: serverSocket_(), loop_(loop), serverChannel_(), connCount_(0), eventLoopThreadPool_(loop, threadNum) {
	serverSocket_.setReuseAddr();		//设置socket选项
	serverSocket_.bindAddress(port);	//绑定端口
	serverSocket_.listenSocket();		//监听客户端连接
	serverSocket_.setNonblocking();		//设置非阻塞

	serverChannel_.setFd(serverSocket_.fd());		//将socketfd填入事件fd,转化为一个事件
	serverChannel_.setReadHandle(std::bind(&TcpServer::onNewConnection, this));
	serverChannel_.setErrorHandle(std::bind(&TcpServer::onConnectionError, this));

	//业务函数先注册,避免空指针
	this->setConnectionCallback(std::bind(&TcpServer::onConnection, this, std::placeholders::_1));
	this->setSendCompleteCallback(std::bind(&TcpServer::onSendComplete, this, std::placeholders::_1));
	this->setErrorCallback(std::bind(&TcpServer::onError, this, std::placeholders::_1));
	this->setCloseCallback(std::bind(&TcpServer::onClose, this, std::placeholders::_1));
}

TcpServer::~TcpServer() {

}

void TcpServer::start() {
	eventLoopThreadPool_.start();		//线程池启动,new出n个IO线程
	serverChannel_.setEvents(EPOLLIN | EPOLLET);	//对端有数据写入时才会触发
	loop_->addChannelToPoller(&serverChannel_);		//将事件添加到epoll中
}

//
void TcpServer::onNewConnection() {
	//循环调用accept, 获取所有建立好连接的客户端fd
	struct sockaddr_in clientaddr;
	int clientfd;
	while ((clientfd = serverSocket_.acceptSocket(clientaddr)) > 0) {
		std::cout << "New client from IP:" << inet_ntoa(clientaddr.sin_addr) 
            << ":" << ntohs(clientaddr.sin_port) << std::endl;
		//如果最大连接数已达到上限,则不接受新的连接了
		if (++connCount_ >= MAXCONNECTION) {
			std::cout << "connected client over maximum" << std::endl;
			close(clientfd);
			continue;
		}
		//选择IO线程
		loop_ = eventLoopThreadPool_.getNextLoop();	//轮流分发loop
		setNonBlock(clientfd);		//客户端socket设置为非阻塞
		//创建连接,注册业务函数
		//TcpConnectionPtr spTcpConn = std::make_shared<TcpConnection>(new TcpConnection(loop_, clientfd, clientaddr));	//错误，不能用栈的指针
		TcpConnectionPtr spTcpConn = std::make_shared<TcpConnection>(loop_, clientfd, clientaddr);
		spTcpConn->setCloseCallback(closeCallback_);
		spTcpConn->setErrorCallback(errorCallback_);
		spTcpConn->setMessageCallback(messageCallback_);
		spTcpConn->setSendCompleteCallback(sendCompleteCallback_);
		spTcpConn->setConnectionCleanUp(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
		//将连接添加到连接红黑树
		if (true) {
			std::lock_guard<std::mutex> lock(mutex_);
			tcpConnList_[clientfd] = spTcpConn;
		}
		//调用新连接回调
		newConnectCallback_(spTcpConn);
		//添加事件到epoll
		spTcpConn->addChannelToLoop();		//这句是重点
	}
}

//连接清理,这里应该由主loop来执行,投递回主线程或多线程加锁删除
void TcpServer::removeConnection(const TcpConnectionPtr sp) {
	std::lock_guard<std::mutex> lock(mutex_);
	--connCount_;
	tcpConnList_.erase(sp->fd());
}

void TcpServer::onConnectionError() {
	std::cout << "UNKNOWN EVENT" << std::endl;
	serverSocket_.closeSocket();
}




//tcp server 还有一项工作,就是将 客户端Ip地址和socketfd制作成一个map.方便随时访问.
void TcpServer::onConnection(const TcpConnectionPtr& conn) {
	std::cout << "new connection socketfd->" << conn->fd() << std::endl;
}
void TcpServer::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete socketfd->" << conn->fd()
		<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}
void TcpServer::onClose(const TcpConnectionPtr& conn) {
	std::cout << "close socketfd->" << conn->fd()
		<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}
void TcpServer::onError(const TcpConnectionPtr& conn) {
	std::cout << "error socketfd->" << conn->fd()
			<< "ip->" << conn->getIpAddr() << ":" << conn->getPort() << std::endl;
}














