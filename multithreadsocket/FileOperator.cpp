#include "FileOperator.h"
#include "Any.h"

#include <sys/sendfile.h>
#include <memory.h>
#include <iostream>
#include <cassert>
#include <unistd.h>






FileOperator::FileOperator() :       filename_(), option_(), onSended_(NULL), onReceived_(NULL), isReceived_(false), isSended(false) {
	open();
}

FileOperator::FileOperator(const std :: string & filename, const std :: string & option)
							:  filename_(filename), option_(option), onSended_(NULL), onReceived_(NULL), isReceived_(false), isSended(false) {
	open();
}



FileOperator::~FileOperator() {

}

void FileOperator::selectFile(const std::string& filename, const std::string& option) {
	filename_ = filename;
	option_ = option;
}



bool FileOperator::open(const std :: string & option) {
	//如果智能指针中保存一个空指针，才去打开文件
	if (!pFile_) {
		FILE* file = fopen(filename_.c_str(), option_.c_str());
		FilePtr sp(file, fclose);
		pFile_ = sp;
		//std::cout << pFile_.use_count() << std::endl;
	}
	
	
	return true;
}

//向文件写入内容
int FileOperator::writeToFile(const std :: string & buffer, FILE* file) {
	int writeBytes = fwrite(buffer.c_str(), sizeof(char), buffer.size(), file);
	if (writeBytes < 0) {
		std::cout << "fwrite error" << std::endl;
		return -1;
	} else if (writeBytes == 0) {
		std::cout << "no data write" << std::endl;
		return 0;
	} else {
		return writeBytes;
	}
}

//从文件读出内容
int FileOperator::readFromFile(std :: string & buffer, FILE* file) {
	char ptr[MAX_BUFFER_LENGTH];
	memset(ptr, 0, MAX_BUFFER_LENGTH);
	int readBytes = fread(ptr, sizeof(char), MAX_BUFFER_LENGTH, file);
	if (readBytes < 0) {
		std::cout << "fread error" << std::endl;
		return -1;
	} else if (readBytes == 0) {
		std::cout << "read no data" << std::endl;
		return 0;
	} else {
		buffer = ptr;
		return readBytes;
	}
}

void FileOperator::close(const TcpConnectionPtr & conn) {
	if (pFile_) {
		fclose(pFile_.get());
	}
}

int FileOperator::sendByTcp(const TcpConnectionPtr & conn) {
	int nbytes = 1;
	std::string str;
	if (pFile_) {
		isSended = false;
		nbytes = readFromFile(str, pFile_.get());
		if (nbytes == -1) {
			return -1;
		}else if (nbytes == 0) {
			std::cout << "send file complete." << std::endl;
		}
		else {
			conn->sendString(str);
			std::cout << "read file " << nbytes << " and sent by tcp" << std::endl;
		}
		return nbytes;
	} else {
		std::cout << "fopen file failed" << std::endl;
		return -1;
	}
	
}

int FileOperator::receiveFromTcp(const TcpConnectionPtr& conn, const std::string& message) {
	int nbytes = 0;
	
	if (pFile_) {
		std::string tmp;
		tmp.assign(message);	//assign 是拷贝操作,不同于 swap只是交换绑定地址不涉及数据拷贝
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
			if (isReceived_) {
				onReceiveComplete(conn);
			}
			
		}
		return nbytes;
	} else {
		std::cout << "fopen file failed" << std::endl;
		return -1;
	}
}



void FileOperator::onWriteComplete(const TcpConnectionPtr& conn) {
	std::string ss("woshishui");
	any i_a = 3;
	int i_b = any_cast<int>(i_a);
	std::string ssb = any_cast<std::string>(ss);
	//std::cout << "test i_b: " << i_b << "ssb: " << ssb << std::endl;

	any dds = std::make_shared<std::string>("nishishui");
	std::shared_ptr<std::string> dd = any_cast<std::shared_ptr<std::string>>(dds);
	//std::cout << "test dd: " << *(dd.get()) << std::endl;


	int nbytes = 0;
	std::string str;

	nbytes = readFromFile(str, pFile_.get());
	
	if (nbytes == -1) {
		std::cout << "send file failed." << std::endl;
	} else if (nbytes == 0) {
		std::cout << "send file complete." << std::endl;
		isSended = true;
		if (onSended_ != NULL) {
			onSended_(conn);
		}
		
	}
	else {
		if (!isSended) {
			conn->sendString(str);
			std::cout << "read file " << nbytes << " and sent by tcp" << std::endl;
		}
		
	}
}


void FileOperator::onReceiveComplete(const TcpConnectionPtr & conn) {
	if (onReceived_ != NULL) {
		onReceived_(conn);
	}
	
}


const std::string FileOperator::WriteCreateMode = "a+b";
const int FileOperator::MAX_BUFFER_LENGTH = 1024;




