#pragma once
#include <list>
#include <string>
#include <QDateTime>


class News
{
public:
	News() = default;

	News* Title(const std::string _title)
	{
		this->title = _title;
		return this;
	}

	News* Link(const std::string _link)
	{
		this->link = _link;
		return this;
	}

	News* PubDate(const QDateTime _pubDate)
	{
		this->pubDate = _pubDate;
		return this;
	}

	News* AddCategorie(const std::string _category)
	{
		this->categories.push_back(_category);
		return this;
	}
	
private:
	int id;
	std::string title;
	std::string link;
	std::list<std::string> categories;
	QDateTime pubDate;
};
