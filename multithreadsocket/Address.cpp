/**********************************
	*包含的头文件中有以下结构体:
	
	typedef uint32_t in_addr_t;
	struct in_addr {
		in_addr_t s_addr;		//32位IPV4地址
	};
	
	struct sockaddr_in {  
		short			 sin_family;	   // 2 bytes e.g. AF_INET, AF_INET6  
		unsigned short	 sin_port;	  // 2 bytes e.g. htons(3490)  
		struct in_addr	 sin_addr;	   // 4 bytes see struct in_addr, below  
		char			 sin_zero[8];	  // 8 bytes zero this if you want to  
	};	
	
	**********************************/

#include "Address.h"


void Address::_address(std::string ip, unsigned int port)
{
    this->sin_family = AF_INET;
    this->ip(ip);
    this->port(port);
}

Address::Address() {
	_address("0.0.0.0", 0);
}
Address::Address(unsigned int port) {
	_address("0.0.0.0", port);
}
Address::Address(std::string ip, unsigned int port) {
	_address(ip, port);
}
Address::Address(struct sockaddr_in addr) {
	_address(inet_ntoa(addr.sin_addr), addr.sin_port);
}
Address::Address(const Address & addr) {
	this->sin_port = addr.sin_port;
	this->sin_addr = addr.sin_addr;
	this->sin_family = addr.sin_family;
}

//获取ip
std::string Address::ip() {
	return inet_ntoa(this->sin_addr);
}
//设置ip
std::string Address::ip(std::string ip) {
	if (inet_aton(ip.c_str(), &this->sin_addr) == 0) {
		
		std::cout << "[ip] with [ip=" << ip << "] Invalid ip address provided" << std::endl;
		
	}
	return this->ip();
}

unsigned int Address::port() {
	return ntohs(this->sin_port);
}

unsigned int Address::port(unsigned int port) {
	this->sin_port = htons(port);
	return this->port();
}

std::ostream& operator<< (std::ostream &out, Address &address)
{
	out << address.ip() << ":" << address.port();
	return out;
}
