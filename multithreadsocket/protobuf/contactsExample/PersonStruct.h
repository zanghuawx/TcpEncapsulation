
#ifndef _PERSONSTRUCT_H_
#define _PERSONSTRUCT_H_

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include "contacts.pb.h"



class PersonStruct {
public:
	PersonStruct() {
		
	}
	~PersonStruct() {
		
	}
	class PhoneNum {
	public:
		PhoneNum() {
			
		}
		~PhoneNum() {
			
		}
		std::string getNumber() const {
			return number;
		}
		void setNumber(const std::string& number) {
			number_ = number;
		}
		
		contactproto::Person_Groups getGroup() const {
			return group_;
		}
		void setGroup(const contactproto::Person_Groups& group) {
			group_ = group;
		}
	private:
		std::string number_;
		contactproto::Person_Groups group_;
	};
	
	std::string getName() const {
		return name_;
	}
	void setName(const std::string& name) {
		name_ = name;
	}
	
	int getId() const {
		return id_;
	}
	void setId(const int& id) {
		id_ = id;
	}
	
	int getPriority() const {
		return priority_;
	}
	void setPriority(const int& priority) {
		priority_ = priority;
	}
	
	std::string getEmail() const {
		return email_;
	}
	void setEmail(const std::string& email) {
		email_ = email;
	}
	
	std::list<PhoneNum>& getPhoneNumList() const {
		return phoneNumList_;
	}
	
	void addPhoneNum(const PhoneNum& num) {
		phoneNumList_.push_back(num);
	}
	
	
private:
	std::string name_;
	int id_;
	std::list<PhoneNum> phoneNumList_;
	int priority_;
	std::string email_;
	
};


#endif


