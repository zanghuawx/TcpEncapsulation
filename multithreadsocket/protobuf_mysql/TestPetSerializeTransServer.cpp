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
	//��bind�Ķ��������غ���ʱ����Ҫ��ʾ��˵��������
	// ǿתΪ�˺���ָ�� (int(PetMysqlOperator::*)(PetMysqlOperator::RowVector & rowContent))
	server.setMessageCallback(std::bind((int(PetMysqlOperator::*)(PetMysqlOperator::RowVector & rowContent)) &PetMysqlOperator::insertRow, &mysqlOperator, std::placeholders::_1));
	Timer timer(loop, 10000);	//�����ʱ��10s��ӡһ��,�����ǰ���ݿ� pet������������
	timer.addOrUpdateTimerCallbackPara("printAllContent", std::bind(&printAll, std::placeholders::_1));
	timer.setCallbackPara(mysqlOperator);
	timer.start();

	server.start();
	loop->loop();

	while (true) {
	
	}

	

}

