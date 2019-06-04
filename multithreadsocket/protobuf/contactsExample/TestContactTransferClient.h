
#ifndef _TESTCONTACTTRANSFERCLIENT_H_
#define _TESTCONTACTTRANSFERCLIENT_H_

#include "ContactTransfer.h"
#include "TcpClient.h"

class TestContactTransferClient {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> Callback;
	TestContactTransferClient(EventLoop* loop, const std::string& ip, const int& port, const std::string& filename);
	~TestContactTransferClient();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);
	void onFileSended(const TcpConnectionPtr& conn);

	void start() {
		client_->start();
	}
	void sendContacts(const TcpConnectionPtr& conn, const std::string& option = FileOperator::WriteCreateMode);
	void sendMessage(const TcpConnectionPtr& conn, const std::string& message);
	ContactTransfer& getContactTrans() {
		return contactTrans_;
	}
	
	std::shared_ptr<TcpClient> client_;
	bool isOver_;

private:
	ContactTransfer contactTrans_;
	
};

#endif


