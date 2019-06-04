
#include "PersonStruct.h"






std::ostream& operator<<(std::ostream& os, const PersonStruct& person) {
	os << "name: " << person.getName() << " id: " << person.getId() << " email: " << person.getEmail()
		<< " priority: " << person.getPriority() << "\n";
	
	const std::list<PhoneNum>& phn =  person.phoneNumList_;
	int i = 1;
	for (auto it : phn) {
		os << " No." << i << ": " << it.getNumber() << " group: " << static_cast<int>(it.getGroup()) << "\n";
		++i;
	}
	return os;
}



