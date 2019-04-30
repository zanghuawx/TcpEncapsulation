
#ifndef _TESTTHREADPOOL_H_
#define _TESTTHREADPOOL_H_

#include "ThreadPool.h"
#include <time.h>
#include <unistd.h>


class TestThreadPool {
public:
	TestThreadPool(int threadnum = 20);
	~TestThreadPool();

	void init();
	void start();

	
private:
	class TaskList {
	public:
		TaskList() {
			
		}
		void func() {
			
			while (1) {
				std::cout << "task id: " << taskid_ << std::endl;
				usleep(500000);
			}
		}
		void setTaskid(const int& id) {
			taskid_ = id;
		}
	private:
		int taskid_;
	};
	

	std::vector<TaskList*> taskList_;
	int threadnum_;
	ThreadPool pool;
	
};

#endif




