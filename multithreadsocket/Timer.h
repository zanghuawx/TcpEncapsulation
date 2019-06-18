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
������������һ����ʱ��������timerfd
��һ��������clockidָ��ʱ�����ͣ�������ֵ��
CLOCK_REALTIME :Systemwide realtime clock. ϵͳ��Χ�ڵ�ʵʱʱ��
CLOCK_MONOTONIC:�Թ̶����������У��Ӳ����е����͸�λ ,�������κ�ϵͳtime-of-dayʱ���޸ĵ�Ӱ��
�ڶ���������flags������0����O_CLOEXEC/O_NONBLOCK��
����ֵ��timerfd���ļ���������

struct timespec {
	time_t tv_sec;          //Seconds    
	long   tv_nsec;			//Nanoseconds 
};
struct itimerspec {
	struct timespec it_interval;  //Interval for periodic timer 
	struct timespec it_value;		// Initial expiration
};

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
���ã�����������ر���fdָ���Ķ�ʱ��
������
fd��timerfd����timerfd_create��������
fnew_value:ָ���µĳ�ʱʱ�䣬�趨new_value.it_value������������ʱ��������رն�ʱ�������new_value.it_intervalΪ0����ʱ��ֻ��ʱһ�Σ�����ʼ�ǴΣ�����֮��ÿ���趨ʱ�䳬ʱһ��
old_value����Ϊnull���򷵻ض�ʱ���������֮ǰ�ĳ�ʱʱ��
flags��1�������õ��Ǿ���ʱ�䣻Ϊ0�������ʱ�䡣
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
	//��д����
	std::string bufferIn_;
	std::string bufferOut_;

	bool running_;


};


#endif
