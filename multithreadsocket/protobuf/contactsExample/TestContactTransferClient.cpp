#include "TestContactTransferClient.h"

#include <time.h>
#include <unistd.h>

TestContactTransferClient::TestContactTransferClient(EventLoop* loop, const std::string& ip, const int& port, const std::string& filename)
								: client_(new TcpClient(loop, ip, port)), isOver_(false), contactTrans_(filename) {
	client_->setConnectionCallback(std::bind(&TestContactTransferClient::onConnection, this, std::placeholders::_1));
	client_->setMessageCallback(std::bind(&TestContactTransferClient::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	client_->setSendCompleteCallback(std::bind(&TestContactTransferClient::onSendComplete, this, std::placeholders::_1));
	

	contactTrans_.setSended(std::bind(&TestContactTransferClient::onFileSended, this, std::placeholders::_1));
}


TestContactTransferClient::~TestContactTransferClient() {

}



void TestContactTransferClient::onConnection(const TcpConnectionPtr & conn) {
	std::cout << "connectioned socketfd->" << conn->fd() << std::endl;
	
}

void TestContactTransferClient::onMessage(const TcpConnectionPtr & conn, std::string & message) {
	contactTrans_.receiveFromTcp(conn, message);
	
}

void TestContactTransferClient::onSendComplete(const TcpConnectionPtr & conn) {
	contactTrans_.onWriteComplete(conn);
}

void TestContactTransferClient::onFileSended(const TcpConnectionPtr & conn) {
	std::cout << "send file success!" << std::endl;
	
	isOver_ = true;
	//std::cout << contactTrans_ << std::endl;
}

void TestContactTransferClient::sendContacts(const TcpConnectionPtr & conn, const std :: string & option) {
	isOver_ = false;
	contactTrans_.sendContactsByTcp(conn);
}

void TestContactTransferClient::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	//遍历连接红黑树,把消息发送到每一个tcp中
	conn->sendString(message);
}



int main(int argc, char** argv) {
	EventLoop* loop = new EventLoop();
	std::string ip("127.0.0.1");
	int port = 8032;
	std::string filename("./testContactTransfer.log");
	TestContactTransferClient client(loop, ip, port, filename);

	
	PersonStruct per;
	per.setId(1);
	per.setName("liangjingmei");
	per.setPriority(3);
	per.setEmail("LJM@qq.com");
	per.addPhoneNum(contactproto::Person_Groups_FRIEND, "18312476666");
	per.addPhoneNum(contactproto::Person_Groups_FAMILY, "13713596666");
	per.addPhoneNum(contactproto::Person_Groups_COLLEGE, "13633666666");

	PersonStruct p;
	p.setId(2);
	p.setName("wuxiao");
	p.setPriority(1);
	p.setEmail("WUX@qq.com");
	p.addPhoneNum(contactproto::Person_Groups_FRIEND, "222222222");
	p.addPhoneNum(contactproto::Person_Groups_FAMILY, "333333333");
	p.addPhoneNum(contactproto::Person_Groups_COLLEGE, "888888888");

	ContactTransfer& contactTrans = client.getContactTrans();
	contactTrans.addPeople(per);
	contactTrans.addPeople(p);
	contactTrans.serializeToOstream();
	
	client.start();
	usleep(100000);
	client.sendContacts(client.client_->getTcpConnectPtr());

	
		usleep(2000000);
		client.sendMessage(client.client_->getTcpConnectPtr(), "complete");
		usleep(1000000);
	
	
}








