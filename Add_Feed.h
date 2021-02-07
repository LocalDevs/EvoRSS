#pragma once

#include <optional>
#include <string>

#include "Feeds.h"
#include "ui_Add_Feed.h"

class Add_Feed : public QDialog, public Ui::Add_Filter
{
	Q_OBJECT

public:
	Add_Feed(QWidget *parent = Q_NULLPTR);
	~Add_Feed();
	bool InsertFeedIntoDB(std::optional<QString> errorMsg, Feeds * _feed);
	static bool CheckFeeAlreadyInDB(QString feedPath);

private slots:
	bool AddNewFeed(std::string feedPath, QString errorMsq);
};
