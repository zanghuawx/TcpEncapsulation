// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//服务器socket类，封装socket描述符及相关的初始化操作

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>


/*
系统调用 int socket(int domain, int type, int protocol); 创建一个套接字
domain:AF_INET ipv4    type:SOCKET_STREAM tcp连接流套接字  protocol: 0就好了


//系统调用:
//设置socket选项
//int  setsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen);
//ip地址绑定socket       	服务端客户端都需要绑定
//int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

//listen函数在一般在调用bind之后-调用accept之前调用
//int listen(int sockfd, int backlog)		监听端口


typedef uint32_t in_addr_t;
struct in_addr {
	in_addr_t s_addr;       //32位IPV4地址
};

struct sockaddr_in {  
    short            sin_family;       // 2 bytes e.g. AF_INET, AF_INET6  
    unsigned short   sin_port;    // 2 bytes e.g. htons(3490)  
    struct in_addr   sin_addr;     // 4 bytes see struct in_addr, below  
    char             sin_zero[8];     // 8 bytes zero this if you want to  
}; 

struct socket   
{   
    socket_state              state;   
    unsigned long             flags;   
    const struct proto_ops    *ops;   
    struct fasync_struct      *fasync_list;   
    struct file               *file;   
    struct sock               *sk;   
    wait_queue_head_t         wait;   
    short                     type;   
};
//接收到新连接,返回这个新连接的socketfd
int accept(int sockfd, struct sockaddr* addr, socklen_t* len)		

//连接函数,一般客户端会调用这个
//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

//发送报文到指定的socketfd
//int send( SOCKET s,const char* buf,int len,int flags);

//接收指定socketfd的报文
//int recv( SOCKET s,char* buf,int len,int flags);


fcntl可实现对指定文件描述符的各种操作
int fcntl(int fd, int cmd, ... );
cmd:
F_DUPFD：复制文件描述符
F_DUPFD_CLOEXEC：复制文件描述符，新文件描述符被设置了close-on-exec
F_GETFD：读取文件描述标识
F_SETFD：设置文件描述标识
F_GETFL：读取文件状态标识
F_SETFL：设置文件状态标识
F_GETLK：如果已经被加锁，返回该锁的数据结构。如果没有被加锁，将l_type设置为F_UNLCK
F_SETLK：给文件加上进程锁
F_SETLKW：给文件加上进程锁，如果此文件之前已经被加了锁，则一直等待锁被释放。

...:
输入的命令,可以是各种类型的数据



*/



class Socket {
private:
	int fd_;
	static const int MAXCONNECTION;
public:
	Socket();
	~Socket();
	//获取fd
	int fd() const;
	//socket设置
	void setSocketOption();
	//设置地址重用
	void setReuseAddr();
	//设置非阻塞
	void setNonblocking();
	//绑定端口
	bool bindAddress(int serverport);
	//开始监听
	bool listenSocket();
	//accept 获取客户端连接
	int acceptSocket(struct sockaddr_in& clientaddr);
	//客户端连接服务端
	int connectSocket(const std::string& ip, int& port);
	//关闭服务器
	bool closeSocket();
	
};

#endif



