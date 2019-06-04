#include "TestContactTransferServer.h"

#include <time.h>
#include <unistd.h>

TestContactTransferServer::TestContactTransferServer(EventLoop * loop, const int port, const std::string& filename, const int threadNum)
								: server_(loop, port, threadNum), isOver_(false), contactTrans_(filename) {
	server_.setConnectionCallback(std::bind(&TestContactTransferServer::onConnection, this, std::placeholders::_1));
	server_.setMessageCallback(std::bind(&TestContactTransferServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	server_.setSendCompleteCallback(std::bind(&TestContactTransferServer::onSendComplete, this, std::placeholders::_1));
	

	contactTrans_.setSended(std::bind(&TestContactTransferServer::onFileSended, this, std::placeholders::_1));
	contactTrans_.setReceived(std::bind(&TestContactTransferServer::onFileReceived, this, std::placeholders::_1));
	
}


TestContactTransferServer::~TestContactTransferServer() {

}



void TestContactTransferServer::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "connectioned socketfd->" << conn->fd() << std::endl;
	
}

void TestContactTransferServer::onMessage(const TcpConnectionPtr & conn, std::string & message) {
	std::cout << message << std::endl;
	contactTrans_.receiveFromTcp(conn, message);
	
}

void TestContactTransferServer::onSendComplete(const TcpConnectionPtr & conn) {
	contactTrans_.onWriteComplete(conn);
}

void TestContactTransferServer::onFileSended(const TcpConnectionPtr & conn) {
	std::cout << "send file success!" << std::endl;
	
	isOver_ = true;
	//std::cout << contactTrans_ << std::endl;
}

void TestContactTransferServer::onFileReceived(const TcpConnectionPtr & conn) {
	std::cout << "received file success!" << std::endl;
	
	std::cout << contactTrans_ << std::endl;
	//std::cout << contactTrans_ << std::endl;
}


void TestContactTransferServer::sendContacts(const TcpConnectionPtr & conn, const std :: string & option) {
	isOver_ = false;
	contactTrans_.sendContactsByTcp(conn);
}


int main(int argc, char** argv) {
	EventLoop* loop = new EventLoop();
	int port = 8032;
	std::string filename("./testContactTransferServer.log");
	TestContactTransferServer server(loop, port, filename);
	
	server.start();


	loop->loop();
	
}

