// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//IO复用流程的抽象，等待事件，处理事件，执行其他任务
#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <iostream>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include "Poller.h"
#include "Channel.h"

class EventLoop {
public:
	//任务类型
	typedef std::function<void()> Functor;
	//事件列表类型
	typedef std::vector<Channel*> ChannelPtrList;

	EventLoop();
	~EventLoop();
	//执行事件循环
	void loop();
	//添加事件
	void addChannelToPoller(Channel* pchannel) {
		//std::cout << "EventLoop.h line 30: addChannelToPoller()" << std::endl;
		poller_.addChannel(pchannel);
	}
	//移除事件
	void removeChannelFromPoller(Channel* pchannel) {
		poller_.removeChannel(pchannel);
	}
	//修改事件
	void updateChannelToPoller(Channel* pchannel) {
		poller_.updateChannel(pchannel);
	}
	//退出事件循环
	void quitLoop() {
		quit_ = true;
	}
	//获取loop所在线程id
	std::thread::id getThreadId() const {
		return tid_;
	}
	//唤醒loop
	void wakeUp();
	//唤醒loop后的读回调
	void handleRead();
	//唤醒loop后的错误处理回调
	void handleError();
	void writeHandle();
	void closeHandle();
	//向任务队列添加任务
	void addTask(Functor func) {
		//std::cout << "EventLoop.h line 57: addTask()" << std::endl;
		//推入任务列表
		if (true) {
			std::lock_guard<std::mutex> lock(mutex_);
			functorList_.push_back(func);
		}
		wakeUp();	//跨线程唤醒,worker线程唤醒IO线程
	}
	//执行任务队列的任务
	void executeTask();

private:
	Poller poller_;	//epoll操作封装
	bool quit_;	//运行状态
	std::thread::id tid_;	//loop所在的线程id
	std::vector<Functor> functorList_;	//任务列表
	std::mutex mutex_;
	ChannelPtrList activeChannelList_;	//活跃事件
	int wakeupfd_;	//跨线程唤醒fd
	Channel wakeupChannel_;	//用于唤醒当前loop的事件
	
};



#endif



