#include "MysqlOperator.h"

int main(int argc, char** argv) {
	MysqlOperator sqlOperator("root", "123456", "exercisesql1", "pet");
	sqlOperator.synchronizeTable();
	std::cout << sqlOperator;
	sqlOperator.insertRow("lejingjing", "sunyanxia", "monkey", "m", "2011-12-01", "2099-05-06");
	sqlOperator.insertRow("beibei", "zhuzhaoyi", "orangutan", "f", "2012-12-02", "2099-06-07");
	sqlOperator.insertRow("huanghuang", "sunyanxia", "bear", "m", "2013-10-02", "2098-12-06");
	sqlOperator.insertRow("jingjing", "xiazhi", "koala", "m", "2018-03-10", "2110-03-06");
	sqlOperator.insertRow("yingying", "gaoyu", "dog", "f", "2019-02-05", "2087-01-03");
	sqlOperator.synchronizeTable();
	std::cout << sqlOperator;
	sqlOperator.deleteRow("lejingjing");
	sqlOperator.updateRow("name", "huanghuang", "owner", "daihuxuan");
	sqlOperator.synchronizeTable();
	std::cout << sqlOperator;
	exit(0);
}
