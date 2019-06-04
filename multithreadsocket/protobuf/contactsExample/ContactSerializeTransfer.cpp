#include "ContactSerializeTransfer.h"
#include <boost/any.hpp>
#include <arpa/inet.h>
#include <sstream>
#include <stdint.h>




ContactSerializeTransfer::ContactSerializeTransfer(EventLoop * loop, const std :: string & ip, const int & port, const std :: string & name)
			: host_(), type_(CLIENT) {
	//TcpClient* cc = new TcpClient(loop, ip, port, name);
	std::shared_ptr<TcpClient> clientPtr = std::make_shared<TcpClient>(loop, ip, port, name);
	//Any.h 有bug，传入的值是智能指针，退出setHost函数作用域就把shared_ptr销毁，转义为any.使其use_count 减为1。
	//而本构造函数作用域一结束，use_count变为0，就把new出的TcpClient释放了，后面在any_cast也不能获取到有效的指针，所以崩溃。使用boost::any可以解决，但要引入boost库
	//此处的解决方案是,智能指针定义为static，局部静态变量.就不会让use_count 减到0
	setHost(clientPtr);
	//为什么这个智能指针引用的指针只被使用一次？应该是要有两次的呀？？
	std::cout << "client shared_ptr use_count: " << clientPtr.use_count() << std::endl;
	//TcpClient client(loop, ip, port, name);
	//setHost(client);	//报错，需要给TcpClient加一个拷贝构造函数试试
	clientPtr->setConnectionCallback(std::bind(&ContactSerializeTransfer::onConnection, this, std::placeholders::_1));
	clientPtr->setMessageCallback(std::bind(&ContactSerializeTransfer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	clientPtr->setSendCompleteCallback(std::bind(&ContactSerializeTransfer::onSendComplete, this, std::placeholders::_1));
}

ContactSerializeTransfer::ContactSerializeTransfer(EventLoop * loop, const int & port, const int & threadNum, const std :: string & name)
			: host_(), type_(SERVER) {
	//TcpServer* ss = new TcpServer(loop, port, threadNum);
	std::shared_ptr<TcpServer> serverPtr =  std::make_shared<TcpServer>(loop, port, threadNum);
	setHost(serverPtr);

	//TcpServer server(loop, port, threadNum);
	//setHost(server);
	serverPtr->setConnectionCallback(std::bind(&ContactSerializeTransfer::onConnection, this, std::placeholders::_1));
	serverPtr->setMessageCallback(std::bind(&ContactSerializeTransfer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	serverPtr->setSendCompleteCallback(std::bind(&ContactSerializeTransfer::onSendComplete, this, std::placeholders::_1));
}

ContactSerializeTransfer::~ContactSerializeTransfer() {

}

void ContactSerializeTransfer::init() {
	if (type_ == SERVER) {
		
	} else if (type_ == CLIENT) {
		
	}
}

int ContactSerializeTransfer::sendContactsByTcp(const TcpConnectionPtr& conn, const google::protobuf::Message& message) {
	std::string sendStr;
	//serializeToCodedOstream(sendStr);
	fillEmptyString(sendStr, message);

	conn->sendString(sendStr);
}

int ContactSerializeTransfer::fillEmptyString(std::string& str, const google::protobuf::Message& message) {
	str.clear();
	const std::string& typeName = message.GetTypeName();
	int nameLen = static_cast<int>(typeName.size() + 1);	//特意在name和data之间空一个byte
	int nameLenNet = htonl(nameLen);
	char* nl = static_cast<char*>(static_cast<void*>((&nameLenNet)));
	
	int dataSize = message.ByteSize();
	int len = dataSize + nameLen + MsgTypeNameLen;
	int lenNet = htonl(len);
	char* dl = static_cast<char*>(static_cast<void*>((&lenNet)));

	str.append(dl, PackLeng);
	str.append(nl, MsgTypeNameLen);

	str.append(typeName.c_str(), nameLen - 1);
	str.append(" ");	//特意在name和data之间空一个byte
	str.resize(PackLeng + MsgTypeNameLen + nameLen + dataSize);
	std::cout << "will fill nameLen: " << nameLen << " dataSize: " << dataSize << std::endl;
	unsigned char* dataBegin = static_cast<unsigned char*>(static_cast<void*>(&*str.begin() + PackLeng + MsgTypeNameLen + nameLen));	//得到str 的下一个append地址
	unsigned char* dataEnd = message.SerializeWithCachedSizesToArray(dataBegin);		//按地址将序列化后的数据放入string 对象
	if (dataEnd - dataBegin != dataSize) {
		std::cout << "message SerializeWithCachedSizesToArray failed" << std::endl;
		return -1;
	}
	return 0;

}

void ContactSerializeTransfer::receiveContactsFromTcp(const TcpConnectionPtr& conn, const std::string& message) {
	parseFromCodedStream(message);
	readAllPeople();
	
}

void ContactSerializeTransfer::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	conn->sendString(message);
}

void ContactSerializeTransfer::start() {
	if (type_ == SERVER) {
		//returnHost<TcpServer>().start();
		//const TcpServer& ss = any_cast<const TcpServer&>(getHost());
		const std::shared_ptr<TcpServer>& ss = boost::any_cast<const std::shared_ptr<TcpServer>&>(getHost());
		ss->start();
	} else {
		//returnHost<TcpClient>().start();
		//any_cast<const TcpClient&>(getHost());
		const std::shared_ptr<TcpClient>& cc = boost::any_cast<const std::shared_ptr<TcpClient>&>(getHost());
		cc->start();
	}
}

void ContactSerializeTransfer::onConnection(const TcpConnectionPtr& conn) {
	
	if (type_ == SERVER) {
		std::cout << "new connection socketfd->" << conn->fd() << std::endl;
	}
	else {
		std::cout << "connected to service"  << std::endl;
	}
}

void ContactSerializeTransfer::onMessage(const TcpConnectionPtr& conn, std::string& message) {
	//std::cout << message << std::endl;
	//receiveContactsFromTcp(conn, message);
	ContactSerializeTransfer::MessagePtr mess(parseDataPack(conn, message));
	contactproto::ContactBook* book = (contactproto::ContactBook*)mess.get();
	setContactBook(*book);
	readAllPeople();
	std::cout << *this << std::endl;
}

void ContactSerializeTransfer::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "contactBoock_ send complete" << std::endl;
}

ContactSerializeTransfer::MessagePtr ContactSerializeTransfer::parseDataPack(const TcpConnectionPtr& conn, std::string& message) {
	
	MessagePtr protobufMessage;
	std::cout << "receive message size: " << message.size() << std::endl;
	if (message.size() >= PackLeng) {
		std::string lenStr = message.substr(0, PackLeng);
		message.erase(0, PackLeng);
		const int lenNet = *static_cast<const int*>(static_cast<const void*>(lenStr.c_str()));
		const int len = ntohl(lenNet);
		if (len > 65536 || len < 0) {
			std::cout << "parseDataPack Invalid length" << len << std::endl;
			//return -1;
		} 
		//获取msgTypeName的长度
		std::string MsgTypeLenStr = message.substr(0, MsgTypeNameLen);
		message.erase(0, MsgTypeNameLen);
		const int msgTypeLenNet = *static_cast<const int*>(static_cast<const void*>(MsgTypeLenStr.c_str()));
		const int nameLen = ntohl(msgTypeLenNet);
		if (nameLen < 2 || nameLen > len - MsgTypeNameLen) {
			std::cout << "nameLen Invalid length" << len << std::endl;
			//return -1;
		}
		//根据msgTypeName的长度读取msgTypeName
		std::string msgTypeName = message.substr(0, nameLen - 1);	//在发送时，name和data中间空了一byte
		message.erase(0, nameLen );		//剔除msg type name 后，剩下的全是序列化的contacts数据了
		protobufMessage.reset(createProtobufMessage(msgTypeName));

		if (protobufMessage) {
			// parse from data
			const char* data = static_cast<const char*>(message.c_str());
			int32_t dataLen = len - MsgTypeNameLen - nameLen;
			std::cout << "protobufMessage dataLen" << dataLen << std::endl;
			if (!(protobufMessage->ParseFromArray(data, dataLen))) {
				std::cout << "protobufMessage Parse Error" << std::endl;
				//return -1;
			}
		} else {
			std::cout << "unknown Message Type" << std::endl;
		}
	}
	return protobufMessage;
}


google::protobuf::Message* ContactSerializeTransfer::createProtobufMessage(const std::string& typeName) {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);

	if (descriptor)
	{
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

std::ostream& operator<<(std::ostream& os, const ContactSerializeTransfer& contact) {
	
	for (std::map<std::string, PersonStruct>::const_iterator it = contact.getPersonMap().cbegin(); it != contact.getPersonMap().cend(); ++it) {
		os << it->second << std::endl;
	}
	return os;
}

const int ContactSerializeTransfer::PackLeng = 4;
const int ContactSerializeTransfer::MsgTypeNameLen = 4;







