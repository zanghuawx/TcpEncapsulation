#ifndef _COUNTERCLERK_H_
#define _COUNTERCLERK_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <memory>

#include "Customer.h"
#include "../Timer.h"

class CounterClerk {
public:
	CounterClerk(const std::string& id, const int& full = 20)
				: counterId_(id), delayTimerPtrQueue_(), transactionTimerPtrQueue_(), queueCustomerPtrQueue_(),
				isIdle_(true), isFull_(false), maxCustomerQueue_(full), transactionFinishCount_(0) {
	}
	~CounterClerk() {
	}
	void setIdle(bool idle) {
		isIdle_ = idle;
	}
	bool isIdle() const {
		return isIdle_;
	}
	void setIsFull(bool full) {
		isFull_ = full;
	}
	bool isFull() const {
		return isFull_;
	}
	
	

private:
	std::string counterId_;		//柜台编号 A,B,C,D等
	std::queue<std::shared_ptr<Timer>> delayTimerPtrQueue_;		//排队办理业务的人，每人被叫号迟疑时间不同，所以弄个定时器队列
	std::queue<std::shared_ptr<Timer>> transactionTimerPtrQueue_;	//排队办理业务的人，每人的业务时间都不同，弄个定时器队列
	std::queue<std::shared_ptr<Customer>> queueCustomerPtrQueue_;	//排队办理业务的人 队列，包括正在办理业务的人(队列最前面)
	bool isIdle_;		//true 表示正在叫号迟疑阶段，闲着的
	bool isFull_;		//true 表示排队+办理的人数已经满了，不能再往窗口队列推入客户
	int maxCustomerQueue_;	//本窗口接受排队+办理的最大人数
	int transactionFinishCount_;	//本窗口已经办理结束业务的人数统计


};

#endif // !_COUNTERCLERK_H_

