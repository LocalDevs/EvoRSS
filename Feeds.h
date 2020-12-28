#pragma once

#include "news.h"
#include "Globals.h"

class Feeds
{
public:
	//The default ctor -- the members initialization was done just for debug purpose when the values are equal to default
	Feeds(std::string _rssLink = "empty Link") : rssLink(_rssLink) {};

	std::string getRSS() { return this->rssLink; }

	//Title
	Feeds* Title(const std::string _title)
	{
		this->title = std::move(_title);
		return this;
	}

	std::string getTitle() { return this->title; }

	//Link Home Page
	Feeds* LinkHomePage(std::string _linkHome)
	{
		this->linkHomePage = _linkHome;
		return this;
	}

	std::string getLinkHomePage() { return this->linkHomePage; }

	//Description
	Feeds* Description(std::string _descrip)
	{
		this->description = _descrip;
		return this;
	}

	std::string getDescription() { return this->description; }

	//Date of addition of the Feed
	Feeds* AddDate(QDate _dateAdd)
	{
		this->addDate = _dateAdd;
		return this;
	}

	//Update Period
	Feeds* UpdatePeriod(std::string _period)
	{
		this->updPeriod = _period;
		return this;
	}

	std::string getUpdPeriod() { return this->updPeriod; }

	//Update Frequency
	Feeds* UpdateFreq(std::string _frequency)
	{
		this->updFreq = _frequency;
		return this;
	}

	std::string getUpdFreq() { return this->updFreq; }

	//Adding a News
	Feeds* AddNews(std::unique_ptr<News> _article)
	{
		this->news.push_back(std::move(_article));
		return this;
	}

	DBObjetct getNewsAsDBObject();

private:
	std::string title;
	std::string linkHomePage;
	std::string rssLink;
	std::string description;
	std::string updPeriod;
	std::string updFreq;
	std::list<std::unique_ptr<News>> news;
	QDate addDate;
};
