#include "InfoFeed.h"
#include <QDateTime>

#include <ios>
#include <tuple>

InfoFeed::InfoFeed(const DBObjetct& feed, QWidget* parent)
{
	setupUi(this);

	for (const auto& element : feed.elements)
	{
		switch (std::get<0>(element))
		{
		case FeedTags::Title:
			this->EdtTitle->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::Description:
			this->EdtDescription->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::LinkHomePage:
			this->EdtHomePage->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::UpdateFrequency:
			this->EdtUpdFrequency->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::UpdatePeriod:
			this->EdtUpdPeriod->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::RssFeed:
			this->Edtlink->setText(std::any_cast<std::string>(std::get<2>(element)).c_str());
			break;

		case FeedTags::ArticleList:
			Model* model = new Model();
			model->append(std::any_cast<DBObjetct>(std::get<2>(element)));
			this->FeedArticles->verticalHeader()->setDefaultSectionSize(10);
			this->FeedArticles->setSelectionBehavior(QAbstractItemView::SelectRows);
			this->FeedArticles->setModel(model);
			this->FeedArticles->resizeColumnsToContents();

			int _widthCols = 0;
			for (int colIndex = 0; colIndex < model->columnsCount; ++colIndex)
				_widthCols += this->FeedArticles->columnWidth(colIndex);



			this->resize(_widthCols + this->QLArticles->width() + this->FeedArticles->autoScrollMargin(), this->size().height());
			this->FeedArticles->setStyleSheet("QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #616161, stop: 0.5 #505050,\
				stop: 0.6 #434343, stop:1 #656565);\
				color: white;\
				padding - std::left: 0px;\
				border: 0px solid #6c6c6c;}");

			this->FeedArticles->verticalHeader()->setHighlightSections(false);

			this->FeedArticles->show();
			break;
		}
	}
}

QVariant Model::data(const QModelIndex& index, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
	std::string categs;

	auto l = std::any_cast<QDate>(std::get<2>(m_data[index.row()].elements[1]));

	//TODO : encapsulate the code with try catch coz the headers my change by time
	switch (index.column())
	{
	case 0: return std::to_string(index.row() + 1).c_str();
	case 1: return std::any_cast<std::string>(std::get<2>(m_data[index.row()].elements[0])).c_str();
	case 2:
		return std::any_cast<QDate>(std::get<2>(m_data[index.row()].elements[1])).toString(Qt::SystemLocaleShortDate);
	case 3:
		for (std::string category : std::any_cast<std::list<std::string>>(std::get<2>(m_data[index.row()].elements[2])))
			categs += category + ", ";
		return categs.c_str();

	default: return "";
	}
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
	switch (section)
	{
	case 0: return "";
	case 1: return "Title";
	case 2: return "PubDate";
	case 3: return "Category";
		//case 3: return "Link";
	default: return {};
	}
}

void Model::append(const DBObjetct& el)
{
	beginInsertRows({}, 0, 0);

	for (const auto& innerElement : el.elements)
	{
		m_data.emplace_back(std::any_cast<DBObjetct>(std::get<2>(innerElement)));
	}

	endInsertRows();
}
