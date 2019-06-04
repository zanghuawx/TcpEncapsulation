#include "TestFile.h"

#include <time.h>
#include <unistd.h>


TestFile::TestFile(EventLoop * loop, const std::string& ip, const int port, const std::string& name) 
	: client_(std::make_shared<TcpClient>(loop, ip, port, name)), file_(name), isSendFileComplete(false) {
	client_->setConnectionCallback(std::bind(&TestFile::onConnection, this, std::placeholders::_1));
	client_->setMessageCallback(std::bind(&TestFile::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	client_->setSendCompleteCallback(std::bind(&TestFile::onSendComplete, this, std::placeholders::_1));
	file_.setSended(std::bind(&TestFile::onFileSended, this, std::placeholders::_1));
}

TestFile::~TestFile() {

}

void TestFile::start() {
	client_->start();
}

void TestFile::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "connectioned socketfd->" << conn->fd() << std::endl;
	
}

void TestFile::onMessage(const TcpConnectionPtr & conn, std::string & message) {

	
	
}

void TestFile::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	conn->sendString(message);
}

void TestFile::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete!" << std::endl;
	file_.onWriteComplete(conn);
}

void TestFile::startSendFile(const TcpConnectionPtr& conn, const std::string& filename, const std::string& option) {
	isSendFileComplete = false;
	file_.selectFile(filename, option);
	file_.sendByTcp(conn);
}

void TestFile::onFileSended(const TcpConnectionPtr& conn) {
	std::cout << "send file success!" << std::endl;
	isSendFileComplete = true;
}








int main(int argc, char** argv) {
	EventLoop* loop = new EventLoop();
	std::string ip("127.0.0.1");
	int port = 8032;
	std::string name("./test/testfile.txt");
	TestFile client(loop, ip, port, name);
	client.start();
	

	if (client.client_->isConnect()) {
		client.startSendFile(client.client_->getTcpConnectPtr(), name);
	}
	
	while (!client.isSendFileComplete) {
		usleep(100000);
	}
	
}


