// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//EventLoopThread类，表示IO线程,执行特定任务的,线程池的是通用任务线程

#ifndef _EVENTLOOP_THREAD_H_
#define _EVENTLOOP_THREAD_H_

#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>
#include "EventLoop.h"


class EventLoopThread {
public:
	EventLoopThread();
	~EventLoopThread();
	EventLoop* getLoop() const;
	//启动线程
	void startThread();
	//线程执行的函数
	void thredFunc();
private:
	std::thread th_;		//线程成员
	std::thread::id threadid_;
	std::string threadname_;
	EventLoop* loop_;		//线程运行的线程循环
};


#endif



