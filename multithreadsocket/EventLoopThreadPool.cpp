
#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop * mainloop, int threadnum)
	: mainloop_(mainloop), threadnum_(threadnum), threadlist_(), index_(0) {

	//初始化IO线程池
	for (int i = 0; i < threadnum_; ++i) {
		EventLoopThread* pLoopThread = new EventLoopThread();
		threadlist_.push_back(pLoopThread);
	}
}

EventLoopThreadPool::~EventLoopThreadPool() {
	std::cout << "Clean up the EventLoopThreadPool" << std::endl;
	for (int i = 0; i < threadnum_; ++i) {
		delete threadlist_[i];
	}
	threadlist_.clear();
}


void EventLoopThreadPool::start() {
	if (threadnum_ > 0) {
		//std::cout << "EventLoopThreadPool line 25: start(), threadnum_: " << threadnum_ << std::endl;
		for (auto i : threadlist_) {
			i->startThread();	//i直接取出list中的内容,而不是类似begin()的指针
			//std::cout << "eventloopthread *p" << i << std::endl;
		}
	}
}


EventLoop* EventLoopThreadPool::getNextLoop() {
	if (threadnum_ > 0) {
		EventLoop* p = threadlist_[index_]->getLoop();
		//std::cout << "eventloopthread *p" << threadlist_[index_] << std::endl;
		index_ = (index_ + 1) % threadnum_;		//轮询
		return p;
		//LC策略，还没写
		
	} else {
		return mainloop_;
	}
	
}







