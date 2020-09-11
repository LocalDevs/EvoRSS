#include "EvoRSS.h"
#include <QMenu>
#include <QMenuBar>


EvoRSS::EvoRSS(QWidget *parent) : QMainWindow(parent)
{
    mainUI_.setupUi(this);

	// adding menus & subMenus
	auto mainMenu = mainUI_.menuBar;

	//File
	auto FileMenu = mainMenu->addMenu("&File");
		//SubMenu
		FileMenu->addAction(new QAction{ "&Import Feeds" });
		FileMenu->addAction(new QAction{ "&Export Feeds" });
		FileMenu->addSeparator();
		FileMenu->addAction(new QAction{ "&Create BackUp" });
		FileMenu->addSeparator();
		FileMenu->addAction(new QAction{ "&Quit" });
	
	//Feed Menu
	auto FeedMenu = menuBar()->addMenu("&Feeds");
		//SubMenu
		FeedMenu->addAction(new QAction{ "&Add Feed" });
		FeedMenu->addAction(new QAction{ "&Update Feed" });
		FeedMenu->addAction(new QAction{ "&Update All" });
		FeedMenu->addSeparator();
		FeedMenu->addAction(new QAction{ "&Mark Read" });
		FeedMenu->addAction(new QAction{ "&Mark All Feeds Read" });
		FeedMenu->addSeparator();
		FeedMenu->addAction(new QAction{ "&Delete" });
		FeedMenu->addSeparator();
		FeedMenu->addAction(new QAction{ "&Feeds Properties" });

	//News
	auto NewsMenu = mainMenu->addMenu("&News");
		//SubMenu
		NewsMenu->addAction(new QAction{ "&Mark All News Read" });
		NewsMenu->addSeparator();
		NewsMenu->addAction(new QAction{ "&Filters" });
		NewsMenu->addAction(new QAction{ "&Sort By" });
		NewsMenu->addSeparator();
		NewsMenu->addAction(new QAction{ "&Delete" });
		NewsMenu->addAction(new QAction{ "&Delete All News" });

	
}
