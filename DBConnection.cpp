#include "DBConnection.h"
#include "Callbacks.h"
#include <sqlite3.h>
#include <utility>
#include <QString>
#include <QtSql/QSqlError>

#include "Globals.h"


DBConnection* DBConnection::theCnx = nullptr;

//Ctor & Dtor
DBConnection::DBConnection(const std::string &path, char* zErrMsg) : _dbPath(path)
{
	QString str = "QSQLITE";
	m_db = QSqlDatabase::addDatabase(str);
	m_db.setDatabaseName(path.c_str());

	if (!m_db.open())
		auto err = m_db.lastError().text();
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


DBConnection* DBConnection::SetConnection(const std::string& dbPath, char* zErrMsg)
{
	if (theCnx == nullptr)
	{
		theCnx = new DBConnection(dbPath, zErrMsg);
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

