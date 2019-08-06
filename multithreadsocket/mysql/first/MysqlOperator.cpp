#include "MysqlOperator.h"

std::ostream& operator<< ( std::ostream& os, MysqlOperator& my) {
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

MysqlOperator::MysqlOperator(const std::string& user, const std::string& password, 
				const std::string& dataBase, const std::string& table, const std::string& serverAddr) : conn_(NULL), 
				tableName_(table), fieldVector_(), rowCount_(0), fieldCount_(0), allDataMap_() {
	
	
	conn_ = mysql_init(NULL);
	mysql_real_connect(conn_, serverAddr.c_str(), user.c_str(), password.c_str(), dataBase.c_str(), 0, NULL, 0);
	
}
MysqlOperator::~MysqlOperator() {
	mysql_close(conn_);
}

void MysqlOperator::synchronizeTable() {
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

int MysqlOperator::insertRow(RowVector& rowContent) {
	
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

int MysqlOperator::insertRow(const std::string& name, const std::string& owner, const std::string& species,
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

int MysqlOperator::deleteRow(const std::string& key) {
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

int MysqlOperator::updateRow(const std::string& key, const std::string& keyValue, const std::string& modifyField, const std::string& value) {
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
