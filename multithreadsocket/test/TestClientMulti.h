
#ifndef _TESTCLIENTMULTI_H_
#define _TESTCLIENTMULTI_H_

#include "TcpClient.h"
#include "ThreadPool.h"
#include <unistd.h>


class TestClientMulti {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	TestClientMulti(EventLoop * loop, const std::string& ip, const int port, const std::string& name = "client");
	~TestClientMulti();
	void start();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);

	std::shared_ptr<TcpClient> client_;
private:
	

	
};



#endif





