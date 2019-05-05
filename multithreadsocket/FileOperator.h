
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
	~FileOperator();
	void selectFile(const std::string& filename, const std::string& option = WriteCreateMode);
	bool open(const std::string& option);
	int writeToFile(const std::string& buffer, FILE* file);
	int readFromFile(std::string& buffer, FILE* file);
	void close(const TcpConnectionPtr & conn);
	int sendByTcp(const TcpConnectionPtr& conn);
	void onWriteComplete(const TcpConnectionPtr& conn);

	void setSended(const Callback& cb) {
		onSended_ = cb;
	}
	
	static const std::string WriteCreateMode;
	static const int MAX_BUFFER_LENGTH;
private:
	FILE* pFile_;
	std::string filename_;
	std::string option_;
	Callback onSended_;
};


#endif


