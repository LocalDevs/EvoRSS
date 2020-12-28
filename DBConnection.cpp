#include "DBConnection.h"
#include "Callbacks.h"
#include <sqlite3.h>
#include <utility>



DBConnection* DBConnection::theCnx = nullptr;

//Ctor & Dtor
DBConnection::DBConnection(std::string path, char* zErrMsg) : _dbPath(std::move(path))
{
	// Save the connection result
	int result = sqlite3_open(_dbPath.c_str(), &_theDb);

	if (result)
		zErrMsg = const_cast<char*>(sqlite3_errmsg(_theDb));
	else
		zErrMsg = nullptr;
}
DBConnection::~DBConnection()
{
	closeConnection();

	delete theCnx;
	theCnx = nullptr;
}

DBConnection* DBConnection::GetConnection(char* zErrMsg)
{
	if (theCnx == nullptr)
	{

		//TODO : add a log
	}

	return theCnx;
}

//TODO: Chnage the name to SetConnection
DBConnection* DBConnection::GetConnection(std::string dbPath, char* zErrMsg)
{
	if (theCnx == nullptr)
	{
		theCnx = new DBConnection(std::move(dbPath), zErrMsg);
	}

	return theCnx;
}

bool DBConnection::executeQuery(const std::string sql, sqlite3_callback callback, char*& zErrMsg)
{
	const auto resultCommand = sqlite3_exec(_theDb, sql.c_str(), callback, 0, &zErrMsg);

	return resultCommand == 0;
}

//TODO: to review with US-7

//bool DBConnection::insertQuery(const std::string insertSql, DBObjetct params)
//{
//	sqlite3_stmt* pStmt = nullptr;
//
//	auto iReturn = sqlite3_prepare_v2(this->_theDb, insertSql.c_str(), static_cast<int> (insertSql.size()) + 1, &pStmt, nullptr);
//	if (iReturn != SQLITE_OK) {
//		//TODO : Should I really close it or just logg the error 
//		sqlite3_close(this->_theDb);
//		return SQLITE_ERROR;
//	}
//
//	//auto nbrParams = sqlite3_bind_parameter_count(pStmt);
//	bool _error = false;
//	
//	for (auto itr = params.elements.begin(); _error != false, itr != params.elements.end(); ++itr)
//	{
//	
//		switch (itr->first)
//		{
//		case FieldType::_STRING:
//			iReturn = sqlite3_bind_text(pStmt, 
//										static_cast<int>(std::distance(params.elements.begin(), itr)) + 1, 
//										std::any_cast<std::string> (itr->second).c_str(),
//										std::any_cast<std::string> (itr->second).size(), 
//										SQLITE_TRANSIENT);
//			if (iReturn != SQLITE_OK) {
//				_error = true;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//
//	if (!_error)
//	{
//		if (sqlite3_step(pStmt) != SQLITE_DONE) {
//			sqlite3_finalize(pStmt);
//			sqlite3_close(this->_theDb);
//			_error = true;
//		}
//	}
//
//	sqlite3_reset(pStmt);
//	sqlite3_clear_bindings(pStmt);
//	
//	return false == _error;
//}


void DBConnection::closeConnection()
{
	// Close the connection
	if (_theDb)
		sqlite3_close(_theDb);

	_theDb = nullptr;
}

