#ifndef _PROTOCOLMESSAGECODEC_H_
#define _PROTOCOLMESSAGECODEC_H_

#include "../TcpServer.h"
#include "../TcpClient.h"
#include "../TcpConnection.h"
#include "../Any.h"
#include "pet.pb.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <time.h>
#include <boost/any.hpp>



/*len（4个字节）  
message Type nameLen(4个字节)  
message Type name (nameLen byte,end with \0)  
protobuf data ( len-nameLen-8 byte） 
checkSum (4个字节)  
以上数据表示的含义为： 
len：占4个字节，它表示除len占据的4个字节外整个消息的长度，即nameLen + name + probobuf data + checkSum占的字节数； 
message Type nameLen：占4个字节，表示消息类型名的长度 
message Type name：占nameLen个字节，表示消息类型名 
protobuf data：占 len-nameLen-8个字节，表示对象序列化后的字节序 
*/

typedef enum HostType {
	SERVER = 0,
	CLIENT
}HOSTTYPE;

//不能直接引用基类的私有成员, 只能通过基类的公有函数间接的操作基类的私有成员
class ProtocolMessageCodec {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
	typedef std::function<void(const MessagePtr&)> Callback;

	ProtocolMessageCodec(EventLoop * loop, const std::string& ip, const int& port, const std::string& name = "client");
	ProtocolMessageCodec(EventLoop * loop,             const int& port, const int& threadNum, const std::string& name = "server");
	virtual ~ProtocolMessageCodec();

	virtual void init();
	
	int sendContactsByTcp(const TcpConnectionPtr& conn, const google::protobuf::Message& message);
	int fillEmptyString(std::string& str, const google::protobuf::Message& message);



	//用于测试
	virtual void sendMessage(const TcpConnectionPtr& conn, const std::string& message);

	void start();
	void onConnection(const TcpConnectionPtr& conn);
	virtual void onMessage(const TcpConnectionPtr& conn, std::string& message);
	virtual void onSendComplete(const TcpConnectionPtr& conn);

	const boost::any& getHost() const {
		return host_;
	}
	void setHost(const boost::any& host) {
		host_ = host;
		if (type_ == CLIENT) {

			std::cout << " set client shared_ptr use_count: " << boost::any_cast<const std::shared_ptr<TcpClient>&>(host_).use_count() << std::endl;
		}

	}

	//获取
	MessagePtr parseDataPack(const TcpConnectionPtr& conn, std::string& message);
	google::protobuf::Message* createProtobufMessage(const std::string& typeName);

	void parseType(const MessagePtr& message);
	
	
	friend std::ostream& operator<<(std::ostream& os, const ProtocolMessageCodec& contact);

	static const int PackLeng;
	static const int MsgTypeNameLen;

	std::map<std::string, Callback> parseHandlerMap_;

private:
	boost::any host_;
	HOSTTYPE type_;



};


#endif


