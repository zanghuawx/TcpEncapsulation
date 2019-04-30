
#ifndef _TESTCLIENT_H_
#define _TESTCLIENT_H_

#include "TcpClient.h"

class TestClient {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	TestClient(EventLoop * loop, const std::string& ip, const int port, const std::string& name = "client");
	~TestClient();
	void start();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);

	std::shared_ptr<TcpClient> client_;
private:
	

	
};



#endif


