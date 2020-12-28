#pragma once
#include <sqlite3.h>
#include <string>
#include "Globals.h"

class DBConnection
{
public:

	//Delete default Ctor
	DBConnection() = delete;
	//Delete Copy Ctor & Assignment
	DBConnection(const DBConnection& srcConnection) = delete;
	DBConnection& operator = (const DBConnection& srcConnection) = delete;
	//Delete Move Ctor & Assignment
	DBConnection(const DBConnection&& srcConnection) = delete;
	DBConnection& operator = (const DBConnection&& srcConnection) = delete;
	

	static DBConnection* GetConnection(char* zErrMsg = nullptr);
	//TODO : this function is to be modified to SetConnection
	static DBConnection* GetConnection(std::string dbPath, char* zErrMsg);

	//TODO: to review with US-7

	//INSERT Queries
	//bool insertQuery(const std::string insertSql, DBObjetct params);
	
	bool executeQuery(const std::string sql, sqlite3_callback callback, char*& zErrMsg);
	
	
	void closeConnection();

	//Destructor
	~DBConnection();
private:	
	DBConnection(std::string path, char* szrErrorMsg);
	

	//The DB Path
	std::string _dbPath;
	//The DB Pointer
	sqlite3* _theDb = nullptr;

	//Static pointer for the connection Instance
	static DBConnection* theCnx;
};

