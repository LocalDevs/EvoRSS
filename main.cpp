#include "EvoRSS.h"
#include <QtWidgets/QApplication>

#include "DBConnection.h"
#include <QtSql/QSqlError>




int main(int argc, char *argv[])
{
		char* zErrMsg = nullptr;

	//TODO: make the path generic
		DBConnection* myCnx = DBConnection::SetConnection(R"(C:\LocalDevs\EvoRSS\DBase\EvoRSS.db)", zErrMsg);
      
		if (myCnx)
		{
			QSqlQuery query;
			bool success = false;
			
			if (query.exec("SELECT feed_id, feed_title FROM Feed"))
			{
				while ( query.next())
				{
					auto res = query.value("Feed_title");
					res = query.value("Feed_id");
				}
				success = true;
			}
			else
			{
				QSqlError strError =  query.lastError();
				auto l = strError.text();
			}
		}

    QApplication a(argc, argv);
    EvoRSS w;
    w.show();
    return a.exec();
}
