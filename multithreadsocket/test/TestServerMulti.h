
#ifndef _TESTSERVERMULTI_H_
#define _TESTSERVERMULTI_H_

#include "TcpServer.h"
#include "ThreadPool.h"
#include <unistd.h>


class TestServerMulti {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	TestServerMulti(EventLoop * loop, const int port, const int threadNum = 5, const int threadPoolNum = 5);
	~TestServerMulti();
	void start();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	void sendInPool(const TcpConnectionPtr& conn, const std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);
private:
	TcpServer server_;
	ThreadPool pool_;

	
};



#endif



