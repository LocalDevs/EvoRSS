#include "Feeds.h"


DBObjetct Feeds::getNewsAsDBObject()
{
	DBObjetct articleList;

	for (const auto& itr : this->news)
	{
		DBObjetct _innerElement;

		_innerElement.elements.emplace_back(std::make_tuple(NewsTags::Title, FieldType::_STRING, (*itr).getTitle()));
		_innerElement.elements.emplace_back(std::make_tuple(NewsTags::PubDate, FieldType::_DATE, (*itr).getPubDate()));
		_innerElement.elements.emplace_back(std::make_tuple(NewsTags::Category, FieldType::_LIST, (*itr).getCategorie()));
		_innerElement.elements.emplace_back(std::make_tuple(NewsTags::Link, FieldType::_STRING, (*itr).getLink()));
		
		articleList.elements.emplace_back(std::make_tuple(FeedTags::ArticleList, FieldType::_ELEMENT, _innerElement));
	}

	return articleList;
}