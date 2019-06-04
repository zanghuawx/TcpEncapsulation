#ifndef _CONTACTTRANSFER_H_
#define _CONTACTTRANSFER_H_
#include "Contacts.h"
#include "FileOperator.h"

//不能直接引用基类的私有成员, 只能通过基类的公有函数间接的操作基类的私有成员
class ContactTransfer : public Contacts, public FileOperator {
public:
	ContactTransfer(const std::string& filename, const std::string& option = FileOperator::WriteCreateMode);
	ContactTransfer(const ContactTransfer& c);
	~ContactTransfer();
	
	void sendContactsByTcp(const TcpConnectionPtr& conn);

	//void receiveContactsFromTcp(const TcpConnectionPtr& conn);	//这个函数父类已经操作了,不需要再写,只需重写父类的onReceiveComplete 虚函数就行
	
	int receiveFromTcp(const TcpConnectionPtr& conn, const std::string& message);
	void onReceiveComplete(const TcpConnectionPtr& conn);
	friend std::ostream& operator<<(std::ostream& os, const ContactTransfer& contact);


};


#endif

