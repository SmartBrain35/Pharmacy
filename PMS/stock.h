#ifndef STOCK_H
#define STOCK_H

#include <QWidget>
#include <QDesktopWidget>
#include <cmath>

#include <QLCDNumber>
#include <QChart>
#include <QtUiTools>

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>


namespace Ui{class Stock;}

class Stock : public QWidget
{
    Q_OBJECT

public:
    explicit Stock(QWidget *parent = nullptr);
    ~Stock();


private slots:
    void on_btn_exit_clicked();

    void on_btn_logout_clicked();

    void on_part_1_clicked();

    void on_btn_capture_clicked();

    void handle_imageCaptured(int id, const QImage &preview);

    void on_btn_stock_save_clicked();


public Q_SLOT:
 void Hide_some_stock_feature();


private:
     Ui::Stock *ui;
     QCamera *camera;
     QCameraImageCapture *capture;
     QCameraViewfinder *viewfinder;
     void setUpCamera();

};

#endif // STOCK_H
