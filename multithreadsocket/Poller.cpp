
/*
typedef union epoll_data {
  void	  *ptr;
  int	   fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t	   events;	  // Epoll events 
  epoll_data_t data;	  // User data variable 
};
*/


#include "Poller.h"
#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <errno.h>

#define EVENTNUM 4096 //最大触发事件数量
#define TIMEOUT 1000 //epoll_wait 超时时间设置


Poller::Poller() : pollfd_(-1), eventlist_(EVENTNUM),
					channelmap_(), mutex_() {
	pollfd_ = epoll_create(256);
	if (pollfd_ == -1) {
		perror("epoll_create1");
        exit(1);
	}
	std::cout << "epoll_create" << pollfd_ << std::endl;
}

Poller::~Poller() {
	close(pollfd_);
}

//收集在 epoll监控的事件中已经发生的事件，如果 epoll中没有任何一个事件发生，
//则最多等待timeout毫秒后返回。epoll_wait的返回值表示当前发生的事件个数
//int epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout);
//等待I/O事件
void Poller::pollWait(ChannelPtrList & activechannellist) {
	//timeout 时间
	int timeout = TIMEOUT;	//1000ms
	//系统调用等待		&*eventlist_.begin() 一定要先去值再取地址，不然会强转失败
	int nfds = epoll_wait(pollfd_, &*eventlist_.begin(), (int)eventlist_.capacity(), timeout);
	//std::cout << "Poller.cpp line 51 : epoll_wait() non block" << std::endl;
	//判断返回的 当前发生的事件个数,-1则报错
	if (nfds == -1) {
		perror("epoll wait error");
	}
	//遍历发生的事件
	//取出事件列表的events元素
	//取出事件列表的data.ptr元素,强转channel*
	//获取描述符
	for (int i = 0; i < nfds; ++i) {
	//for (auto it : eventlist_) {
	//for (EventList::iterator it = eventlist_.begin(); it != eventlist_.end(); it++) {
		//std::cout << "Poller.cpp line 61 : it:" << std::endl;
		uint32_t event = eventlist_[i].events;
		Channel* pchannel = (Channel*)eventlist_[i].data.ptr;
		int fd = pchannel->getFd();
		//std::cout << "Poller.cpp line 65 : iterator end" << std::endl;

		std::map<int, Channel*>::const_iterator iter;	//迭代器指向的元素不能改变
		if (true) {
			//用lock_guard类所管理器,传入的mutex会被当前线程锁住,析构时会自动解锁,不需要手动解锁了
			std::lock_guard<std::mutex> lock(mutex_);
			iter = channelmap_.find(fd);	//通过描述符(关键字)寻找红黑数种的Channel*元素
			//if()作用域结束就会自动释放mutex_ 解锁
		}
		if (iter != channelmap_.end()) {
			pchannel->setEvents(event);
			activechannellist.push_back(pchannel);	//从后面插入channel(fd和事件的封装)
			//std::cout << "Poller.cpp line 79: pollwait(). activechannellist.push_back" << std::endl;
		} else {
			std::cout << "not find channel!" << std::endl;
		}
	}
	//如果发生的事件个数等于eventlist_的容量，需要扩容
	if (nfds == (int)eventlist_.capacity()) {
		std::cout << "resize:" << nfds << std::endl;
		eventlist_.resize(nfds * 2);
	}

}


//添加事件
void Poller::addChannel(Channel * pchannel) {
	//std::cout << "Poller.cpp line 95: addChannel()" << std::endl;
	//获取事件描述符fd
	//获取epoll_event.events
	//获取指针
	//上锁添加事件红黑,fd关键字和channel指针
	int fd = pchannel->getFd();
	struct epoll_event event;
	event.events = pchannel->getEvents();
	event.data.ptr = pchannel;
	if (true) {
		std::lock_guard<std::mutex> lock(mutex_);
		channelmap_[fd] = pchannel;
	}
	if (epoll_ctl(pollfd_, EPOLL_CTL_ADD, fd, &event) == -1) {
		perror("epoll add error");
        exit(1);
	}
}

//删除事件
void Poller::removeChannel(Channel * pchannel) {
	//获取事件描述符fd
	//获取epoll_event.events
	//获取指针
	//上锁添加事件红黑,fd关键字和channel指针
	int fd = pchannel->getFd();
	struct epoll_event event;
	event.events = pchannel->getEvents();
	event.data.ptr = pchannel;
	if (true) {
		std::lock_guard<std::mutex> lock(mutex_);
		channelmap_.erase(fd);
	}
	if (epoll_ctl(pollfd_, EPOLL_CTL_DEL, fd, &event) == -1) {
		perror("epoll delete error");
        exit(1);
	}
}

//更新事件
void Poller::updateChannel(Channel * pchannel) {
	int fd = pchannel->getFd();
	struct epoll_event event;
	event.events = pchannel->getEvents();
	event.data.ptr = pchannel;
	if (epoll_ctl(pollfd_, EPOLL_CTL_MOD, fd, &event) == -1) {
		perror("epoll update error");
        exit(1);
	}
}






