#include "PetMysqlOperator.h"

std::ostream& operator<< ( std::ostream& os, PetMysqlOperator& my) {
	os << "fields: ";
	for (std::vector<std::string>::iterator it = my.fieldVector_.begin(); it != my.fieldVector_.end(); ++it) {
		os << *it << " ";
	}
	os << "\t";
	for (auto it : my.allDataMap_) {
		for (auto content : it.second) {
			os << content << " ";
		}
		os << "\t";
	}
	os << std::endl;
	return os;
}

PetMysqlOperator::PetMysqlOperator(const std::string& user, const std::string& password, 
				const std::string& dataBase, const std::string& table, const std::string& serverAddr) : conn_(NULL), 
				tableName_(table), fieldVector_(), rowCount_(0), fieldCount_(0), allDataMap_() {
	
	
	conn_ = mysql_init(NULL);
	mysql_real_connect(conn_, serverAddr.c_str(), user.c_str(), password.c_str(), dataBase.c_str(), 0, NULL, 0);
	
}
PetMysqlOperator::~PetMysqlOperator() {
	mysql_close(conn_);
}

void PetMysqlOperator::synchronizeDownloadTable() {
	std::string obtainIdentifyTable("select * from ");
	obtainIdentifyTable.append(tableName_);
	MYSQL_RES* res = NULL;
	mysql_query(conn_, obtainIdentifyTable.c_str());
	res = mysql_store_result(conn_);		//obtain the content of pet table
	
	rowCount_ = mysql_num_rows(res);		//obtain total number rows of table
	fieldCount_ = mysql_num_fields(res);	//obtain total number fields of table
	
	MYSQL_FIELD* field = NULL;
	fieldVector_.clear();
	for (int i = 0; i < fieldCount_; ++i) {
		field = mysql_fetch_field_direct(res, i);
		fieldVector_.push_back(field->name);
	}
	
	MYSQL_ROW row = NULL;
	allDataMap_.clear();
	row = mysql_fetch_row(res);
	while (NULL != row) {
		std::string key = row[0];
		for (int i = 0; i < fieldCount_; ++i) {
			allDataMap_[key].push_back(row[i]);
		}
		row = mysql_fetch_row(res);
	}
	mysql_free_result(res);
}

int PetMysqlOperator::synchronizeUploadTable() {
	std::string clearTableStr("truncate table tablename");
	int res = mysql_query(conn_, clearTableStr.c_str());
	if (res) {
		std::cout << "clear table failed!" << std::endl;
		return -1;
	}
	//delay
	for (int i = 0; i < 32767; ++i) {
		for (int j = 0; j < 32767; ++j) {
			
		}
	}
	for (auto it : allDataMap_) {
		insertRow(it.second);
	}
	return 0;
}

int PetMysqlOperator::insertRow(RowVector& rowContent) {
	
	if (rowContent.size() < 6) {
		return -1;
	}
	std::string insertStr;
	insertStr.append("insert into ");
	insertStr.append(tableName_);
	insertStr.append(" values('");
	insertStr.append(rowContent[0]);
	insertStr.append("', '");
	insertStr.append(rowContent[1]);
	insertStr.append("', '");
	insertStr.append(rowContent[2]);
	insertStr.append("', '");
	insertStr.append(rowContent[3]);
	insertStr.append("', '");
	insertStr.append(rowContent[4]);
	insertStr.append("', '");
	insertStr.append(rowContent[5]);
	insertStr.append("')");
	int res = mysql_query(conn_, insertStr.c_str());
	if (res) {
		return -1;
	}
	return 0;
}

int PetMysqlOperator::insertRow(const std::string& name, const std::string& owner, const std::string& species,
		const std::string& sex, const std::string& birth, const std::string& death) {
	
	std::string insertStr;
	insertStr.append("insert into ");
	insertStr.append(tableName_);
	insertStr.append(" values('");
	insertStr.append(name);
	insertStr.append("', '");
	insertStr.append(owner);
	insertStr.append("', '");
	insertStr.append(species);
	insertStr.append("', '");
	insertStr.append(sex);
	insertStr.append("', '");
	insertStr.append(birth);
	insertStr.append("', '");
	insertStr.append(death);
	insertStr.append("')");
	int res = mysql_query(conn_, insertStr.c_str());
	if (res) {
		std::cout << "insert row failed!" << std::endl;
		return -1;
	}
	return 0;
	
}

int PetMysqlOperator::deleteRow(const std::string& key) {
	std::string deleteStr;
	deleteStr.append("delete from pet where name = '");
	deleteStr.append(key);
	deleteStr.append("'");
	int res = mysql_query(conn_, deleteStr.c_str());
	if (res) {
		std::cout << "delete row failed!" << std::endl;
		return -1;
	}
	return 0;
}

int PetMysqlOperator::updateRow(const std::string& key, const std::string& keyValue, const std::string& modifyField, const std::string& value) {
	std::string updateStr;
	updateStr.append("update ");
	updateStr.append(tableName_);
	updateStr.append(" set ");
	updateStr.append(modifyField);
	updateStr.append(" = '");
	updateStr.append(value);
	updateStr.append("' where ");
	updateStr.append(key);
	updateStr.append(" = ");
	updateStr.append("'");
	updateStr.append(keyValue);
	updateStr.append("'");
	
	int res = mysql_query(conn_, updateStr.c_str());
	if (res) {
		std::cout << "update row failed!" << std::endl;
		return -1;
	}
	return 0;
}

void PetMysqlOperator::addDataToMap(const std::string& key, RowVector& rowData) {
	allDataMap_[key] = rowData;
}

int PetMysqlOperator::queryRowByKey(const std::string& key, RowVector& rowData) {
	std::string obtainIdentifyTable("select * from ");
	obtainIdentifyTable.append(tableName_);
	obtainIdentifyTable.append(" where name = '");
	obtainIdentifyTable.append(key);
	obtainIdentifyTable.append("'");
	
	MYSQL_RES* res = NULL;
	mysql_query(conn_, obtainIdentifyTable.c_str());
	res = mysql_store_result(conn_);		//obtain the content of pet table
	
	rowCount_ = mysql_num_rows(res);		//obtain total number rows of table
	fieldCount_ = mysql_num_fields(res);	//obtain total number fields of table
	
	MYSQL_ROW row = NULL;

	row = mysql_fetch_row(res);
	if (NULL != row) {
		return -1;
	}
	for (int i = 0; i < fieldCount_; ++i) {
		rowData.push_back(row[i]);
	}
	mysql_free_result(res);
	return 0;
}



