#include <QMessageBox>
#include <QString>

#include "Add_Feed.h"
#include "Feeds.h"
#include "news.h"
#include "Globals.h"
#include "InfoFeed.h"



// Dependencies for XML parsing process
#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <xmlLite.h>
#include <iostream>




#define getURL URLOpenBlockingStreamA



Add_Feed::Add_Feed(QWidget* parent) : QDialog(parent)
{
	auto lamb = []() {
		QMessageBox msgBox;
		msgBox.setText("The document has been modified.");
		msgBox.exec();
	};

	setupUi(this);
	QString str("Aymen");

	connect(BtnFinish, &QPushButton::clicked,
		[=]() {
			//std::string str = R"(https://blog.panicsoftware.com/feed/)";			
			return(AddNewFeed(FeedPath->text().toLocal8Bit().constData()));
		});
}

Add_Feed::~Add_Feed()
{
}

bool Add_Feed::AddNewFeed(std::string feedPath)
{
	const char* URL = feedPath.c_str();
	IStream* stream;
	HRESULT hr = S_OK;
	bool _mappingError = false;

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
					QString str =  value.c_str();
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

	////View the summary of what we retrieved
	std::unique_ptr<InfoFeed> _infoDlg = std::make_unique<InfoFeed>(params);
	_infoDlg.get()->exec();

	//TODO: to review with US-7
	//executeInsert("INSERT INTO TMP(feed_title, feed_link, feed_description, feed_updateFrequency, feed_updatePeriod) VALUES (?, ?, ?, ?, ?)", params);
	return S_OK;

}