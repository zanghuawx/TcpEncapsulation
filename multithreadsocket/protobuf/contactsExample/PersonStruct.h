
#ifndef _PERSONSTRUCT_H_
#define _PERSONSTRUCT_H_

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include "contacts.pb.h"



class PhoneNum {
public:
	PhoneNum() : number_(), group_() {
		
	}
	PhoneNum(const std::string& number, const contactproto::Person_Groups& group) : number_(number), group_(group) {
		
	}
	~PhoneNum() {
		
	}
	std::string getNumber() const {
		return number_;
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

class PersonStruct		     {
public:
	PersonStruct() : name_(), id_(), phoneNumList_(), priority_(), email_() {
		
	}
	PersonStruct(const std::string& name, const int& id, const int& priority, const std::string email ) : name_(name), id_(id), 
					phoneNumList_(), priority_(priority), email_(email) {
		
	}
	~PersonStruct() {
		
	}
	
	
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

	//是返回list还是返回list引用？  经测试,容器还是直接返回,拷贝一下就可以了,返回容器的引用容易出问题。
	//不过返回常左值引用就没问题
	const std::list<PhoneNum>& getPhoneNumList() const {
		const std::list<PhoneNum>& leftReference = phoneNumList_;
		return leftReference;
	}
	void addPhoneNum(const contactproto::Person_Groups& group,  const std::string& number) {
		PhoneNum num;
		num.setGroup(group);
		num.setNumber(number);
		phoneNumList_.push_back(num);
	}
	void addPhoneNum(const PhoneNum& num) {
		phoneNumList_.push_back(num);
	}

	//因为没有办法修改 ostream 类和 istream 类,所以只能将<<和>>重载为全局函数的形式,由于这两个函数需要访问  类的私有成员,因此在  类定义中将它们声明为友元
	friend std::ostream& operator<<(std::ostream& os, const PersonStruct& person);
	
	std::list<PhoneNum> phoneNumList_;
	
private:
	std::string name_;
	int id_;
	
	int priority_;
	std::string email_;
	
};




#endif


