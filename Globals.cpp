#include "Globals.h"
#include <algorithm>

#include "DBConnection.h"

std::string wcharToString(const wchar_t* val)
{
	if (val == nullptr)
		return std::string();

	auto w_str = std::wstring(val);

	return std::string(w_str.begin(), w_str.end());
}

void getTruncatedElement(std::string& _srcStr)
{
	_srcStr.erase(std::remove_if(_srcStr.begin(), _srcStr.end(),
		[](const char c) {return c == '\n' || c == '\t'; }), _srcStr.end());
}

//TODO: to review with US-7

//bool executeInsert(std::string insert_Qry, DBObjetct params)
//{
//	return DBConnection::GetConnection()->insertQuery(std::move(insert_Qry), params);
//}




