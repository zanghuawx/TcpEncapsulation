#include "FileOperator.h"
#include <sys/sendfile.h>
#include <memory.h>
#include <iostream>
#include <cassert>
#include "Any.h"





FileOperator::FileOperator() : pFile_(), filename_(), option_() {
	
}

FileOperator::FileOperator(const std :: string & filename, const std :: string & option)
							: pFile_(), filename_(filename), option_(option) {
	
}



FileOperator::~FileOperator() {
	if (pFile_ != NULL) {
		fclose(pFile_);
	}
}

void FileOperator::selectFile(const std::string& filename, const std::string& option) {
	filename_ = filename;
	option_ = option;
}



bool FileOperator::open(const std :: string & option) {
	
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
	if (pFile_ != NULL) {
		fclose(pFile_);
	}
}

int FileOperator::sendByTcp(const TcpConnectionPtr & conn) {
	int nbytes = 1;
	std::string str;
	pFile_ = fopen(filename_.c_str(), option_.c_str());
	if (pFile_) {
		
		nbytes = readFromFile(str, pFile_);
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


void FileOperator::onWriteComplete(const TcpConnectionPtr& conn) {
	std::string ss("woshishui");
	any i_a = 3;
	int i_b = any_cast<int>(i_a);
	std::string ssb = any_cast<std::string>(ss);
	std::cout << "test i_b: " << i_b << "ssb: " << ssb << std::endl;

	any dds = std::make_shared<std::string>("nishishui");
	std::shared_ptr<std::string> dd = any_cast<std::shared_ptr<std::string>>(dds);
	std::cout << "test dd: " << *(dd.get()) << std::endl;

	
	FILE* file = fopen(filename_.c_str(), option_.c_str());
	FilePtr ass(file, fclose);
	std::string cont("woheni");
	FileStruct as = {ass, "haha", cont};
	//any aa = as;
	//FileStruct aas = any_cast<FileStruct>(aa);
	//std::cout << "test aas: " << aas.name << std::endl;
	std::string dis = any_cast<std::string>(as.context);
	std::cout << "test dis: " << dis << std::endl;

	int nbytes = 0;
	std::string str;

	nbytes = readFromFile(str, pFile_);
	
	if (nbytes == -1) {
		std::cout << "send file failed." << std::endl;
	}else if (nbytes == 0) {
		std::cout << "send file complete." << std::endl;
		onSended_(conn);
	}
	else {
		conn->sendString(str);
		std::cout << "read file " << nbytes << " and sent by tcp" << std::endl;
	}
}


const std::string FileOperator::WriteCreateMode = "a+b";
const int FileOperator::MAX_BUFFER_LENGTH = 1024;




