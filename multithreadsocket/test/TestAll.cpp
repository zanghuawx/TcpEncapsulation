#include "TestAll.h"


TestAll::TestAll(EventLoop * loop, const int port, const int threadNum) 
	: server_(loop, port, threadNum) {
	server_.setConnectionCallback(std::bind(&TestAll::onConnection, this, std::placeholders::_1));
	server_.setMessageCallback(std::bind(&TestAll::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	server_.setSendCompleteCallback(std::bind(&TestAll::onSendComplete, this, std::placeholders::_1));
}

TestAll::~TestAll() {

}

void TestAll::start() {
	server_.start();
}

void TestAll::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "new connection socketfd->" << conn->fd() << std::endl;
	
}

void TestAll::onMessage(const TcpConnectionPtr & conn, std::string & message) {
	std::string str;
	
	sendMessage(conn, message);
	str.swap(message);
	message.erase();
	str.insert(0, "received msg: ");
	std::cout << "fd: " << conn->fd() << " ....... " << str << std::endl;
	
}

void TestAll::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	//遍历连接红黑树,把消息发送到每一个tcp中
	for (std::map<int, TcpConnectionPtr>::iterator it = server_.tcpConnList_.begin(); it != server_.tcpConnList_.end(); ++it) {
		std::cout << "socketfd: " << it->first << std::endl;	//it->first 是红黑树的关键字
		it->second->sendString(message);	//it->second 是红黑树的值
	}

	//conn->sendString(message);
}

void TestAll::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete!" << std::endl;
}






int main(int argc, char** argv) {
	EventLoop* loop = new EventLoop();
	int port = 8032;
	TestAll server(loop, port);
	server.start();
	loop->loop();
}


