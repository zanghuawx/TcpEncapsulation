
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "Socket.h"
#include "Address.h"

#include <functional>
#include <string>
#include <map>
#include <mutex>
#include <pthread.h>


#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"


class TcpClient : public std::enable_shared_from_this<TcpClient> {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> Callback;
	TcpClient(EventLoop* loop, const std::string& ip, const int& port, const std::string& name = "client");
	~TcpClient();

	void start();
	void close();

	void startMainLoop();
	bool isConnect() const {
		return isConnected;
	}

	void setMessageCallback(const MessageCallback& cb) {
		messageCallback_ = cb;
	}
	void setConnectionCallback(const Callback& cb) {
		connectedCallback_ = cb;
	}
	void setSendCompleteCallback(const Callback& cb) {
		sendCompleteCallback_ = cb;
	}
	void setCloseCallback(const Callback& cb) {
		closeCallback_ = cb;
	}
	void setErrorCallback(const Callback& cb) {
		errorCallback_ = cb;
	}

	TcpConnectionPtr getTcpConnectPtr() const {
		return conn_;
	}
	

private:
	//业务接口回调函数
	MessageCallback messageCallback_;
	Callback connectedCallback_;
	Callback sendCompleteCallback_;
	Callback closeCallback_;
	Callback errorCallback_;

	//服务器socket
	Socket clientSocket_;
	EventLoop* loop_;		//主loop
	Address addr_;
	std::string name_;
	bool isConnected;
	

	std::mutex mutex_;
	EventLoopThreadPool eventLoopThreadPool_;
	std::thread th_;

	TcpConnectionPtr conn_;

	void startOnLoop();
	void newConnection();

	void closeConnection(const TcpConnectionPtr& conn);

	void onConnection(const TcpConnectionPtr& conn);
	void onSendComplete(const TcpConnectionPtr& conn);
	void onClose(const TcpConnectionPtr& conn);
	void onError(const TcpConnectionPtr& conn);
	
	
	
};


#endif



