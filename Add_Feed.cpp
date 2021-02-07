#include <QMessageBox>
#include <QString>

#include "Add_Feed.h"
#include "Feeds.h"
#include "news.h"
#include "Globals.h"
#include "InfoFeed.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


// Dependencies for XML parsing process
#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <xmlLite.h>
#include <iostream>

#include "DBConnection.h"


#define getURL URLOpenBlockingStreamA

bool Add_Feed::CheckFeeAlreadyInDB(QString feedPath)
{
	QSqlQuery sqlQry;
	bool exist = false;
	sqlQry.prepare("Select count(*) from Feed where feed_feedLink = :f_path");
	sqlQry.bindValue(":f_path", feedPath);
	if (sqlQry.exec() && sqlQry.next() && !sqlQry.value(0).toBool())
		return true;

	return false;
}

Add_Feed::Add_Feed(QWidget* parent) : QDialog(parent)
{
	auto lamb = []() {
		QMessageBox msgBox;
		msgBox.setText("The document has been modified.");
		msgBox.exec();
	};

	setupUi(this);
	QString _errorMsg;

	connect(BtnFinish, &QPushButton::clicked,
		[=]() {
			//std::string str = R"(https://blog.panicsoftware.com/feed/)";			
			return(AddNewFeed(FeedPath->text().toLocal8Bit().constData(), _errorMsg));
		});
}

Add_Feed::~Add_Feed() = default;

bool Add_Feed::InsertFeedIntoDB(std::optional<QString> errorMsg, Feeds * _feed)
{
	DB_BEGIN_TRANSACT

		QSqlQuery Sqlqry;

	Sqlqry.prepare("Insert into feed \
						 	(feed_title, feed_alias, feed_homePage, feed_feedLink, feed_category, feed_description, feed_addDate, feed_lastUpdate, feed_lastBuildDate, feed_updatePeriod, feed_updateFrequency) \
						 values \
							(:f_title, :f_alias, :f_homePage, :f_feedLink, :f_category, :f_description, :f_addDate, :f_lastUpdate, :f_lastBuildDate, :f_updatePeriod, :f_updateFrequency)");

	Sqlqry.bindValue(":f_title", _feed->getTitle().c_str());
	Sqlqry.bindValue(":f_alias", _feed->getTitle().c_str());
	Sqlqry.bindValue(":f_homePage", _feed->getLinkHomePage().c_str());
	Sqlqry.bindValue(":f_feedLink", _feed->getRSS().c_str());
	Sqlqry.bindValue(":f_category", "");
	Sqlqry.bindValue(":f_description", _feed->getDescription().c_str());
	Sqlqry.bindValue(":f_addDate", QDate::currentDate());
	Sqlqry.bindValue(":f_lastUpdate", QDate::currentDate());
	Sqlqry.bindValue(":f_lastBuildDate", QDate::currentDate());
	Sqlqry.bindValue(":f_updatePeriod", _feed->getUpdPeriod().c_str());
	Sqlqry.bindValue(":f_updateFrequency", _feed->getUpdFreq().c_str());

	if (!Sqlqry.exec())
	{
		errorMsg = Sqlqry.lastError().text();
		DB_ROLLBACK_TRANSACT
			return false;
	}

	int nextId = -1;

	Sqlqry.prepare("Select feed_id from feed where feed_homePage = :f_homePage");
	Sqlqry.bindValue(":f_homePage", _feed->getLinkHomePage().c_str());

	if (Sqlqry.exec() && Sqlqry.next())
	{
		nextId = Sqlqry.value(0).toInt();
		errorMsg = "The Inserted Feed line was not found " + Sqlqry.lastError().text();
		DB_ROLLBACK_TRANSACT
			return false;
	}

	Sqlqry.prepare(" Insert into News \
							(news_idFeed, news_title, news_link, news_dateInsertion, news_pubDate ) \
						  values \
							(:n_idFeed, :n_title, :n_link, :n_dateInsertion, :n_pubDate )");

	const auto& refElement = std::any_cast<DBObjetct>(std::get<2>(_feed->getNewsAsDBObject().elements[0]));

	for (const auto& allElets : _feed->getNewsAsDBObject().elements)
	{
		for (const auto& elt : std::any_cast<DBObjetct>(std::get<2>(allElets)).elements)
		{
			auto l = std::get<0>(elt);

			switch (std::get<0>(elt))
			{
			case NewsTags::Title:
			case NewsTags::Link:
				Sqlqry.bindValue(":n_link", std::any_cast<std::string>(std::get<2>(elt)).c_str());
				break;
			case NewsTags::PubDate:
				Sqlqry.bindValue(":n_pubDate", std::any_cast<QDate>(std::get<2>(elt)));
				break;
			}

		}

		Sqlqry.bindValue(":n_dateInsertion", QDate::currentDate());
		Sqlqry.bindValue(":n_idFeed", nextId);

		if (!Sqlqry.exec())
		{
			errorMsg = "Can't Insert the article into the articles table " + Sqlqry.lastError().text();
			DB_ROLLBACK_TRANSACT
				return false;
		}
	}

	DB_COMMIT_TRANSACT
		return true;
}

