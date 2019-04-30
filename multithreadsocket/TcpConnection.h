//TcpConnection类，客户端连接的抽象表示

#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include <functional>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <memory>
#include "Channel.h"
#include "EventLoop.h"

//std::enable_shared_from_this 能让一个对象（假设其名为 t ，且已被一个 std::shared_ptr 对象 pt 管理）
//安全地生成其他额外的 std::shared_ptr 实例（假设名为 pt1, pt2, ... ） ，它们与 pt 共享对象 t 的所有权。
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	typedef std::shared_ptr<TcpConnection> spTcpConnection;		//TcpConnection只能指针
	//回调函数类型
	typedef std::function<void (const spTcpConnection&)> Callback;
	typedef std::function<void (const spTcpConnection&, std::string&)> MessageCallback;

	typedef struct sockaddr_in SockAddr;

	TcpConnection(EventLoop* loop, int fd, const struct sockaddr_in& clientaddr);
	~TcpConnection();

	//获取当前连接的fd
	int fd() const {
		return fd_;
	}
	SockAddr getSockAddr() const {
		return clientAddr_;
	}
	std::string getIpAddr() const {
		return inet_ntoa(clientAddr_.sin_addr);
	}
	int getPort() const {
		return ntohs(clientAddr_.sin_port);
	}
	//获取当前连接所属的loop
	EventLoop* getLoop() const {
		return loop_;
	}
	//添加本连接对应的事件到loop
	void addChannelToLoop();
	//发送数据的函数
	void sendString(const std::string& str);
	//在当前IO线程发送数据函数
	void sendInLoop();
	//主动清理连接
	void shutDown();
	//在当前io线程清理连接函数
	void shutDownInLoop();

	//可读事件回调
	void handleRead();
	//可写事件回调
	void handleWrite();
	//错误事件回调
	void handleError();
	//连接关闭事件回调
	void handleClose();

	//设置收到数据回调函数
	void setMessageCallback(const MessageCallback& cb) {
		messageCallback_ = cb;
	}
	//设置发送完数据的回调函数
	void setSendCompleteCallback(const Callback& cb) {
		sendCompleteCallback_ = cb;
	}
	//设置连接关闭的回调函数
	void setCloseCallback(const Callback& cb) {
		closeCallback_ = cb;
	}
	//设置连接异常的回调函数
	void setErrorCallback(const Callback& cb) {
		errorCallback_ = cb;
	}
	//设置连接清理函数
	void setConnectionCleanUp(const Callback& cb) {
		connectCleanup_ = cb;
	}
	//设置异步处理标志,开启工作线程池的时候使用
	void setAsyncProcess(const bool asyncProcess) {
		asyncProcess_ = asyncProcess;
	}
	
private:
	static int recvn(int fd, std::string& bufferin);
	static int sendn(int fd, std::string& bufferout);
	
	int fd_;
	EventLoop* loop_;
	//各种回调函数
	MessageCallback messageCallback_;
	Callback sendCompleteCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	Callback connectCleanup_;
	//异步调用标志位,当工作任务交给线程池时，置为true，任务完成回调时置为false
	bool asyncProcess_;

	std::unique_ptr<Channel> spChannel_;	//事件的智能指针
	struct sockaddr_in clientAddr_;		//对端地址
	bool halfClose_;	//半关闭标志位.半关闭表示客户端断开了，而服务端还未主动断开此连接
	bool disConnected_;	//连接已关闭标志位

	//读写缓冲
	std::string bufferIn_;
	std::string bufferOut_;

	
};


#endif

