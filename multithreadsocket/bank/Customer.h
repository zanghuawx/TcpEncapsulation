#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>

class Customer {
public:
	Customer(const std::string& name, const int& requestTime, const long long& arriveTime, const int& delayTime = 10)
			: name_(name), requestTime_(requestTime), consumeTime_(), arriveTime_(arriveTime),
			leaveTime_(), remainTime_(), queueUpId_(), delayTime_(delayTime) {
		
	}
	Customer(const std::string& name)
		: name_(name), requestTime_(), consumeTime_(), arriveTime_(),
		leaveTime_(), remainTime_(), queueUpId_(), delayTime_() {
	}
	~Customer() {
	}
	std::string getName() const {
		return name_;
	}
	void setRequestTime(const int& req) {
		requestTime_ = req;
	}
	int getRequestTime() const {
		return requestTime_;
	}
	void setConsumeTime(const long long& consume) {
		consumeTime_ = consume;
	}
	long long getConsumeTime() const {
		return consumeTime_;
	}
	void setArriveTime(const long long& arrive) {
		arriveTime_ = arrive;
	}
	long long getArriveTime() const {
		return arriveTime_;
	}
	void setLeaveTime(const long long& leave) {
		leaveTime_ = leave;
	}
	long long getLeaveTime() const {
		return leaveTime_;
	}
	void setRemainTime(const int& remain) {
		remainTime_ = remain;
	}
	int getRemainTime() const {
		return remainTime_;
	}
	void setIdentify(const std::string& id) {
		queueUpId_ = id;
	}
	std::string getIdentify() const {
		return queueUpId_;
	}
	void setDelayTime(const int& delay) {
		delayTime_ = delay;
	}
	int getDelayTime() const {
		return delayTime_;
	}
	
private:
	std::string name_;
	int requestTime_;		//办理业务所需要的时间，秒
	long long consumeTime_;		//从进银行到离开银行，总共耗时多少秒
	long long arriveTime_;	//到达银行的时间，秒
	long long leaveTime_;	//离开银行的时间，秒
	int remainTime_;		//办业务剩余的时间，如果还在排队，那当然=需求时间
	std::string queueUpId_;	//在大堂经理排队，排到后，会得到一个排队号,四个窗口分别为A****,B****,C****,D****
	int delayTime_;			//上一位客户办完业务后，叫号到柜台办理会有一点延时0-60s，随机的

};


#endif // !_CUSTOMER_H_


