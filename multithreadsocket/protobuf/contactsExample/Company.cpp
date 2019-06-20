#include "Company.h"
#include <iostream>
#include <fstream>
#include <map>

std::ostream& operator << (std::ostream& os, Company& company) {
	for (std::map<std::string, PersonStruct>::const_iterator it = company.getPersonMap().cbegin(); it != company.getPersonMap().cend(); ++it) {
		os << it->second << "\n";
	}
	os << company.getCompanyName() << " amount " << company.getCompanyName() << std::endl;
	return os;
}

Company::Company() : Contacts(), companyName_(), amount_(), company_() {

}

Company::~Company() {

}void Company::setNameAndAmount(const std::string& name, const int& amount) {
	companyName_ = name;
	amount_ = amount;
	company_.set_name(name);
	company_.set_amount(amount);
}

int Company::readAllPeople()  {
	listAllPeople(getPersonMap());
	std::cout << "company name: " << companyName_ << std::endl;
}



