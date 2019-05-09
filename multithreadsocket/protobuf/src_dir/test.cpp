#include "example.pb.h"
#include <fstream>
#include <iostream>
using namespace std;

int main() {
	/*
	ptest::person p;
	p.set_name("zhangsan");
	p.set_id(110);
	p.set_email("zhangsan@163");
	
	fstream output("out.log", ios::out | ios::binary);
	if (!p.SerializeToOstream(&output)) {
		cerr << "Failed to write msg" << endl;
		return -1;
	}
	*/
	
	fstream input("out.log", ios::in | ios::binary);
	ptest::person per;
	per.ParseFromIstream(&input);
	cout << "name: " << per.name() << endl;
	cout << "email: " << per.email() << endl;
	
	return 0;
	
}