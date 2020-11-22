#include "Add_Feed.h"
#include "Feeds.h"
#include "news.h"
#include <QMessageBox>
#include <QString>
#include "Globals.h"

// Dependencies for XML parsing process
#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <xmlLite.h>
#include <iostream>




#define getURL URLOpenBlockingStreamA



Add_Filter::Add_Filter(QWidget* parent) : QDialog(parent)
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
			std::string str = R"(https://akrzemi1.wordpress.com/feed/)";//FeedPath->text().toLocal8Bit().constData();
			return(AddNewFeed(str));
		});
}

Add_Filter::~Add_Filter()
{
}

bool Add_Filter::AddNewFeed(std::string feedPath)
{
	const char* URL = feedPath.c_str();
	IStream* stream;
	HRESULT hr = S_OK;
	bool _mappingError = false;

	if (FAILED(hr = getURL(nullptr, URL, &stream, 0, nullptr)))
		return false;

	IXmlReader* rdr;
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
				_isFeed = true;
			else if (element && !strElement.compare("item"))
				_isArticle = true;
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

			if (_isFeed && _isArticle)
			{
				_mappingError = true;
				//TODO : Add Log Instance - description isFeed and isArticle are both true - can't have both cases on same Time
				return false;
			}

			if (_isFeed)
			{
				switch (stringToEnum<FeedTags>(wcharToString(element)))
				{
				case FeedTags::Title:
					if (_feed)
					{
						_mappingError = true;
						//TODO : Add Log Instance - the isFeed was set to false after the first read of title - how did we get here ?
						return false;
					}
					_feed = std::make_unique<Feeds>();
					_feed->Title(value);
					break;

				case FeedTags::Link:
					_feed->Link(value);
					break;

				case FeedTags::Description:
					_feed->Description(value);
					break;

				case FeedTags::UpdatePeriod:
					_feed->UpdatePeriod(value);
					break;

				case FeedTags::UpdateFrequency:
					_feed->UpdateFreq(value);
					_isFeed = false;
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
					//article.get()->PubDate(value);
					break;
				deafult:
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
	params.elements.emplace_back(std::make_pair(FieldType::_STRING, std::forward<std::string>(_feed->getTitle())));
	params.elements.emplace_back(std::make_pair(FieldType::_STRING, std::forward<std::string>(_feed->getLink())));
	params.elements.emplace_back(std::make_pair(FieldType::_STRING, std::forward<std::string>(_feed->getDescription())));
	params.elements.emplace_back(std::make_pair(FieldType::_STRING, std::forward<std::string>(_feed->getUpdFreq())));
	params.elements.emplace_back(std::make_pair(FieldType::_STRING, std::forward<std::string>(_feed->getUpdPeriod())));

	executeInsert("INSERT INTO TMP(feed_title, feed_link, feed_description, feed_updateFrequency, feed_updatePeriod) VALUES (?, ?, ?, ?, ?)", params);
	return S_OK;

}