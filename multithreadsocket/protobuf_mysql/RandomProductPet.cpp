#include "RandomProductPet.h"


RandomProductPet::RandomProductPet() {
	srand((unsigned int)time(NULL));	//初始化随机数种子
	name();
	owner();
	species();
	sex();
	birth();
	death();
}
RandomProductPet::~RandomProductPet() {
}
void RandomProductPet::name() {
	specifiedRandStringLength(name_, 9);
}
void RandomProductPet::owner() {
	specifiedRandStringLength(owner_, 9);
}
void RandomProductPet::species() {
	specifiedRandStringLength(species_, 6);
}
void RandomProductPet::sex() {
	specifiedRandStringLength(sex_, 1);
}
void RandomProductPet::birth() {
	unsigned int year, month, day;
	productDate(year, month, day);
	year += 2000;
	mergeDate(birth_, year, month, day);
}

void RandomProductPet::death() {
	unsigned int year, month, day;
	productDate(year, month, day);
	year += 2100;
	mergeDate(death_, year, month, day);
}


char RandomProductPet::asciiWord() {
	char tmp = 0;

	tmp = rand() % 122;
	if (tmp <= 25) {
		tmp += 97;
	}
	else if (tmp > 25 && tmp < 52) {
		tmp += 71;
	}
	else if (tmp >= 52 && tmp < 71) {
		tmp += 45;
	}
	else if (tmp >= 71 && tmp < 97) {
		tmp += 26;
	}
	return tmp;
}
void RandomProductPet::specifiedRandStringLength(std::string& str, int length) {
	str.clear();
	while (length-- > 0) {
		str.append(1, asciiWord());
	}
}
void RandomProductPet::productDate(unsigned int& year, unsigned int& month, unsigned int& day) {

	year = (unsigned)rand() % 99;
	month = (unsigned)rand() % 12 + 1;
	day = (unsigned)rand() % 30 + 1;
}
void RandomProductPet::mergeDate(std::string& date, unsigned int& year, unsigned int& month, unsigned int& day) {
	std::string yearStr;
	std::string monthStr;
	std::string dayStr;
	std::stringstream ss;
	ss << year;
	ss >> yearStr;
	ss.clear();
	ss << month;
	ss >> monthStr;
	if (month < 10) {
		monthStr = "0" + monthStr;
	}
	ss.clear();
	ss << day;
	ss >> dayStr;
	if (day < 10) {
		dayStr = "0" + monthStr;
	}
	date = yearStr + "-" + monthStr + "-" + dayStr;
}
