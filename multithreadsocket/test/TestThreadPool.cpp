
#include "TestThreadPool.h"

TestThreadPool::TestThreadPool(int threadnum) : taskList_(), threadnum_(threadnum), pool(threadnum) {
	init();
}

TestThreadPool::~TestThreadPool() {
	for (auto it : taskList_) {
		delete it;
	}
}

void TestThreadPool::init() {
	for (int i = 0; i < threadnum_; ++i) {
		TaskList* task = new TaskList();
		task->setTaskid(i);
		taskList_.push_back(task);
	}
}

void TestThreadPool::start() {
	for (auto it : taskList_) {
		pool.addTask(std::bind(&TaskList::func, it));
		
	}
	pool.start();
}


int main() {
	TestThreadPool test(20);
	test.start();

	while(1) {

	}
}



