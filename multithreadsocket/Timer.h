#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdio.h>
#include <sys/epoll.h>
#include <time.h>
#include <sys/timerfd.h>
#include <functional>
#include <mutex>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <map>

#include "Channel.h"
#include "EventLoop.h"
/*
int timerfd_create(int clockid, int flags);
它是用来创建一个定时器描述符timerfd
第一个参数：clockid指定时间类型，有两个值：
CLOCK_REALTIME :Systemwide realtime clock. 系统范围内的实时时钟
CLOCK_MONOTONIC:以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响
第二个参数：flags可以是0或者O_CLOEXEC/O_NONBLOCK。
返回值：timerfd（文件描述符）

struct timespec {
	time_t tv_sec;          //Seconds    
	long   tv_nsec;			//Nanoseconds 
};
struct itimerspec {
	struct timespec it_interval;  //Interval for periodic timer 
	struct timespec it_value;		// Initial expiration
};

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
作用：用来启动或关闭有fd指定的定时器
参数：
fd：timerfd，有timerfd_create函数返回
fnew_value:指定新的超时时间，设定new_value.it_value非零则启动定时器，否则关闭定时器，如果new_value.it_interval为0，则定时器只定时一次，即初始那次，否则之后每隔设定时间超时一次
old_value：不为null，则返回定时器这次设置之前的超时时间
flags：1代表设置的是绝对时间；为0代表相对时间。
*/

enum TimerMode {
	SINGLE = 0,
	CYCLE,
	TIMER_MODE_MAX
};


class Timer : public std::enable_shared_from_this<Timer> {
public:
	typedef std::function<void()> Callback;
	Timer(EventLoop* loop, const int& ms, const TimerMode& mode = CYCLE);
	~Timer();
	void initTimerfd();
	void start();

	void stop();
	void closeTimerfd();


	void handleRead();
	void handleWrite();
	void handleError();
	void handleClose();

	void setReadCallback(const Callback& cb) {
		readCallback_ = cb;
	}
	void setWriteCallback(const Callback& cb) {
		writeCallback_ = cb;
	}

	void setErrorCallback(const Callback& cb) {
		errorCallback_ = cb;
	}

	void setCloseCallback(const Callback& cb) {
		closeCallback_ = cb;
	}

	void addOrUpdateTimerCallback(const std::string& key, const Callback& cb);
	void removeTimerCallback(const std::string& key);
	void removeAllTimerCallback();

	const bool& isRunning() const {
		return running_;
	}



private:
	Callback readCallback_;
	Callback writeCallback_;
	Callback errorCallback_;
	Callback closeCallback_;

	int timerfd_;
	std::mutex mutex_;
	std::unique_ptr<Channel> spChannel_;
	int ms_;
	int mode_;
	EventLoop* loop_;
	long count_;
	std::map<std::string, Callback> timerCallbackMap_;
	//读写缓冲
	std::string bufferIn_;
	std::string bufferOut_;

	bool running_;


};


#endif
