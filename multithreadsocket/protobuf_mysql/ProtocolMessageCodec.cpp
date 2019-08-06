#include "ProtocolMessageCodec.h"
#include <boost/any.hpp>
#include <arpa/inet.h>
#include <sstream>
#include <stdint.h>




ProtocolMessageCodec::ProtocolMessageCodec(EventLoop * loop, const std :: string & ip, const int & port, const std :: string & name)
			: host_(), type_(CLIENT), parseHandlerMap_() {
	//TcpClient* cc = new TcpClient(loop, ip, port, name);
	std::shared_ptr<TcpClient> clientPtr = std::make_shared<TcpClient>(loop, ip, port, name);
	//Any.h ��bug�������ֵ������ָ�룬�˳�setHost����������Ͱ�shared_ptr���٣�ת��Ϊany.ʹ��use_count ��Ϊ1��
	//�������캯��������һ������use_count��Ϊ0���Ͱ�new����TcpClient�ͷ��ˣ�������any_castҲ���ܻ�ȡ����Ч��ָ�룬���Ա�����ʹ��boost::any���Խ������Ҫ����boost��
	//�˴��Ľ��������,����ָ�붨��Ϊstatic���ֲ���̬����.�Ͳ�����use_count ����0
	setHost(clientPtr);
	//Ϊʲô�������ָ�����õ�ָ��ֻ��ʹ��һ�Σ�Ӧ����Ҫ�����ε�ѽ����
	std::cout << "client shared_ptr use_count: " << clientPtr.use_count() << std::endl;
	//TcpClient client(loop, ip, port, name);
	//setHost(client);
	clientPtr->setConnectionCallback(std::bind(&ProtocolMessageCodec::onConnection, this, std::placeholders::_1));
	clientPtr->setMessageCallback(std::bind(&ProtocolMessageCodec::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	clientPtr->setSendCompleteCallback(std::bind(&ProtocolMessageCodec::onSendComplete, this, std::placeholders::_1));
	init();
}

ProtocolMessageCodec::ProtocolMessageCodec(EventLoop * loop, const int & port, const int & threadNum, const std :: string & name)
			: host_(), type_(SERVER), parseHandlerMap_() {
	std::shared_ptr<TcpServer> serverPtr =  std::make_shared<TcpServer>(loop, port, threadNum);
	setHost(serverPtr);

	serverPtr->setConnectionCallback(std::bind(&ProtocolMessageCodec::onConnection, this, std::placeholders::_1));
	serverPtr->setMessageCallback(std::bind(&ProtocolMessageCodec::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	serverPtr->setSendCompleteCallback(std::bind(&ProtocolMessageCodec::onSendComplete, this, std::placeholders::_1));
	init();
}

ProtocolMessageCodec::~ProtocolMessageCodec() {

}

void ProtocolMessageCodec::init() {
	parseHandlerMap_["default"] = std::bind(&ProtocolMessageCodec::parseType, this, std::placeholders::_1);
	/*��������͵�protobuf��...............*/
}

int ProtocolMessageCodec::sendContactsByTcp(const TcpConnectionPtr& conn, const google::protobuf::Message& message) {
	std::string sendStr;

	fillEmptyString(sendStr, message);

	conn->sendString(sendStr);
}

int ProtocolMessageCodec::fillEmptyString(std::string& str, const google::protobuf::Message& message) {
	str.clear();
	const std::string& typeName = message.GetTypeName();
	int nameLen = static_cast<int>(typeName.size() + 1);	//������name��data֮���һ��byte
	int nameLenNet = htonl(nameLen);
	char* nl = static_cast<char*>(static_cast<void*>((&nameLenNet)));
	
	int dataSize = message.ByteSize();
	int len = dataSize + nameLen + MsgTypeNameLen;
	int lenNet = htonl(len);
	char* dl = static_cast<char*>(static_cast<void*>((&lenNet)));

	str.append(dl, PackLeng);
	str.append(nl, MsgTypeNameLen);

	str.append(typeName.c_str(), nameLen - 1);
	str.append(" ");	//������name��data֮���һ��byte
	str.resize(PackLeng + MsgTypeNameLen + nameLen + dataSize);
	std::cout << "will fill nameLen: " << nameLen << " dataSize: " << dataSize << std::endl;
	unsigned char* dataBegin = static_cast<unsigned char*>(static_cast<void*>(&*str.begin() + PackLeng + MsgTypeNameLen + nameLen));	//�õ�str ����һ��append��ַ
	unsigned char* dataEnd = message.SerializeWithCachedSizesToArray(dataBegin);		//����ַ�����л�������ݷ���string ����
	if (dataEnd - dataBegin != dataSize) {
		std::cout << "message SerializeWithCachedSizesToArray failed" << std::endl;
		return -1;
	}
	return 0;

}



void ProtocolMessageCodec::sendMessage(const TcpConnectionPtr& conn, const std::string& message) {
	conn->sendString(message);
}

void ProtocolMessageCodec::start() {
	if (type_ == SERVER) {

		const std::shared_ptr<TcpServer>& ss = boost::any_cast<const std::shared_ptr<TcpServer>&>(getHost());
		ss->start();
	}
	else {

		const std::shared_ptr<TcpClient>& cc = boost::any_cast<const std::shared_ptr<TcpClient>&>(getHost());
		cc->start();
	}
}

void ProtocolMessageCodec::onConnection(const TcpConnectionPtr& conn) {
	
	if (type_ == SERVER) {
		std::cout << "new connection socketfd->" << conn->fd() << std::endl;
	}
	else {
		std::cout << "connected to service"  << std::endl;
	}
}

void ProtocolMessageCodec::onMessage(const TcpConnectionPtr& conn, std::string& message) {

	ProtocolMessageCodec::MessagePtr mess(parseDataPack(conn, message));
	
	const std::string& typeName = mess->GetTypeName();
	//parseHandlerMap_[TypeContacts] = std::bind(&ProtocolMessageCodec::parseTypeContacts, this, std::placeholders::_1);
	std::cout << "parse typeName: " << typeName << std::endl;
	
	parseHandlerMap_["default"](mess);
	
	
	

	/*
	contactproto::ContactBook* book = (contactproto::ContactBook*)mess.get();
	setContactBook(*book);
	readAllPeople();
	std::cout << *this << std::endl;
	*/
}

void ProtocolMessageCodec::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "contactBoock_ send complete" << std::endl;
}

ProtocolMessageCodec::MessagePtr ProtocolMessageCodec::parseDataPack(const TcpConnectionPtr& conn, std::string& message) {
	
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
		//��ȡmsgTypeName�ĳ���
		std::string MsgTypeLenStr = message.substr(0, MsgTypeNameLen);
		message.erase(0, MsgTypeNameLen);
		const int msgTypeLenNet = *static_cast<const int*>(static_cast<const void*>(MsgTypeLenStr.c_str()));
		const int nameLen = ntohl(msgTypeLenNet);
		if (nameLen < 2 || nameLen > len - MsgTypeNameLen) {
			std::cout << "nameLen Invalid length" << len << std::endl;
			//return -1;
		}
		//����msgTypeName�ĳ��ȶ�ȡmsgTypeName
		std::string msgTypeName = message.substr(0, nameLen - 1);	//�ڷ���ʱ��name��data�м����һbyte
		message.erase(0, nameLen );		//�޳�msg type name ��ʣ�µ�ȫ�����л���contacts������
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


google::protobuf::Message* ProtocolMessageCodec::createProtobufMessage(const std::string& typeName) {
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

void ProtocolMessageCodec::parseType(const MessagePtr& message) {
	std::cout << "parse type..." << std::endl;
}

std::ostream& operator<<(std::ostream& os, const ProtocolMessageCodec& contact) {
	
	
	return os;
}

const int ProtocolMessageCodec::PackLeng = 4;
const int ProtocolMessageCodec::MsgTypeNameLen = 4;








