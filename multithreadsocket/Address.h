
#ifndef _ADDRESS_H_
#define _ADDRESS_H_
#include <iostream>
#include <sstream>
#include <exception>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/ioctl.h>


struct Address : public sockaddr_in {
private:
	void _address(std::string ip, unsigned int port);
public:
	Address();
	Address(unsigned int port);
	Address(std::string ip, unsigned int port);
	Address(struct sockaddr_in addr);
	Address(const Address& addr);

	std::string ip();
	std::string ip(std::string ip);
	
	unsigned int port();
	unsigned int port(unsigned int port);
	friend std::ostream& operator<< (std::ostream& out, Address& addr);
};


#endif




