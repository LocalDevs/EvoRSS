#pragma once

#include <QDateTime>
#include "news.h"

class Feeds
{
public:
	//The default ctor -- the members initialization was done just for debug purpose when the values are equal to default
	Feeds() : title("default"), link("default"), description("default"), updPeriod("default"), updFreq("default"){}

	Feeds* Title(const std::string _title)
	{
		this->title = std::move(_title);
		return this;
	}

	std::string getTitle() { return this->title; }

	Feeds* Link(std::string _link)
	{
		this->link = _link;
		return this;
	}

	std::string getLink() { return this->link; }

	Feeds* Description(std::string _descrip)
	{
		this->description = _descrip;
		return this;
	}

	std::string getDescription() { return this->description; }

	Feeds* AddDate(QDateTime _dateAdd)
	{
		this->addDate = _dateAdd;
		return this;
	}

	

	Feeds* UpdatePeriod(std::string _period)
	{
		this->updPeriod = _period;
		return this;
	}

	std::string getUpdPeriod() { return this->updPeriod; }

	Feeds* UpdateFreq(std::string _frequency)
	{
		this->updFreq = _frequency;
		return this;
	}

	std::string getUpdFreq() { return this->updFreq; }

	Feeds* AddNews(std::unique_ptr<News> _article)
	{
		this->news.push_back(std::move(_article));
		return this;
	}
	
private :
	std::string title;
	std::string link;
	std::string description;
	std::string updPeriod;
	std::string updFreq;
	std::list<std::unique_ptr<News>> news;
	QDateTime addDate;
};
