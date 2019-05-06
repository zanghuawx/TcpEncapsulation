
#include "TestClientMulti.h"
#include <time.h>
#include <unistd.h>
#include <sstream>



TestClientMulti::TestClientMulti(EventLoop * loop, const std::string& ip, const int port, const std::string& name) 
	: client_(std::make_shared<TcpClient>(loop, ip, port, name)) {
	client_->setConnectionCallback(std::bind(&TestClientMulti::onConnection, this, std::placeholders::_1));
	client_->setMessageCallback(std::bind(&TestClientMulti::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	client_->setSendCompleteCallback(std::bind(&TestClientMulti::onSendComplete, this, std::placeholders::_1));
}

TestClientMulti::~TestClientMulti() {

}

void TestClientMulti::start() {
	client_->start();
}

void TestClientMulti::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "connectioned socketfd->" << conn->fd() << std::endl;
	
}

void TestClientMulti::onMessage(const TcpConnectionPtr & conn, std::string & message) {
	std::string str;
	
	//sendMessage(conn, message);
	str.swap(message);
	message.erase();
	str.insert(0, "received msg: ");
	
	std::cout << "conn fd:" << conn->fd() << " " << str << std::endl;
	
}

void TestClientMulti::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	//遍历连接红黑树,把消息发送到每一个tcp中
	conn->sendString(message);
}

void TestClientMulti::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete!" << std::endl;
}






int main(int argc, char** argv) {

	std::string ip("127.0.0.1");
	int port = 8032;
	std::string arg("\nwo shi shui --> ");
	
	ThreadPool pool(2);
	pool.start();
	
	typedef std::shared_ptr<TestClientMulti> ClientPtr;
	typedef std::set<ClientPtr> ClientPtrSet;
	ClientPtrSet clientPtrSet;
	
	for (int i = 0 ; i < 10;  i++) {
		EventLoop* loop = new EventLoop();
		clientPtrSet.insert(std::make_shared<TestClientMulti>(loop, ip, port, "hahaha"));
		
	}

	for (ClientPtrSet::iterator it = clientPtrSet.begin(); it != clientPtrSet.end(); ++it) {
		ThreadPool::Task func = std::bind(&TestClientMulti::start, (*it).get());
		pool.addTask(func);
		//(*it)->start();
	}
	
	while (true) {

		for (ClientPtrSet::iterator it = clientPtrSet.begin(); it != clientPtrSet.end(); ++it) {
			if ((*it)->client_->isConnect()) {
				std::stringstream ss;
				ss << arg << (*it)->client_->getTcpConnectPtr()->fd();
				ThreadPool::Task func = std::bind(&TestClientMulti::sendMessage, (*it).get(), (*it)->client_->getTcpConnectPtr(), ss.str() );
				pool.addTask(func);
			}
			usleep(50000);
		}
		
		usleep(2000000);
	}
}







