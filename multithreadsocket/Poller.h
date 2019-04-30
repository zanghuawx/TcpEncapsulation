// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//Poller类，对epoll的封装

/*
//epoll系统调用:

//创建一个epoll实例,返回这个epoll实例的fd,
//size是这个epoll所要处理的大致事件数目
int epoll_create(int size);

//向 epoll对象中添加、修改或者删除感兴趣的事件
//epfd为epoll对象标识
//op: EPOLL_CTL_ADD添加事件 EPOLL_CTL_ADD修改 EPOLL_CTL_ADD删除
//fd:待监测的连接套接字标识
//event: 先看结构体
struct epoll_event{
__uint32_t events;
epoll_data_t data;
};

int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);

//收集在 epoll监控的事件中已经发生的事件，如果 epoll中没有任何一个事件发生，
//则最多等待timeout毫秒后返回。epoll_wait的返回值表示当前发生的事件个数
int epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout);


*/

#ifndef _POLLER_H_
#define _POLLER_H_

#include <vector>
#include <mutex>
#include <map>
#include <sys/epoll.h>
#include "Channel.h"

class Poller {
private:
	//事件指针数组类型
	typedef std::vector<Channel*> ChannelPtrList;
	typedef std::vector<struct epoll_event> EventList;
public:
	//epoll实例
	int pollfd_;

	//events数组，用于传递给epollwait
	EventList eventlist_;

	//事件表红黑树
	std::map<int, Channel*> channelmap_;

	//保护事件map的互斥量
	std::mutex mutex_;

	Poller();
	~Poller();

	//等待事件，epoll_wait封装
	//int epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout);
	void pollWait(ChannelPtrList& activechannellist);

	//添加事件，EPOLL_CTL_ADD
	void addChannel(Channel* pchannel);

	//移除事件，EPOLL_CTL_DEL
	void removeChannel(Channel* pchannel);

	//修改事件，EPOLL_CTL_MOD
	void updateChannel(Channel* pchannel);


};


#endif


