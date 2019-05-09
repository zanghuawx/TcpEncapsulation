
#ifndef _CONTACTS_H_
#define _CONTACTS_H_
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "contacts.pb.h"

class Contacts {
public:
	Contacts(const std::string& filename);	//电话簿文件
	~Contacts();
	
	void AddPeople(contactproto::Person* person);
	void clearAllPeople() {
		contactBook_.clear_people();
	}
	
private:
	contactproto::ContactBook contactBook_;
};


#endif
