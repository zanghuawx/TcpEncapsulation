
//Channel类，fd和事件的封装

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

/*

std::function对象是对C++中现有的可调用实体的一种类型安全的包裹
（我们知道像函数指针这类可调用实体，是类型不安全的）
template <class T>
T g_Minus(T i, T j) {
    return i - j;
}

function<int(int, int)> f = g_Minus<int>;
*/

#include <functional>
#include <stdint.h>

class Channel {
public:
	//回调函数类型
	typedef std::function<void()> Callback;
	Channel();
	~Channel();

	//设置文件描述符
	void setFd(int fd) {
		fd_ = fd;
	}
	//获取文件描述符
	int getFd() const {
		return fd_;
	}
	//设置触发事件
	void setEvents(uint32_t events) {
		events_ = events;
	}
	//获取触发事件
	uint32_t getEvents() const {
		return events_;
	}
	//事件分发处理
	void handleEvent();
	//设置读事件回调
	void setReadHandle(const Callback& cb) {
		readhandler_ = cb;
		//readhandler_ = std::move(cb);	//提供效率可以使用move语义,这里暂不使用
	}
	//设置写事件的回调
	void setWriteHandle(const Callback& cb) {
		writehandler_ = cb;
	}
	//设置错误事件回调
	void setErrorHandle(const Callback& cb) {
		errorhandler_ = cb;
	}
	//设置close事件回调
	void setCloseHandle(const Callback& cb) {
		closehandler_ = cb;
	}
private:
	int fd_;
	uint32_t events_;		//事件，一般情况下为epoll events 
	Callback readhandler_;	
	Callback writehandler_;
	Callback errorhandler_;
	Callback closehandler_;
};




#endif


