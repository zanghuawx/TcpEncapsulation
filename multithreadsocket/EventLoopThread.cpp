
#include "EventLoopThread.h"
#include <iostream>
#include <sstream>

//构造函数在工作线程中执行
EventLoopThread::EventLoopThread() : th_(), threadid_(-1), threadname_("IO thread"), loop_(NULL) {
	
}

//析构函数在工作线程中执行
EventLoopThread::~EventLoopThread() {
	//线程结束时清理
	loop_->quitLoop();
	th_.join();		//等待IO线程退出。清理IO线程，防止内存泄漏，因为pthread_created回calloc
}

EventLoop* EventLoopThread::getLoop() const {
	return loop_;
}

void EventLoopThread::startThread() {
	//std::cout << "EventLoopThread.cpp line 23: startThread()" << std::endl;
	//创建线程      库 构造     explicit thread (Fn&& fn, Args&&... args);
	th_ = std::thread(&EventLoopThread::thredFunc, this);
}

void EventLoopThread::thredFunc() {
	//loop_ = new EventLoop();
	EventLoop loop;
    loop_ = &loop;
	//std::cout << "eventloop *p" << loop_ << std::endl;
	threadid_ = std::this_thread::get_id();
	std::stringstream sin;
	sin << threadid_;
	threadname_ += sin.str();

	//std::cout << "in the thread:" << threadname_ << std::endl;
	try {
		loop_->loop();				//epoll_wait 阻塞等待内核通知
	} catch (std::bad_alloc& ba) {
		std::cerr << "bad_alloc caught in EventLoopThread::ThreadFunc loop: " << ba.what() << '\n';
	}
}









