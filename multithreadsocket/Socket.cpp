#include "Address.h"
#include "Socket.h"
#include <sys/select.h>
#include <sys/ioctl.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

//系统调用 int socket(int domain, int type, int protocol); 创建一个套接字
//domain:AF_INET ipv4    type:SOCKET_STREAM tcp连接流套接字  protocol: 0就好了
Socket::Socket() {
	//创建一个tcp ipv4的socket
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ == -1) {
		perror("socket create fail!");
		exit(-1);
	}
	std::cout << "create socket" << fd_ << std::endl;
}

Socket::~Socket() {
	close(fd_);
	std::cout << "server close..." << std::endl;
}

int Socket::fd() const {
	return fd_;
}

void Socket::setSocketOption() {
	
}
//设置socket选项
//int  setsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen);
void Socket::setReuseAddr() {
	int on = 1;
	setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	//设置地址重用
}

//设置非阻塞
void Socket::setNonblocking() {
	int opts = fcntl(fd_, F_GETFL);
	if (opts < 0) {
		perror("fcntl(fd_, F_GETFL");
		exit(1);
	}
	if (fcntl(fd_, F_SETFL, opts | O_NONBLOCK) < 0) {
		perror("fcntl(fd_, SETFL, opts");
		exit(1);
	}
	std::cout << "server setnonblocking..." << std::endl;
}

//ip地址绑定socket       	服务端客户端都需要绑定
//int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
bool Socket::bindAddress(int serverport) {
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));		//先清零
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(_ServerIP.c_str());	任何ip地址都可以连接
	serveraddr.sin_port = htons(serverport);		//将主机字节顺序转为网络字节顺序
	int ret = bind(fd_, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (ret == -1) {
		close(fd_);
		perror("error bind");
		exit(1);
	}
	std::cout << "server bind address..." << std::endl;
	return true;
}

//listen函数在一般在调用bind之后-调用accept之前调用
//int listen(int sockfd, int backlog)		监听端口
bool Socket::listenSocket() {
	int ret = listen(fd_, Socket::MAXCONNECTION);
	if (ret == -1) {
		perror("error listen");
		close(fd_);
		exit(1);
	}
	std::cout << "server listenning..." << std::endl;
	return true;
}

//接收到新连接,返回这个新连接的socketfd
//int accept(int sockfd, struct sockaddr* addr, socklen_t* len)		
int Socket::acceptSocket(struct sockaddr_in & clientaddr) {
	socklen_t length = sizeof(clientaddr);
	int clientfd = accept(fd_, (struct sockaddr*)&clientaddr, &length);
	//非阻塞的,立即返回
	if (clientfd < 0) {
		//EAGAIN 不是一种错误,它表明当前没有接收到新的连接或fifo中没有新的数据可读
		if (errno == EAGAIN) {
			return 0;
		}
		return clientfd;
	}
	return clientfd;
}

//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
int Socket::connectSocket(const std::string& ip, int& port) {
	int ret = 0;
	Address addr(ip, port);
	int res =  connect(fd_, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	/*
	if (ret < 0) {
		//连接立即返回，此时errno若不是EINPROGRESS，表明错误
		if (errno != EINPROGRESS) {
			std::cout << "connect error != EINPROGRESS" << std::endl;
			close(fd_);
			exit(1);		
		}
	}
	std::cout << "connect to server success" << std::endl;
	return true;
	*/
	
	if (0 == res) {
		std::cout << "socket connect succeed immediately." << std::endl;
		ret = 0;
	} else {
		std::cout << "errno:" << errno << std::endl;
		if (errno == EINPROGRESS) {
			int times = 0;
			while (times++ < 5) {
				fd_set rfds, wfds;
                struct timeval tv;
                
                FD_ZERO(&rfds);
                FD_ZERO(&wfds);
                FD_SET(fd_, &rfds);
                FD_SET(fd_, &wfds);
				tv.tv_sec = 10; 
                tv.tv_usec = 0;
                int selres = select(fd_ + 1, &rfds, &wfds, NULL, &tv);		//观察socketfd的可读可写状态
				switch (selres) {
					case -1:
						std::cout << "select error" << std::endl;
                        ret = -1;
                        break;
					case 0:
                        std::cout << "select time out" << std::endl;
                        ret = -1;
                        break;
					default:
						if (FD_ISSET(fd_, &rfds) || FD_ISSET(fd_, &wfds)) {
							connect(fd_, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
							int err = errno;
							if  (err == EISCONN) {
                                std::cout << "connect finished 111." << std::endl;
                                ret = 0;
                            } else {
								std::cout << "connect failed. errno" << errno << std::endl;
                                ret = errno;
                            }
						}
						
				}

				if (-1 != selres && (ret != 0))
                {
                    std::cout << "check connect result again..." << times << std::endl;
                    continue;
                } else {
					break;
				}
			}
		} else {
			std::cout << "connect to host " << ip << ":" << port << " failed" << std::endl;
			ret = errno;
		}
	
	}
	return ret;

	
}


bool Socket::closeSocket() {
	close(fd_);
	std::cout << "server close..." << std::endl;
	return true;
}


const int Socket::MAXCONNECTION = 8192;







