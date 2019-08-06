#include "ContactSerializeTransfer.h"
#include <time.h>
#include <unistd.h>

int main() {
	EventLoop* loop = new EventLoop();
	std::string ip("127.0.0.1");
	int port = 8032;
	ContactSerializeTransfer client(loop, ip, port, "haha");
	client.start();

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

	Contacts contact;
	contact.addPeople(per);
	contact.addPeople(p);

	contactproto::Person pers;
	pers.set_name("wuqianyu");
	pers.set_email("wuqianyu@qq.com");
	pers.set_id(3);
	pers.set_priority(2);


	const std::shared_ptr<TcpClient>& cc = boost::any_cast<const std::shared_ptr<TcpClient>&>(client.getHost());
	std::cout << "usleep(800000)" << std::endl;
	usleep(800000);
	if (cc->isConnect()) {
		//client.sendContactsByTcp(cc->getTcpConnectPtr());
		std::string str;
		google::protobuf::Message* mess = &contact.getContactBook();
		google::protobuf::Message* perso = &pers;
		client.sendContactsByTcp(cc->getTcpConnectPtr(), *mess);
		usleep(1000000);
		std::cout << pers.name() << pers.email() << pers.id() << std::endl;
		client.sendContactsByTcp(cc->getTcpConnectPtr(), *perso);
	}
	


	usleep(3000000);
	
}
