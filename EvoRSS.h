#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include "ui_EvoRSS.h"


class EvoRSS : public QMainWindow
{
    Q_OBJECT

public:
    EvoRSS(QWidget *parent = Q_NULLPTR);
protected:

    std::unique_ptr<QAction> addFeed;
	
private:
    Ui::EvoRSSClass mainUI_{};
    void CreateMenu();
    void CreateStatusBar() const;
};
