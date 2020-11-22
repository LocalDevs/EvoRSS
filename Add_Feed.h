#pragma once

#include <QDialog>
#include <memory>
#include <string>
#include "ui_Add_Feed.h"

class Add_Filter : public QDialog, public Ui::Add_Filter
{
	Q_OBJECT

public:
	Add_Filter(QWidget *parent = Q_NULLPTR);
	~Add_Filter();

private slots:
	bool AddNewFeed(std::string feedPath);
};
