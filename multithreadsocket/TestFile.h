
#ifndef _TESTCLIENT_H_
#define _TESTCLIENT_H_

#include "FileOperator.h"
#include "TcpClient.h"

class TestFile {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
	TestFile(EventLoop * loop, const std::string& ip, const int port, const std::string& name = "client");
	~TestFile();
	void start();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);
	
	void startSendFile(const TcpConnectionPtr& conn, const std::string& filename, const std::string& option = FileOperator::WriteCreateMode);
	void onFileSended(const TcpConnectionPtr& conn);
	

	std::shared_ptr<TcpClient> client_;
	bool isSendFileComplete;
private:
	
	ConnectionCallback fileSendOneFrameComplete_;
	FileOperator file_;
	
	
};



#endif


