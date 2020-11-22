#pragma once
#include <string>
#include <algorithm>
#include <any>
#include <vector>

//Enum used for the elements type
// of the elements vector
enum FieldType
{
	_INT = 0,
	_DOUBLE,
	_STRING,
	_LIST,
	_DATE
};

struct DBObjetct
{
	std::vector <std::pair<FieldType, std::any>> elements;
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
	static const int Link = 2;
	static const int Description = 3;
	static const int UpdatePeriod = 4;
	static const int UpdateFrequency = 5;
};

class NewsTags : public ITags
{
public:

	static const int Title = 1;
	static const int Link = 2;
	static const int PubDate = 3;
	static const int Category = 4;
};

//enum class FeedTags
//{
//	Error = -1,
//	Title,
//	Link,
//	Description,
//	UpdatePeriod,
//	UpdateFrequency
//};

//enum class NewsTags
//{
//	Title,
//	Link,
//	PubDate,
//	Category
//};

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
		if (stringValue == "link")				return FeedTags::Link;
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
		/*Case a non handled enum type*/		return - 1;
}

//Database Querries
bool executeInsert(std::string insert_Qry, DBObjetct params);