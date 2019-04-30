
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <functional>
#include <string>
#include <map>
#include <mutex>
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"

#define MAXCONNECTION 20000

class TcpServer {
public:
	//定义TcpConnection智能指针类型
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	//定义回调函数类型一个onMessage一个onConnection
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> Callback;
	//构造函数,主loop,端口,线程池数量
	TcpServer(EventLoop* loop, const int port, const int threadNum);
	~TcpServer();

	//启动tcp服务器
	void start();

	//业务函数注册
	//注册接受到数据回调
	void setMessageCallback(const MessageCallback& cb) {
		messageCallback_ = cb;
	}
	//注册新连接回调函数
	void setConnectionCallback(const Callback& cb) {
		newConnectCallback_ = cb;
	}
	//注册数据发送完成回调函数
	void setSendCompleteCallback(const Callback& cb) {
		sendCompleteCallback_ = cb;
	}
	//注册连接关闭回调函数
	void setCloseCallback(const Callback& cb) {
		closeCallback_ = cb;
	}
	//注册连接异常回调函数
	void setErrorCallback(const Callback& cb) {
		errorCallback_ = cb;
	}

	std::map<int, TcpConnectionPtr> tcpConnList_;	//tcp 连接表

private:
	//业务接口回调函数
	MessageCallback messageCallback_;
	Callback newConnectCallback_;
	Callback sendCompleteCallback_;
	Callback closeCallback_;
	Callback errorCallback_;

	//服务器socket
	Socket serverSocket_;
	EventLoop* loop_;		//主loop
	Channel serverChannel_;	//服务器事件
	int connCount_;			//连接数量统计
	
	std::mutex mutex_;		//保护tcp连接表的互斥量
	EventLoopThreadPool eventLoopThreadPool_;	//IO线程池(假如用上了)
	
	//服务器对新连接连接处理的函数,主要是将客户socket事件加入epoll中,分发IO loop. 跟用户的新连接回调函数是两个概念.
	void onNewConnection();
	//移除tcp连接的函数
	void removeConnection(const TcpConnectionPtr sp);
	//服务器socket的异常处理函数
	void onConnectionError();

	//预先定义业务函数,免得用户忘记注册业务函数接口,事件回调时造成空指针
	//void onMessage(const TcpConnectionPtr& conn, const std::string& message);	//这个用户不会忘记注册,要是不注册则是没读明白源码
	void onConnection(const TcpConnectionPtr& conn);
	void onSendComplete(const TcpConnectionPtr& conn);
	void onClose(const TcpConnectionPtr& conn);
	void onError(const TcpConnectionPtr& conn);
};





#endif

