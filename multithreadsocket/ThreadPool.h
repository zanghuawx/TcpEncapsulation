/*
Author: wuxiao

//ThreadPool类，简易线程池实现，表示worker线程,执行通用任务线程
//
// 使用的同步原语有 
// pthread_mutex_t mutex_l;//互斥锁
// pthread_cond_t condtion_l;//条件变量
// 使用的系统调用有
// pthread_mutex_init();	//互斥量
// pthread_cond_init();		//条件变量初始化
// pthread_create(&thread_[i],NULL,threadFunc,this)	//创建线程
// pthread_mutex_lock()		//上锁
// pthread_mutex_unlock()	//解锁
// pthread_cond_signal()	//条件变量信号
// pthread_cond_wait()		//条件变量阻塞
// pthread_cond_broadcast();	//条件变量广播
// pthread_join()				//等待子线程执行结束
// pthread_mutex_destory()		//销毁互斥量
// pthread_cond_destory()		//销毁条件变量
*/


#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <mutex>  
#include <thread> 
#include <condition_variable>
#include <functional>
#include <memory>
#include <set>


class ThreadPool {
public:
	typedef std::shared_ptr<std::thread> ThreadPtr;	//使用智能指针,当对象析构,会自动回收引用的 new出的内存
	typedef std::set<ThreadPtr> ThreadPtrSet;		//使用set 实现红黑树的平衡二叉检索树, set中只有key而没有value, key只能是唯一,适合指针集合
	
	//线程池任务类型
	typedef std::function<void()> Task;
	//构造函数
	ThreadPool(int threadnum = 0);
	~ThreadPool();
	//启动线程池
	void start();
	//暂停线程池
	void stop();
	//添加任务
	void addTask(Task task);
	//线程池执行的函数
	void threadFunc();
	//获取线程数量
	int getThreadNum() const {
		return threadnum_;
	}
private:
	//运行状态
	bool started_;
	//线程数量
	int threadnum_;
	//线程列表
	ThreadPtrSet threadPtrSet_;
	//任务队列
	std::queue<Task> taskQueue_;		//std::queue  只能在头extract, 只能在尾push
	//任务队列互斥量
	std::mutex mutex_;
	//任务队列同步的条件变量
	std::condition_variable condition_;
	
};




#endif


