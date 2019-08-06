#include "PetProtocMessCodec.h"
#include "pet.pb.h"

PetProtocMessCodec::PetProtocMessCodec(EventLoop* loop, const std::string& ip, const int& port, const std::string& name) : 
					ProtocolMessageCodec(loop, ip, port, name), petRowContent_()/*, name_(), owner_(), species_(), sex_(), birth_(), death_()*/ {

}
PetProtocMessCodec::PetProtocMessCodec(EventLoop* loop, const int& port, const int& threadNum, const std::string& name) :
					ProtocolMessageCodec(loop, port, threadNum, name), petRowContent_()/*, name_(), owner_(), species_(), sex_(), birth_(), death_()*/ {

}
PetProtocMessCodec::~PetProtocMessCodec() {

}

void PetProtocMessCodec::init() {
	parseHandlerMap_[TypePet] = std::bind(&PetProtocMessCodec::parseTypePet, this, std::placeholders::_1);
}

void PetProtocMessCodec::onMessage(const TcpConnectionPtr& conn, std::string& message) {

	ProtocolMessageCodec::MessagePtr mess(parseDataPack(conn, message));

	const std::string& typeName = mess->GetTypeName();
	//parseHandlerMap_[TypeContacts] = std::bind(&ProtocolMessageCodec::parseTypeContacts, this, std::placeholders::_1);
	std::cout << "parse typeName: " << typeName << std::endl;

	parseHandlerMap_[typeName](mess);

	//接下来一步，把解析的protocol类信息，存入数据库
	if (messageCallback_(petRowContent_) == -1) {
		std::cout << "access row failed!" << std::endl;
	}


	/*
	contactproto::ContactBook* book = (contactproto::ContactBook*)mess.get();
	setContactBook(*book);
	readAllPeople();
	std::cout << *this << std::endl;
	*/
}

void PetProtocMessCodec::onSendComplete(const TcpConnectionPtr& conn) {
	std::cout << "contactBoock_ send complete" << std::endl;
}

void PetProtocMessCodec::parseTypePet(const MessagePtr& message) {
	petproto::PetProto* pet = (petproto::PetProto*)message.get();
	petRowContent_.clear();
	petRowContent_.push_back(pet->name());
	petRowContent_.push_back(pet->owner());
	petRowContent_.push_back(pet->species());
	petRowContent_.push_back(pet->sex());
	petRowContent_.push_back(pet->birth());
	petRowContent_.push_back(pet->death());
	//name_.assign(pet->name());
	//owner_.assign(pet->owner());
	//species_.assign(pet->species());
	//sex_.assign(pet->sex());
	//birth_.assign(pet->birth());
	//death_.assign(pet->death());
}



const std::string PetProtocMessCodec::TypePet = "contactproto.PetProto";
