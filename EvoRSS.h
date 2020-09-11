#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EvoRSS.h"

class EvoRSS : public QMainWindow
{
    Q_OBJECT

public:
    EvoRSS(QWidget *parent = Q_NULLPTR);

private:
    Ui::EvoRSSClass mainUI_{};
};
