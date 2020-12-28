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

	std::string getTitle() { return this->title; }


	News* Link(const std::string _link)
	{
		this->link = _link;
		return this;
	}

	std::string getLink() { return this->link; }


	News* PubDate(const QDate _pubDate)
	{
		this->pubDate = _pubDate;
		return this;
	}

	QDate getPubDate() { return this->pubDate; }


	News* AddCategorie(const std::string _category)
	{
		this->categories.push_back(_category);
		return this;
	}

	std::list<std::string> getCategorie() { return this->categories; }

	
private:
	int id;
	std::string title;
	std::string link;
	std::list<std::string> categories;
	QDate pubDate;
};