bool Add_Feed::AddNewFeed(std::string feedPath, QString errorMsg)
{
	const char* URL = feedPath.c_str();
	IStream* stream;
	HRESULT hr = S_OK;
	bool _mappingError = false;

	if (!CheckFeeAlreadyInDB(feedPath.c_str()))
	{
		errorMsg = "The feed already exists.";
		return false;
	}

	if (FAILED(hr = getURL(nullptr, URL, &stream, 0, nullptr)))
		return false;

	IXmlReader* rdr = nullptr;
	if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&rdr), 0)))
		return false;

	if (FAILED(hr = rdr->SetInput(stream)))
		return false;

	const wchar_t* element = 0;
	XmlNodeType nodeType;

	std::unique_ptr<Feeds> _feed = nullptr;
	std::unique_ptr<News> _article = nullptr;

	bool _isFeed = false;
	bool _isArticle = false;

	while (S_OK == rdr->Read(&nodeType))
	{
		switch (nodeType)
		{
		case XmlNodeType_Element:
		{
			rdr->GetLocalName(&element, 0);
			if (!element)
				break;

			// Keep trace if in Channel or Item Element					
			auto wstrElement = std::wstring(element);
			auto strElement = std::string(wstrElement.begin(), wstrElement.end());

			if (element && !strElement.compare("channel"))
			{
				_isFeed = true;
				_isArticle = false;
			}
			else if (element && !strElement.compare("item"))
			{
				_isArticle = true;
				_isFeed = false;
			}
		}

		break;

		case XmlNodeType_Text:
		case XmlNodeType_CDATA:
		{
			if (!element)
				break;

			const wchar_t* val = 0;

			if (FAILED(rdr->GetValue(&val, 0)))
				break;

			auto value = wcharToString(val);
			getTruncatedElement(value);

			//Due to complexity of the xml content it is difficult to say when we are not
			//in a feed as some elements may be contained in both feed & Articles 
			//and some of the feed may not be present as Update Frequency & Hours

			if (_isFeed)
			{
				switch (stringToEnum<FeedTags>(wcharToString(element)))
				{
				case FeedTags::Title:
					//Ignore all the title elements if the _feed object is already created
					//as the read url feed may contain many title elements for the channel
					if (!_feed)
					{
						_feed = std::make_unique<Feeds>(feedPath);
						_feed->Title(value);
					}
					break;

				case FeedTags::LinkHomePage:
					_feed->LinkHomePage(value);
					break;

				case FeedTags::Description:
					_feed->Description(value);
					break;

				case FeedTags::UpdatePeriod:
					_feed->UpdatePeriod(value);
					break;

				case FeedTags::UpdateFrequency:
					_feed->UpdateFreq(value);
					break;
				default:
					break;
				}
			}
			else if (_isArticle)
			{
				switch (stringToEnum<NewsTags>(wcharToString(element)))
				{
				case NewsTags::Title:
					//Test we item contains more the one title subtag
					if (!_article)
					{
						_article = std::make_unique<News>();
						_article->Title(value);
					}
					break;
				case NewsTags::Category:
					_article->AddCategorie(value);
					break;
				case NewsTags::Link:
					if (_article)
						_article->Link(value);
					break;
				case NewsTags::PubDate:
					//TODO : cast the value to QDateTime
					QString str = value.c_str();
					QStringList dateList = str.split(",")[1].split("+")[0].split(" ");
					dateList.removeAll(" ");

					_article->PubDate(
						QDate(dateList[3].toInt(), MonthToInt(dateList[2].toStdString()), dateList[1].toInt())
						/*QDateTime(
							QDate(dateList[].toInt(), MonthToInt(dateList[1].toStdString()), dateList[0].toInt()),
							QTime(22, 10, 0)
						)*/
					);

					break;
				}
			}
		}
		break;

		case XmlNodeType_EndElement:
		{
			rdr->GetLocalName(&element, 0);
			if (!element)
				break;

			if (!_wcsicmp(element, L"item") && _isArticle)
			{
				_feed->AddNews(std::move(_article));
				_isArticle = false;
			}
		}
		break;

		default:
			break;
		}

	}

	stream->Release();
	rdr->Release();

	DBObjetct params;
	params.elements.emplace_back(std::make_tuple(FeedTags::Title, FieldType::_STRING, std::forward<std::string>(_feed->getTitle())));
	params.elements.emplace_back(std::make_tuple(FeedTags::LinkHomePage, FieldType::_STRING, std::forward<std::string>(_feed->getLinkHomePage())));
	params.elements.emplace_back(std::make_tuple(FeedTags::Description, FieldType::_STRING, std::forward<std::string>(_feed->getDescription())));
	params.elements.emplace_back(std::make_tuple(FeedTags::UpdateFrequency, FieldType::_STRING, std::forward<std::string>(_feed->getUpdFreq())));
	params.elements.emplace_back(std::make_tuple(FeedTags::UpdatePeriod, FieldType::_STRING, std::forward<std::string>(_feed->getUpdPeriod())));
	params.elements.emplace_back(std::make_tuple(FeedTags::RssFeed, FieldType::_STRING, std::forward<std::string>(_feed->getRSS())));
	params.elements.emplace_back(std::make_tuple(FeedTags::ArticleList, FieldType::_LIST, DBObjetct(_feed->getNewsAsDBObject())));

	//View the summary of what we retrieved
	//std::unique_ptr<InfoFeed> _infoDlg = std::make_unique<InfoFeed>(params);
	//_infoDlg.get()->exec();

	if (!InsertFeedIntoDB(std::move(errorMsg), _feed.get()))
	{
		errorMsg = "An Error did happened when Inerting the new Feed. Check the Error Msg";
		return false;
	}

	return true;
}

