

#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include "Timer.h"

static int readTimerfd(int& fd, std::string& bufferin) {

	int nbyte = 0;
	char buf[128];
	bufferin.clear();
	nbyte = read(fd, buf, sizeof(buf));
	if (nbyte != sizeof(uint64_t)) {
		std::cout << "read timer fd failed" << std::endl;
		return -1;
	}
	bufferin.append(buf, nbyte);
	return nbyte;
}

static int writeTimerfd(int& fd, std::string& bufferin) {
	return 0;
}



Timer::Timer(EventLoop* loop, const int& ms, const TimerMode& mode) : timerfd_(), mutex_(),
			spChannel_(new Channel()), ms_(ms), mode_(mode), loop_(loop), count_(0), timerCallbackMap_(),
			bufferIn_(), bufferOut_(), running_(false) {

	initTimerfd();
}

Timer::~Timer() {
	loop_->removeChannelFromPoller(spChannel_.get());
	close(timerfd_);
	closeCallback_();
}


void Timer::initTimerfd() {
	timerfd_ = timerfd_create(CLOCK_MONOTONIC, 0);
	if (timerfd_ < 0) {
		std::cout << "Failed in create timerfd" << std::endl;
		exit(1);
	}

	spChannel_->setFd(timerfd_);
	spChannel_->setReadHandle(std::bind(&Timer::handleRead, this));
	spChannel_->setWriteHandle(std::bind(&Timer::handleWrite, this));
	spChannel_->setErrorHandle(std::bind(&Timer::handleError, this));
	spChannel_->setCloseHandle(std::bind(&Timer::handleClose, this));
	spChannel_->setEvents(EPOLLIN | EPOLLET);
	


}

void Timer::start() {
	std::lock_guard<std::mutex> lock(mutex_);
	if (running_ == false) {
		loop_->addTask(std::bind(&EventLoop::addChannelToPoller, loop_, spChannel_.get()));

		itimerspec it;
		it.it_interval.tv_sec = (time_t)ms_ / 1000;
		it.it_interval.tv_nsec = (long)(((long)ms_ % 1000) * 1000 * 1000);
		it.it_value.tv_sec = (time_t)ms_ / 1000;
		it.it_value.tv_nsec = (long)(((long)ms_ % 1000) * 1000 * 1000);
		//std::cout << "sec: " << it.it_interval.tv_sec << " nsec: " << it.it_interval.tv_nsec << std::endl;
		//Æô¶¯¶¨Ê±Æ÷
		int ret = timerfd_settime(timerfd_, 0, &it, NULL);
		if (ret < 0) {
			std::cout << "Failed in timerfd_settime" << std::endl;
			exit(1);
		}
		running_ = true;
		std::cout << "timer start..." << std::endl;
	}
}

void Timer::stop() {
	std::lock_guard<std::mutex> lock(mutex_);
	if (running_ == true) {
		count_ = 0;
		loop_->addTask(std::bind(&EventLoop::removeChannelFromPoller, loop_, spChannel_.get()));
		running_ = false;
		std::cout << "timer stop..." << std::endl;
	}
	

}

void Timer::closeTimerfd() {
	stop();
	close(timerfd_);
	std::cout << "timer stop" << std::endl;
	closeCallback_();
}

void Timer::handleRead() {
	int ret = readTimerfd(timerfd_, bufferIn_);
	if (ret != -1) {
		++count_;
		//std::cout << "timer execute number of times: " << count_  << std::endl;
		/*
		if (readCallback_ != NULL) {
			readCallback_();
		}
		*/
		for (std::map<std::string, Callback>::iterator it = timerCallbackMap_.begin();
			it != timerCallbackMap_.end(); ++it) {
			it->second();
		}
	}
	if (mode_ == SINGLE) {
		if (count_) {
			std::cout << "timer execute single and stop" << std::endl;
			stop();
		}
	}
}

void Timer::handleWrite() {
	if (writeCallback_ != NULL) {
		writeCallback_();
	}
}

void Timer::handleError() {
	if (errorCallback_ != NULL) {
		errorCallback_();
	}
}

void Timer::handleClose() {
	if (closeCallback_ != NULL) {
		closeCallback_();
	}
}

void Timer::addOrUpdateTimerCallback(const std::string& key, const Callback& cb) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (cb != NULL && key.size()) {
		//timerCallbackMap_.insert_or_assign(key, cb);	//c++17
		timerCallbackMap_[key] = cb;
	}
}

void Timer::removeTimerCallback(const std::string& key) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (timerCallbackMap_.find(key) != timerCallbackMap_.end()) {
		timerCallbackMap_.erase(key);
	}
}

void Timer::removeAllTimerCallback() {
	std::lock_guard<std::mutex> lock(mutex_);
	timerCallbackMap_.clear();
}


