
#include "CounterClerk.h"


CounterClerk::CounterClerk(EventLoop* loop, const std::string& id, const int* timeBase, const int& full, const long long& onDutyTime, const long long& stopServiceTime)
	: loop_(loop), counterId_(id), timeBase_(timeBase), delayTimerPtrQueue_(), transactionTimerPtrQueue_(), queueCustomerPtrQueue_(),
	isIdle_(true), maxCustomerQueue_(full), transactionFinishCount_(0), currentServiceCustomerName_("--"),
	onDutyTime_(onDutyTime), offDutyTime_(), stopServiceTime_(stopServiceTime), dutyTimer_(loop, 1000), transactionTimerCallback_(), isDuty_(false) {

	dutyTimer_.addOrUpdateTimerCallback(id, std::bind(&dutyTask, this));	//将工作任务添加到定时器回调中,每一秒钟巡视一下工作情况
}

void CounterClerk::onDelayTimeOut() {
	isIdle_ = false;
	currentServiceCustomerName_ = queueCustomerPtrQueue_.front()->getName();
	//先判断是否是上班第一个客户的延时到了
	if (transactionFinishCount_ > 0) {
		transactionTimerPtrQueue_.pop();	//把上一个客户的业务定时器删掉
	}
	transactionTimerPtrQueue_.front()->start();		//启动当前客户的业务定时器
}

void CounterClerk::onTransactionTimeOut() {
	isIdle_ = true;
	currentServiceCustomerName_ = "--";
	++transactionFinishCount_;
	queueCustomerPtrQueue_.front()->setLeaveTime(*timeBase_);		//标记离开时间
	//此客户耗时=离开时间-到达时间
	queueCustomerPtrQueue_.front()->setConsumeTime(queueCustomerPtrQueue_.front()->getLeaveTime() - queueCustomerPtrQueue_.front()->getArriveTime());
	std::cout << queueCustomerPtrQueue_.front()->getIdentify() << " consume time: " << queueCustomerPtrQueue_.front()->getConsumeTime() << std::endl;
	queueCustomerPtrQueue_.pop();		//请走当前的客户,第二个客户把当前客户挤出去了
	delayTimerPtrQueue_.pop();			//销毁当前客户的延时定时器，第二个延时定时器把当前定时器挤出去了
	//办完了今天最后一位客户的业务，下班打卡，啊哈哈哈哈哈嗝~
	if (queueCustomerPtrQueue_.size() == 0) {
		isIdle_ = false;		//下班了，当然是窗口忙啊
		isDuty_ = false;
		offDutyTime_ = *timeBase_;
		int hour = offDutyTime_ / 3600;
		int minute = (offDutyTime_ % 3600) / 60;

		std::cout << counterId_ << " counter clerk " << "下班！啊哈哈哈哈嗝~~" << std::endl;
		std::cout << hour << "点" << minute << "分才下班，好惨啊~~~" << std::endl;
		transactionTimerPtrQueue_.pop();		//因为下班了,要把最后一个业务定时器删掉
	} else {
		delayTimerPtrQueue_.front()->start();		//启动即将办业务的客户的延时定时器
		std::cout << "please " << queueCustomerPtrQueue_.front()->getIdentify() << "come to " << counterId_ << " counter transaction" << std::endl;
	}
	transactionTimerCallback_();				//回调，在这里面，执行一个累加大堂办完业务客户总数的统计，需要是线程安全的,因为四个窗口有可能同时要累加
}

void CounterClerk::dutyTask() {
	//工作任务就疯狂打印吧，耗时间就对了
	//当前客户业务剩余时间递减
	if (isDuty_ == false) {
		std::cout << counterId_ << "柜台下班了！" << std::endl;
		return;
	}
	queueCustomerPtrQueue_.front()->setRemainTime(queueCustomerPtrQueue_.front()->getRemainTime() - 1);
	int minute = (queueCustomerPtrQueue_.front()->getRemainTime() % 3600) / 60;
	std::cout << counterId_ << "柜台：" << queueCustomerPtrQueue_.front()->getIdentify() << "正在办理业务，" << "还剩" << minute << "分钟" << std::endl;
}

int CounterClerk::addCustomerToQueue(CustomerPtr& cus) {
	//判断是否已经上下班了
	if (*timeBase_ < onDutyTime_ && *timeBase_ > stopServiceTime_) {
		return -1;
	}
	if (isDuty_ == false) {
		isDuty_ = true;
	}
	//判断当前是否已经满了
	if (isFull()) {
		return -1;
	}
	
	queueCustomerPtrQueue_.push(cus);	//客户来到柜台排队
	std::shared_ptr<Timer> delayTimer(new Timer(loop_, cus->getDelayTime() * 1000, TimerMode::SINGLE));	
	delayTimer->addOrUpdateTimerCallback(cus->getName() + " delay", std::bind(&onDelayTimeOut, this));
	delayTimerPtrQueue_.push(delayTimer);		//维护一个客户需要延迟的定时器

	std::shared_ptr<Timer> transactionTimer(new Timer(loop_, cus->getRequestTime() * 1000, TimerMode::SINGLE));
	transactionTimer->addOrUpdateTimerCallback(cus->getName() + " transaction", std::bind(&onTransactionTimeOut, this));
	transactionTimerPtrQueue_.push(transactionTimer);	//维护一个客户办业务需要的时间的定时器

	//如果工作嘀嗒还没启动,就说明counter clerk还没开始干活,所以要启动
	if (!dutyTimer_.isRunning) {
		//初次启动工作定时器,也就是刚上班，需要叫号，客户延时计时开始
		dutyTimer_.start();
		delayTimerPtrQueue_.front()->start();
	}

}
