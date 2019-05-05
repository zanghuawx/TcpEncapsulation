

#include "EventLoop.h"
#include <iostream>
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>

int createEventfd() {
	//创建一个对象,这个对象能被用户空间应用用作一个事件等待/响应机制
	//主要用于线程间或进程间跨线程/进程的操作
	int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
		std::cout << "Failed in eventfd" << std::endl;
        exit(1);
	}
	return evtfd;
}


EventLoop::EventLoop() : poller_(), quit_(true), tid_(std::this_thread::get_id()),
						functorList_(), mutex_(), activeChannelList_(), 
						wakeupfd_(createEventfd()), wakeupChannel_() {

	//事件的一系列绑定或设置操作
	//将事件添加到epoll中
	wakeupChannel_.setFd(wakeupfd_);
	wakeupChannel_.setReadHandle(std::bind(&EventLoop::handleRead, this));
	wakeupChannel_.setErrorHandle(std::bind(&EventLoop::handleError, this));
	wakeupChannel_.setWriteHandle(std::bind(&EventLoop::writeHandle, this));
	wakeupChannel_.setCloseHandle(std::bind(&EventLoop::closeHandle, this));
	wakeupChannel_.setEvents(EPOLLIN | EPOLLET);
	poller_.addChannel(&wakeupChannel_);		//随便给这个events fd写什么内容都会触发epoll事件
}

EventLoop::~EventLoop() {
	close(wakeupfd_);	//关闭这个eventfd对象
}

//ssize_t write(int filedes, void *buf, size_t nbytes);
// 返回：若成功则返回写入的字节数，若出错则返回-1
// filedes：文件描述符
// buf:待写入数据缓存区
// nbytes:要写入的字节数
void EventLoop::wakeUp() {
	std::cout << "EventLoop.cpp line 46 wakeUp()" << std::endl;
	uint64_t one = 1;
	//象这个eventfd对象写入内容,随便写入什么都会触发epoll(EPOLLIN | EPOLLPRI)事件
	//然后自动回调 readhandler_, 也就是绑定的EventLoop::handleRead()
	ssize_t ret = write(wakeupfd_, (char*)(&one), sizeof one);
	if (ret == -1) {
		
	}
}

//ssize_t read(int filedes, void *buf, size_t nbytes);
// 返回：若成功则返回读到的字节数，若已到文件末尾则返回0，若出错则返回-1
// filedes：文件描述符
// buf:读取数据缓存区
// nbytes:要读取的字节数
void EventLoop::handleRead() {
	std::cout << "EventLoop.cpp line 62 handleRead()" << std::endl;
	uint64_t one = 1;
	ssize_t ret = read(wakeupfd_, &one, sizeof(one));
	if (ret == -1) {
		
	}
}

void EventLoop::handleError() {

}

void EventLoop::loop() {
	//运行状态在运行
	//若在运行就loop
	//从epoll中pollwait活跃的事件
	//遍历并处理事件
	//清除活跃的事件列表
	//执行任务
	quit_ = false;
	while (!quit_) {
		//test
		//std::cout << "EventLoop::loop()" << std::endl;
		//将所有就绪的事件从内核事件表中复制到vector<struct epoll_event>中,转换为封装的channel对象后,插入到活跃事件列表中
		poller_.pollWait(activeChannelList_);
		std::cout << "EventLoop::loop() : poller_.pollWait(activeChannelList_) end" << std::endl;
		for (Channel* pchannel : activeChannelList_) {
			std::cout << "iterator pchannel start" << std::endl;
			pchannel->handleEvent();
			std::cout << "iterator pchannel end" << std::endl;
		}
		activeChannelList_.clear();
		//std::cout << "will executeTask()..." << std::endl;
		executeTask();
	}
	
}

void EventLoop::writeHandle() {

}
void EventLoop::closeHandle() {
	
}



void EventLoop::executeTask() {
	//新建列表,并交换活跃事件列表元素,要上锁
	//遍历交换后的列表,执行
	//清除列表
	std::vector<Functor> functionList;
	std::cout << "EventLoop.cpp line 103: executeTask()" << std::endl;
	if (true) {
		std::lock_guard<std::mutex> lock(mutex_);
		functionList.swap(functorList_);	//交换元素
	}
	for (Functor& func : functionList) {
		func();
	}
	functionList.clear();
}






