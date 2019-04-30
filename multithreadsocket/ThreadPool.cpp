//Author: wuxiao

#include "ThreadPool.h"
#include <deque>
#include <unistd.h>


ThreadPool::ThreadPool(int threadnum) : started_(false), threadnum_(threadnum), threadPtrSet_(), 
										taskQueue_(), mutex_(), condition_() {

}

//shared_ptr 的引用计数概念:  有多少个shared_ptr对象指向相同德 new出来的内存空间.
// 如  shared_ptr<string> a = make_shared<string>(new string("hello"));	shared_ptr<string> b(a);	则 a和b都引用了 new string("hello")这个内存空间
//因此它们 a和b的引用计数都为 2
//make_shared<string>(new string("hello"))  返回的是shared_ptr类,它管理着 new string("hello")
ThreadPool::~ThreadPool() {
	//遍历列表,join等待子线程结束
	for (ThreadPtrSet::iterator it = threadPtrSet_.begin(); it != threadPtrSet_.end(); ++it) {
		(*it)->join();
	}

	//使用了智能指针,ThreadPool对象析构,成员变量 threadPtrSet_<shared_ptr>链表也会一一析构shared_ptr对象,解了引用就会自动delete thread指针
	/*
	for (ThreadPtrSet::iterator it = threadPtrSet_.begin(); it != threadPtrSet_.end(); ++it) {
		delete (*t);
	}
	*/
}

void ThreadPool::start() {
	if (threadnum_ > 0) {
		started_ = true;
		for (int i = 0; i < threadnum_; i++) {
			//ThreadPtr pThread = std::make_shared<std::thread>(&ThreadPool::threadFunc, this);
			threadPtrSet_.insert(std::make_shared<std::thread>(&ThreadPool::threadFunc, this));		//直接将make_shared返回的shared_ptr插入平衡二叉树
		}
	}
}

void ThreadPool::stop() {
	started_ = false;
	condition_.notify_all();	//唤醒所有线程
}


void ThreadPool::addTask(Task task) {
	//非静态代码块
	{
		std::lock_guard<std::mutex> lock(mutex_);
		taskQueue_.push(task);
	}
	condition_.notify_one();	//依次唤醒等待队列的线程
}

void ThreadPool::threadFunc() {
	std::thread::id tid = std::this_thread::get_id();
	std::stringstream sin;
	sin << tid;
	std::cout << "worker thread is running :" << tid << std::endl;
	Task task;
	while (started_) {

		task = NULL;
		{
			std::unique_lock<std::mutex> lock(mutex_);
			//如果任务列表空了,则设置条件等待,等待其它线程插入了任务,再通知本线程继续执行
			while (taskQueue_.empty() && started_) {
				condition_.wait(lock);		//等到了其它线程唤醒信号发出(也就是插入了任务到列表)
			}
			//如果其它线程(一般是工作线程)停止了线程池,就直接退出不再执行了
			if (!started_) {
				break;
			}
			//才能继续把任务取出来执行
			task = taskQueue_.front();		//链表中的第一个任务赋值
			taskQueue_.pop();				//将第一个任务取出来,剩下的就依次推前一位
		}
		//接下来就执行插入的任务,一般都是loop
		if (task) {
			try {
				task();
			} catch (std::bad_alloc& ba) {
				std::cerr << "bad_alloc caught in ThreadPool::ThreadFunc task: " << ba.what() << '\n';
                while(1);
			}
		}
	}
	
}





