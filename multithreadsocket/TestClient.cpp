#include "TestClient.h"
#include <time.h>
#include <unistd.h>


TestClient::TestClient(EventLoop * loop, const std::string& ip, const int port, const std::string& name) 
	: client_(std::make_shared<TcpClient>(loop, ip, port, name)) {
	client_->setConnectionCallback(std::bind(&TestClient::onConnection, this, std::placeholders::_1));
	client_->setMessageCallback(std::bind(&TestClient::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	client_->setSendCompleteCallback(std::bind(&TestClient::onSendComplete, this, std::placeholders::_1));
}

TestClient::~TestClient() {

}

void TestClient::start() {
	client_->start();
}

void TestClient::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "connectioned socketfd->" << conn->fd() << std::endl;
	
}

void TestClient::onMessage(const TcpConnectionPtr & conn, std::string & message) {
	std::string str;
	
	//sendMessage(conn, message);
	str.swap(message);
	message.erase();
	str.insert(0, "received msg: ");
	
	std::cout << " " << str << std::endl;
	
}

void TestClient::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	//遍历连接红黑树,把消息发送到每一个tcp中
	conn->sendString(message);
}

void TestClient::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "send complete!" << std::endl;
}






int main(int argc, char** argv) {
	EventLoop* loop = new EventLoop();
	std::string ip("127.0.0.1");
	int port = 8032;
	TestClient client(loop, ip, port, "hesheng");
	client.start();
	std::string arg("wo shi shui");
	while (true) {
		if (client.client_->isConnect()) {
			client.sendMessage(client.client_->getTcpConnectPtr(), arg);
		}
		
		usleep(50000);
	}
}




