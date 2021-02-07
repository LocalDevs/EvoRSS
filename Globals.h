#pragma once
#include <string>
#include <algorithm>
#include <any>
#include <vector>

namespace macro_QSQlDB
{
#define DB_BEGIN_TRANSACT QSqlDatabase::database().transaction();
#define DB_COMMIT_TRANSACT QSqlDatabase::database().commit();
#define DB_ROLLBACK_TRANSACT QSqlDatabase::database().rollback();
	int a = 0;
}

//Enum used for the elements type
// of the elements vector
enum FieldType
{
	_INT = 0,
	_DOUBLE,
	_STRING,
	_LIST,     //this means it is a list of Feeds or News
	_ELEMENT,  //this means it is a line representing Feed or News
	_DATE
};

typedef  std::tuple<int, FieldType, std::any> DBObjType;

//template <typename T>
struct DBObjetct
{
	//typedef T value_type;
	std::vector <std::tuple<int, FieldType, std::any>> elements;
};

namespace
{
	int MonthToInt(const std::string& stringValue)
	{
		std::vector<std::string> innerList{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		return 1 + std::distance(innerList.begin(), std::find(innerList.begin(), innerList.end(), stringValue));
	}
};


class ITags
{
public:
	ITags() = default;
	static const int Error = -1;
};

class FeedTags : public ITags
{
public:

	static const int Title = 1;
	static const int LinkHomePage = 2;
	static const int RssFeed = 3;
	static const int Description = 4;
	static const int UpdatePeriod = 5;
	static const int UpdateFrequency = 6;
	static const int ArticleList = 7;			//this means it is a list of News(articles) inside the Feed Object

	static const int MAX_ID = 8;
};

class NewsTags : public ITags
{
public:

	static const int Title = FeedTags::MAX_ID + 1;
	static const int Link = FeedTags::MAX_ID + 2;
	static const int PubDate = FeedTags::MAX_ID + 3;
	static const int Category = FeedTags::MAX_ID + 4;
};

//get the string from the underlying pointer
std::string wcharToString(const wchar_t* val);

//truncate the special caracters
void getTruncatedElement(std::string& _srcStr);

//this function maps the stringValue to the enumeration
////Used to be able to apply switch on the tags for Feeds
template <class T>
int stringToEnum(std::string stringValue)
{
	std::transform(stringValue.begin(), stringValue.end(), stringValue.begin(), [&](unsigned char c) {return std::tolower(c); });

	if (std::is_base_of<FeedTags, T>::value)
	{
		if (stringValue == "title")				return FeedTags::Title;
		if (stringValue == "link")				return FeedTags::LinkHomePage;
		if (stringValue == "description")		return FeedTags::Description;
		if (stringValue == "updateperiod")		return FeedTags::UpdatePeriod;
		if (stringValue == "updatefrequency")	return FeedTags::UpdateFrequency;
		/*Case no mapping could be found*/		return FeedTags::Error;
	}
	else if (std::is_base_of<NewsTags, T>::value)
	{
		if (stringValue == "title")				return NewsTags::Title;
		if (stringValue == "link")				return NewsTags::Link;
		if (stringValue == "pubdate")			return NewsTags::PubDate;
		if (stringValue == "category")			return NewsTags::Category;
		/*Case no mapping could be found*/		return NewsTags::Error;
	}
	else
		/*Case a non handled enum type*/		return -1;
}

//TODO: to review with US-7
//Database Querries
//bool executeInsert(std::string insert_Qry, DBObjetct params);