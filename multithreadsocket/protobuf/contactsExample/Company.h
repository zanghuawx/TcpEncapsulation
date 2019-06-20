
#ifndef _COMPANY_H_
#define _COMPANY_H_

#include <ctime>
#include <string>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "contacts.pb.h"
#include "PersonStruct.h"
#include "Contacts.h"

class Company : public Contacts {
public:
	Company();
	~Company();

	int readAllPeople() override;

	void setNameAndAmount(const std::string& name, const int& amount);
	const std::string& getCompanyName() const {
		return companyName_;
	}
	const int& getAmount() const {
		return amount_;
	}
	void setContactbookPtr(contactproto::ContactBook* book) {
		company_.set_allocated_contactbook(book);
	}
	void setContactbookPtr() {
		company_.set_allocated_contactbook(&getContactBook());
	}
	/*
	const contactproto::ContactBook& getContactbook() const {
		return company_.contactbook();
	}
	*/

	contactproto::CompanyInfo& getCompany() {
		return company_;
	}
	void setCompany(const contactproto::CompanyInfo& company) {
		company_ = company;
	}

	friend std::ostream& operator << (std::ostream& os, Company& company);

private:
	std::string companyName_;
	int amount_;
	contactproto::CompanyInfo company_;
};

#endif

