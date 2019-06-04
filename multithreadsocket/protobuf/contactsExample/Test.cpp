
#include "Contacts.h"
#include "PersonStruct.h"
#include "contacts.pb.h"


int main() {
	/*
	Contacts contact("test.log");
	
	PersonStruct per;
	per.setId(1);
	per.setName("liangjingmei");
	per.setPriority(3);
	per.setEmail("LJM@qq.com");
	per.addPhoneNum(contactproto::Person_Groups_FRIEND, "18312476666");
	per.addPhoneNum(contactproto::Person_Groups_FAMILY, "13713596666");
	per.addPhoneNum(contactproto::Person_Groups_COLLEGE, "13633666666");

	PersonStruct p;
	p.setId(2);
	p.setName("wuxiao");
	p.setPriority(1);
	p.setEmail("WUX@qq.com");
	p.addPhoneNum(contactproto::Person_Groups_FRIEND, "222222222");
	p.addPhoneNum(contactproto::Person_Groups_FAMILY, "333333333");
	p.addPhoneNum(contactproto::Person_Groups_COLLEGE, "888888888");

	contact.addPeople(per);
	contact.addPeople(p);
	contact.serializeToOstream();
	*/
	
	
	Contacts contact("test.log");
	contact.readAllPeople();
	std::map<std::string, PersonStruct> personMap;
	contact.listAllPeople(personMap);
	std::cout << "executy" << std::endl;
	for (std::map<std::string, PersonStruct>::iterator it = personMap.begin(); it != personMap.end(); ++it) {
		//std::cout << "name: " << it->first << " email: " << it->second.getEmail() << std::endl;
		
		std::cout << it->second << std::endl;
	}
	
	return 0;
}
