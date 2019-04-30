
#ifndef _TESTCLIENTMULTI_H_
#define _TESTCLIENTMULTI_H_

#include "ThreadPool.h"
#include "TcpServer.h"
#include <time.h>
#include <unistd.h>


class TestClientMulti {
public:
	TestClientMulti(int threadnum = 2000);
	~TestClientMulti();

	void init();
	void start();

	
private:
	
	

	std::vector<TaskList*> taskList_;
	int threadnum_;
	ThreadPool pool;
	
};

#endif




