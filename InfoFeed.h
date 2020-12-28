#pragma once

//#include <QDateTime>
#include <QDialog>
#include "Globals.h"
#include "ui_Infos_Feed.h"


class InfoFeed : public QDialog, Ui::Info_Feed
{
public:
	explicit InfoFeed(const DBObjetct& feed, QWidget* parent = Q_NULLPTR);
private:
protected:
};

class Model : public QAbstractTableModel {
	std::vector<DBObjetct> m_data;
public:
	const int columnsCount = 4;
	
public:
	Model(QObject* parent = {}) : QAbstractTableModel{ parent }
	{
		m_data.clear();
	}

	int rowCount(const QModelIndex&) const override { return m_data.size(); }

	int columnCount(const QModelIndex&) const override { return columnsCount; }

	QVariant data(const QModelIndex& index, int role) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	void append(const DBObjetct& el);
};