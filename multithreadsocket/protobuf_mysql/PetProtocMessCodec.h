
#ifndef _PETPROTOCMESSCODEC_H_
#define _PETPROTOCMESSCODEC_H_

#include "ProtocolMessageCodec.h"
class PetProtocMessCodec :
	public ProtocolMessageCodec {
public:
	typedef std::function<int(std::vector<std::string>&)> MessageHandler;
	PetProtocMessCodec(EventLoop* loop, const std::string& ip, const int& port, const std::string& name = "client");
	PetProtocMessCodec(EventLoop* loop, const int& port, const int& threadNum, const std::string& name = "server");
	~PetProtocMessCodec();

	void init() override;
	void onMessage(const TcpConnectionPtr& conn, std::string& message) override;
	void onSendComplete(const TcpConnectionPtr& conn) override;

	void parseTypePet(const MessagePtr& message);
	void setMessageCallback(MessageHandler cb) {
		messageCallback_ = cb;
	}
	static const std::string TypePet;
private:
	std::function<int (std::vector<std::string>&)> messageCallback_;
	std::vector<std::string> petRowContent_;
	//std::string name_;
	//std::string owner_;
	//std::string species_;
	//std::string sex_;
	//std::string birth_;
	//std::string death_;
};

#endif


