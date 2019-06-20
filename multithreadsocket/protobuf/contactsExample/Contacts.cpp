#include "Contacts.h"
#include "PersonStruct.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

std::ostream& operator<< (std::ostream& os, Contacts& contacts) {
	
	for (std::map<std::string, PersonStruct>::iterator it = contacts.getPersonMap().begin(); it != contacts.getPersonMap().end(); ++it) {
		os << it->second << std::endl;
	}
	return os;
}

Contacts::Contacts() : contactBook_(), filename_(), personMap_() {
	
}


Contacts::Contacts(const std :: string & filename) : contactBook_(), filename_(filename), personMap_() {
	
}
Contacts::~Contacts() {

}

void Contacts::addPeople(const PersonStruct& person) {
	addPeople(contactBook_.add_people(), person);
}





int Contacts::readAllPeople() {
	/*
	std::fstream ifs(filename_, ios::in | ios::binary);
	if (!contactBook_.ParseFromIstream(&ifs)) {
		std::cout << "Failed to parse contact book_." << std::endl;
		return -1;
	}
	*/
	listAllPeople(personMap_);
	return 0;
}

void Contacts::listAllPeople(PersonStructMap& personMap) {
	//contact book have number of person
	for (int i = 0; i < contactBook_.people_size(); i++) {
		const contactproto::Person& person = contactBook_.people(i);
		std::cout << "person name: " << person.name() << " email: " << person.email() << std::endl;

		PersonStruct personStruct;
		personStruct.phoneNumList_.clear();
		
		for (int j = 0; j < person.phones_size(); j++) {
			const contactproto::Person_PhoneNumber& personPhoneNum = person.phones(j);


			const std::string& number = personPhoneNum.number();
			const contactproto::Person_Groups group = personPhoneNum.group();
			
			
			PhoneNum phoneNum;
			phoneNum.setNumber(number);
			phoneNum.setGroup(group);

			personStruct.phoneNumList_.push_back(phoneNum);

			/*父类强转为子类的问题，dynamic_cast<> 需要父类有虚函数多态和子类指针向上转型，例如
			Sub su(10, "father");
			Base* b = &su;	//go up cast type, implement multi state
			Sub* s = dynamic_cast<Sub*>(b);
			*/
			
		}
		
		personStruct.setEmail(person.email());
		personStruct.setId(person.id());
		personStruct.setName(person.name());
		personStruct.setPriority(person.priority());

		personMap.insert(std::pair<std::string, PersonStruct>(person.name(), personStruct)) ;
	}
}

int Contacts::serializeToOstream() {
	//添加完person后,实行序列化,把所有信息重新序列化到文件
	std::fstream output(filename_, ios::out | ios::trunc | ios::binary);
	if (!contactBook_.SerializeToOstream(&output)) {
		std::cout << "contact book name: " << filename_ << " serial failed" << std::endl;
		return -1;
	}
	return 0;
}

//将contacts序列化成ostream,再assign给string对象
int Contacts::serializeToCodedOstream(std::string& outString) {
	int len = contactBook_.ByteSize() + 4;
	char* buffer = new char[len];

	//执行完下四步操作，就把contactbook_系列化到了CodeOutputStream,而输出流又绑定输出到了char* 中
	//这样就可以发送取出了
	google::protobuf::io::ArrayOutputStream arrayOut(buffer, len);
	google::protobuf::io::CodedOutputStream codeOut(&arrayOut);
	codeOut.WriteVarint32(contactBook_.ByteSize());		//WriteVarint32()的作用是写入长度,variant可根据长度自动调整长度标识所占用的大小
	if (!contactBook_.SerializeToCodedStream(&codeOut)) {
		std::cout << "serialize contactBook_ to Coded Stream failed";
		return -1;
	}

	outString.clear();
	outString = buffer;	//这里可以直接赋值

	delete[] buffer;
	return len;
}

int Contacts::parseFromCodedStream(const std::string& message) {
	int len = message.size();
	const char* buffer = new char[len];
	buffer = message.c_str();
	google::protobuf::io::ArrayInputStream arrayIn(buffer, len);
	google::protobuf::io::CodedInputStream codeIn(&arrayIn);
	//codeIn.SetTotalBytesLimit(10485760, 5242880‬);
	bool success = contactBook_.ParseFromCodedStream(&codeIn);
	if (!success) {
		std::cout << "parse contactBook_ from Coded Stream failed" << std::endl;
		return -1;
	}
	//int readLen = codeIn.ReadVarint32();
	delete[] buffer;
	return len;
}

void Contacts::addPeople(contactproto :: Person * people, const PersonStruct& person) {
	std::cout << "add person name: " << person.getName() << std::endl;
	people->set_name(person.getName());
	people->set_id(person.getId());

	const std::list<PhoneNum>& phoneNumList = person.phoneNumList_;
	for (auto au : phoneNumList) {
		addPeoplePhoneNum(people->add_phones(), au);
	}

	people->set_priority(person.getPriority());
	people->set_email(person.getEmail());

}

void Contacts::addPeoplePhoneNum(contactproto::Person_PhoneNumber* phoneNum, const PhoneNum& num) {
	phoneNum->set_number(num.getNumber());
	phoneNum->set_group(num.getGroup());
}










