#include "ContactTransfer.h"


std::ostream& operator<<(std::ostream& os, const ContactTransfer& contact) {
	const std::map<std::string, PersonStruct>& map = contact.getPersonMap();
	for (std::map<std::string, PersonStruct>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
		std::cout << it->second << std::endl;
	}
}

ContactTransfer::ContactTransfer(const std::string& filename, const std::string& option)
										: Contacts(filename), FileOperator(filename) {

}
ContactTransfer::ContactTransfer(const ContactTransfer & c) : Contacts(), FileOperator() {
	
}

ContactTransfer::~ContactTransfer() {

}

void ContactTransfer::sendContactsByTcp(const TcpConnectionPtr & conn) {
	this->serializeToOstream();
	this->sendByTcp(conn);

}



int ContactTransfer::receiveFromTcp(const TcpConnectionPtr& conn, const std::string& message) {
	
	//如接收到complete字样,作为文件传输结束标志,可以写入文件了
	if (message.compare(0, 8, "complete") == 0) {
		onReceiveComplete(conn);
		return 0;
	}
	int nbytes = 0;
	
	
	if (pFile_) {
		std::string tmp;
		tmp.assign(message);	//assign 是拷贝操作,不同于 swap只是交换绑定地址不涉及数据拷贝
		std::cout << "receive " << tmp.size() << " bytes" << std::endl;
		while (tmp.size()) {
			std::string str;
			if (tmp.size() >= MAX_BUFFER_LENGTH) {
				str = tmp.substr(0, MAX_BUFFER_LENGTH);
			} else {
				str = tmp.substr(0, tmp.size());
			}
			nbytes += writeToFile(str, pFile_.get());
			tmp.erase(0, str.size());
			
		}
		
		
		if (nbytes == -1) {
			return -1;
		} else if (nbytes == 0) {
			std::cout << "receive no message." << std::endl;
		} else {
			std::cout << "receive " << nbytes << " bytes" << std::endl;
			
		}
		return nbytes;
	} else {
		std::cout << "fopen file failed" << std::endl;
		return -1;
	}
}

void ContactTransfer::onReceiveComplete(const TcpConnectionPtr & conn) {
	//接受文件完成后,将文件内容反序列化，然后执行回调,回调可以定制自己的需求，比如打印反序列化后的contactbook

	//反序列化,并读取personMap
	this->readAllPeople();
	//std::cout << *this << std::endl;
	
	//执行被父类的回调函数
	isReceived_ = true;
	this->FileOperator::onReceiveComplete(conn);	//要求contacttransfer 对象 set了onReceived_ （绑定了）

	
}




