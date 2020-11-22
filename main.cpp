#include "EvoRSS.h"
#include <QtWidgets/QApplication>
#include "DBConnection.h"





int main(int argc, char *argv[])
{
		char* zErrMsg = nullptr;
	
		DBConnection* myCnx = DBConnection::GetConnection(R"(C:\LocalDevs\EvoRSS\DBase\EvoRSS.db)", zErrMsg);
      
		if (myCnx)
			myCnx->executeQuery(R"(SELECT * FROM 'Fees')", [](void* ctx, int argc, char** argv, char** columnName) -> int
                {
                    //static_cast<vector<SomeClass>*>(ctx)->push_back(SomeClass());
                    return 0;
                }, zErrMsg);


    QApplication a(argc, argv);
    EvoRSS w;
    w.show();
    return a.exec();
}
