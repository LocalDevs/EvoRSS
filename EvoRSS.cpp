#include "EvoRSS.h"
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <memory>
#include "Add_Feed.h"

//TODO : figure out how to use smart pointers : don't use static if possible

std::unique_ptr<QLabel> Lab1, Lab2, Lab3;

EvoRSS::EvoRSS(QWidget* parent) : QMainWindow(parent)
{
	mainUI_.setupUi(this);
	auto stat = statusBar();

	//Create The Menu & Status Bar
	this->CreateMenu();
	this->CreateStatusBar();
}

void EvoRSS::CreateStatusBar() const
{

	auto statusBar = this->statusBar();
	Lab1 = std::make_unique<QLabel>(tr("Db Cnx : Ok"));
	Lab1->setFixedWidth(350);
	Lab1->setAlignment(Qt::AlignLeft);
	statusBar->addPermanentWidget(Lab1.get());

	Lab2 = std::make_unique<QLabel>(tr("Unread : 150"));
	Lab2->setFixedWidth(350);
	Lab2->setAlignment(Qt::AlignCenter);
	statusBar->addPermanentWidget(Lab2.get());

	Lab3 = std::make_unique<QLabel>(tr("Group : C++"));
	Lab3->setFixedWidth(350);
	Lab3->setAlignment(Qt::AlignRight);
	statusBar->addPermanentWidget(Lab3.get());
}

void EvoRSS::CreateMenu()
{
	auto mainMenu = menuBar();

	auto fileMenu = mainMenu->addMenu("&File");
	//SubMenu
	fileMenu->addAction(new QAction{ "&Import Feeds" });
	fileMenu->addAction(new QAction{ "&Export Feeds" });
	fileMenu->addSeparator();
	fileMenu->addAction(new QAction{ "&Create BackUp" });
	fileMenu->addSeparator();
	fileMenu->addAction(new QAction{ "&Quit" });

	//Feed Menu
	auto feedMenu = mainMenu->addMenu("&Feeds");

		//SubMenu
		this->addFeed = std::make_unique<QAction>("&Add Feed");
		feedMenu->addAction(this->addFeed.get());

		//Adding Feed CallFunction
		auto lamb = []() {
			Add_Filter x = new Add_Filter();
			x.exec();
		};

		//Add_Filter x = new Add_Filter();
		connect(addFeed.get(), &QAction::triggered, qApp, lamb);

	feedMenu->addAction(new QAction{ "&Update Feed" });
	feedMenu->addAction(new QAction{ "&Update All" });
	feedMenu->addSeparator();
	feedMenu->addAction(new QAction{ "&Mark Read" });
	feedMenu->addAction(new QAction{ "&Mark All Feeds Read" });
	feedMenu->addSeparator();
	feedMenu->addAction(new QAction{ "&Delete" });
	feedMenu->addSeparator();
	feedMenu->addAction(new QAction{ "&Feeds Properties" });



	//News
	auto newsMenu = mainMenu->addMenu("&News");
	//SubMenu
	newsMenu->addAction(new QAction{ "&Mark All News Read" });
	newsMenu->addSeparator();
	newsMenu->addAction(new QAction{ "&Filters" });
	newsMenu->addAction(new QAction{ "&Sort By" });
	newsMenu->addSeparator();
	newsMenu->addAction(new QAction{ "&Delete" });
	newsMenu->addAction(new QAction{ "&Delete All News" });
}