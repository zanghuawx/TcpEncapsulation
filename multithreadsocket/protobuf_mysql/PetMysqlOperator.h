#ifndef _PetMysqlOperator_H_
#define _PetMysqlOperator_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <mysql/mysql.h>
#include <stdlib.h>

class PetMysqlOperator {
public:
	typedef std::vector<std::string> RowVector;
	PetMysqlOperator(const std::string& user, const std::string& password, 
				const std::string& dataBase, const std::string& table, const std::string& serverAddr = "localhost");
	~PetMysqlOperator();
	
	void synchronizeDownloadTable();
	int synchronizeUploadTable();
	int insertRow(RowVector& rowContent);
	int insertRow(const std::string& name, const std::string& owner, const std::string& species,
		const std::string& sex, const std::string& birth, const std::string& death = "NULL");
	int deleteRow(const std::string& key);
	int updateRow(const std::string& key, const std::string& keyValue, const std::string& modifyField, const std::string& value);
	
	void addDataToMap(const std::string& key, RowVector& rowData);
	int queryRowByKey(const std::string& key, RowVector& rowData);
	
	friend std::ostream& operator<< ( std::ostream& os, PetMysqlOperator& my);
	
private:
	MYSQL* conn_;
	std::string tableName_;
	std::vector<std::string> fieldVector_;
	int rowCount_;
	int fieldCount_;
	std::map<std::string, std::vector<std::string>> allDataMap_;
	
	
};

#endif
