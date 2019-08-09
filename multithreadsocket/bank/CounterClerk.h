
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
#include "../EventLoop.h"

class CounterClerk {
public:
	typedef std::shared_ptr<Customer> CustomerPtr;
	typedef std::function<void()> Callback;
	//例如 A柜台, 时基是大堂共用的，排队人数最大20，上班时间9点，停止服务时间17点,下班时间就看最后一个人了。
	CounterClerk(EventLoop* loop, const std::string& id, const int* timeBase, const int& full = 20, const long long& onDutyTime = 32400, const long long& stopServiceTime = 61200);
	~CounterClerk() {
	}
	std::string counterId() const {
		return counterId_;
	}
	void setIdle(bool idle) {
		isIdle_ = idle;
	}
	bool isIdle() const {
		return isIdle_;
	}
	bool isFull() const {
		return queueCustomerPtrQueue_.size() >= maxCustomerQueue_;
	}
	int getTransactionFinishCount() const {
		return transactionFinishCount_;
	}
	std::string getCurrentServiceCustomerName() const {
		return currentServiceCustomerName_;
	}
	long long getOnDutyTime() const {
		return onDutyTime_;
	}
	long long getOffDutyTime() const {
		return offDutyTime_;
	}
	long long getStopServiceTime_() const {
		return stopServiceTime_;
	}
	bool isDuty() const {
		return isDuty_;
	}

	void setTransactionTimerCb(Callback& cb) {
		transactionTimerCallback_ = cb;
	}
	void onDelayTimeOut();	//客户延时结束回调
	void onTransactionTimeOut();	//当前客户办完业务回调
	void dutyTask();
	//往窗口安排人，如果返回-1表示失败，持有本类对象的对象需要做别的安排
	int addCustomerToQueue(CustomerPtr& cus);

	
	

private:
	EventLoop* loop_;			//线程
	std::string counterId_;		//柜台编号 A,B,C,D等
	const int* timeBase_;				//需要一个时基，整个项目只有银行有一个时基，取地址赋给每个柜台的这个指针，也就是多个指针指向同一个地址，因为它只读不写
	std::queue<std::shared_ptr<Timer>> delayTimerPtrQueue_;		//排队办理业务的人，每人被叫号迟疑时间不同，所以弄个定时器队列
	std::queue<std::shared_ptr<Timer>> transactionTimerPtrQueue_;	//排队办理业务的人，每人的业务时间都不同，弄个定时器队列
	std::queue<std::shared_ptr<Customer>> queueCustomerPtrQueue_;	//排队办理业务的人 队列，包括正在办理业务的人(队列最前面)
	bool isIdle_;		//true 表示正在叫号迟疑阶段，闲着的
	int maxCustomerQueue_;	//本窗口接受排队+办理的最大人数
	int transactionFinishCount_;	//本窗口已经办理结束业务的人数统计
	std::string currentServiceCustomerName_;	//当前服务的客户名字
	long long onDutyTime_;						//上班时间
	long long offDutyTime_;						//下班时间
	long long stopServiceTime_;					//停止服务时间
	Timer dutyTimer_;							//工作定时器,上班了就启动,如果下班了，就停止掉
	Callback transactionTimerCallback_;			//回调接口
	bool isDuty_;								//是否在上班

};

#endif // !_COUNTERCLERK_H_


