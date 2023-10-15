#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QtDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class Home; }
QT_END_NAMESPACE

class Home : public QWidget
{
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

    //slot functions declarations
private slots:
    void on_btn_exit_clicked();

    void on_btn_login01_clicked();

    //user defined functions
public Q_SLOT:
     void hideLoginLables();
     void My_Eroor_Message(QString *mess, QString *title);
private:
    Ui::Home *ui;
};
#endif // HOME_H
