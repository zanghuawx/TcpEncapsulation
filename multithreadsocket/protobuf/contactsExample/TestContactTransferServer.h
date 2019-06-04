
#ifndef _TESTCONTACTTRANSFERSERVER_H_
#define _TESTCONTACTTRANSFERSERVER_H_

#include "ContactTransfer.h"
#include "TcpServer.h"

class TestContactTransferServer {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&, std::string&)> MessageCallback;
	typedef std::function<void (const TcpConnectionPtr&)> Callback;
	TestContactTransferServer(EventLoop * loop, const int port, const std::string& filename, const int threadNum = 1);
	~TestContactTransferServer();
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, std::string& message);
	void onSendComplete(const TcpConnectionPtr& conn);
	void onFileSended(const TcpConnectionPtr& conn);
	void onFileReceived(const TcpConnectionPtr& conn);

	void start() {
		server_.start();
	}
	void sendContacts(const TcpConnectionPtr& conn, const std::string& option = FileOperator::WriteCreateMode);
	ContactTransfer& getContactTrans() {
		return contactTrans_;
	}
	
	TcpServer server_;
	bool isOver_;

private:
	ContactTransfer contactTrans_;
	
};

#endif

