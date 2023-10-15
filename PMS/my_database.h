#ifndef MY_DATABASE_H
#define MY_DATABASE_H

#include <QObject>
#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QMessageBox>

class My_Database
{
public:
    My_Database(const QString& serverName);


public Q_SLOTS:
    bool Open_Connection();

    bool Close_Database();

   QSqlDatabase getDatabase();


private:
    QSqlDatabase mdb;
};

#endif // MY_DATABASE_H
