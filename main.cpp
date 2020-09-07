#include "EvoRSS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EvoRSS w;
    w.show();
    return a.exec();
}
