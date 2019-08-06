#include "PetMysqlOperator.h"
#include "PetProtocMessCodec.h"
#include "pet.pb.h"
#include "../Timer.h"
#include <unistd.h>
#include <functional>
#include <time.h>
#include <unistd.h>

void printAll(boost::any& para) {
	PetMysqlOperator pet(boost::any_cast<PetMysqlOperator&>(para));
	std::cout << pet << std::endl;
}

int main() {
	EventLoop* loop = new EventLoop();
	int port = 8032;
	PetProtocMessCodec server(loop, port, 2);
	PetMysqlOperator mysqlOperator("root", "123456", "exercisesql1", "pet");
	//当bind的对象是重载函数时，需要显示的说明，如下
	// 强转为此函数指针 (int(PetMysqlOperator::*)(PetMysqlOperator::RowVector & rowContent))
	server.setMessageCallback(std::bind((int(PetMysqlOperator::*)(PetMysqlOperator::RowVector & rowContent)) &PetMysqlOperator::insertRow, &mysqlOperator, std::placeholders::_1));
	Timer timer(loop, 10000);	//这个定时器10s打印一次,输出当前数据库 pet表中所有数据
	timer.addOrUpdateTimerCallbackPara("printAllContent", std::bind(&printAll, std::placeholders::_1));
	timer.setCallbackPara(mysqlOperator);
	timer.start();

	server.start();
	loop->loop();

	while (true) {
	
	}

	

}

