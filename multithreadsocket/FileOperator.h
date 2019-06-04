
#ifndef _FILEOPERATOR_H_
#define _FILEOPERATOR_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <memory>
#include <iostream>
#include <functional>
#include "TcpConnection.h"
#include "Any.h"

class FileOperator {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void (const TcpConnectionPtr&)> Callback;
	typedef std::shared_ptr<FILE> FilePtr;
	typedef struct {
		FilePtr fp;
		std::string name;
		any context;
	}FileStruct;
	FileOperator();
	FileOperator(const std::string& filename, const std::string& option = WriteCreateMode);
	virtual ~FileOperator();
	void selectFile(const std::string& filename, const std::string& option = WriteCreateMode);
	bool open(const std::string& option = WriteCreateMode);
	int writeToFile(const std::string& buffer, FILE* file);
	int readFromFile(std::string& buffer, FILE* file);
	void close(const TcpConnectionPtr & conn);
	int sendByTcp(const TcpConnectionPtr& conn);
	virtual int receiveFromTcp(const TcpConnectionPtr& conn, const std::string& message);
	void onWriteComplete(const TcpConnectionPtr& conn);
	virtual void onReceiveComplete(const TcpConnectionPtr& conn);

	void setSended(const Callback& cb) {
		onSended_ = cb;
	}
	void setReceived(const Callback& cb) {
		onReceived_ = cb;
	}
	
	bool isReceived_;
	bool isSended;
	static const std::string WriteCreateMode;
	static const int MAX_BUFFER_LENGTH;

	std::string& getOption() {
		return option_;
	}

	FilePtr pFile_;
private:
	
	std::string filename_;
	std::string option_;
	Callback onSended_;
	Callback onReceived_;
	
};


#endif


