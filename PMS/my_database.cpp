#include "my_database.h"

My_Database::My_Database(const QString& serverName)
{
    //establishing database configuration //SMART-BRAIN
    mdb = QSqlDatabase::addDatabase("QODBC3");

    mdb.setDatabaseName("DRIVER={SQL Server}; Server= "+serverName+"; Database=PHARMACY; Trusted_Conncection = true");


}

//check if database connectivity is yes
bool My_Database::Open_Connection()
{
    mdb.open();
    return true;
}

//closing connection and remove database before it goes outta scope
bool My_Database::Close_Database()
{
    mdb.close();
    mdb.removeDatabase("QODBC3");
    return true ;
}

QSqlDatabase My_Database::getDatabase()
{
    return mdb;
}
