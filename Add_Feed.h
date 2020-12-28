#pragma once

#include <string>
#include "ui_Add_Feed.h"

class Add_Feed : public QDialog, public Ui::Add_Filter
{
	Q_OBJECT

public:
	Add_Feed(QWidget *parent = Q_NULLPTR);
	~Add_Feed();

private slots:
	bool AddNewFeed(std::string feedPath);
};
