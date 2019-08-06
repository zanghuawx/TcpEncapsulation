#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sstream>


class RandomProductPet
{
public:
	RandomProductPet();
	~RandomProductPet();
	void name();
	void owner();
	void species();
	void sex();
	void birth();
	void death();

	inline const std::string& get_name() const {
		return name_;
	}
	inline const std::string& get_owner() const {
		return owner_;
	}
	inline const std::string& get_species() const {
		return species_;
	}
	inline const std::string& get_sex() const {
		return sex_;
	}
	inline const std::string& get_birth() const {
		return birth_;
	}
	inline const std::string& get_death() const {
		return death_;
	}


private:
	char asciiWord();
	void specifiedRandStringLength(std::string& str, int length);
	void productDate(unsigned int& year, unsigned int& month, unsigned int& day);
	void mergeDate(std::string& date, unsigned int& year, unsigned int& month, unsigned int& day);

	std::string name_;
	std::string owner_;
	std::string species_;
	std::string sex_;
	std::string birth_;
	std::string death_;
};

