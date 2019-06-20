
#ifndef _CONTACTS_H_
#define _CONTACTS_H_
#include <ctime>


#include <string>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "contacts.pb.h"
#include "PersonStruct.h"

class Contacts {
public:
	typedef std::map<std::string, PersonStruct> PersonStructMap;
	Contacts();
	Contacts(const std::string& filename);	//电话簿文件
	virtual ~Contacts();

	void addPeople(const PersonStruct& person);
	
	void clearAllPeople() {
		contactBook_.clear_people();
	}
	virtual int readAllPeople();
	void listAllPeople(PersonStructMap& personMap);
	int serializeToOstream();
	int serializeToCodedOstream(std::string& outString);
	int parseFromCodedStream(const std::string& message);
	

	//前一个是protobuf 必须传入 contactproto :: Person* contactBook::addPeople() . 作为一个指针传入,在函数内给指针赋值就可以达到添加的效果,后一个是用户想添加people的信息
	void addPeople(contactproto :: Person * people, const PersonStruct& person);
	
	void addPeoplePhoneNum(contactproto::Person_PhoneNumber* phoneNum, const PhoneNum& num);

	//返回左值引用，要在外部修改
	contactproto::ContactBook& getContactBook() {
		return contactBook_;
	}
	void setContactBook(const contactproto::ContactBook& book) {
		contactBook_ = book;
	}
	std::string& getFilename() {
		return filename_;
	}
	//返回常引用,不需要在外部修改
	PersonStructMap& getPersonMap() {
		return personMap_;
	}

	friend std::ostream& operator<< (std::ostream& os, Contacts& contacts);
	
private:
	contactproto::ContactBook contactBook_;
	std::string filename_;
	PersonStructMap personMap_;
};


#endif
