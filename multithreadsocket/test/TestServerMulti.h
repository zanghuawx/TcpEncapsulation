
#ifndef _TESTSERVERMULTI_H_
#define _TESTSERVERMULTI_H_

#include "TcpServer.h"

class TestServerMulti {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	TestAll(EventLoop * loop, const int port, const int threadNum = 1);
	~TestAll();
	void start();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);
private:
	TcpServer server_;

	
};



#endif



