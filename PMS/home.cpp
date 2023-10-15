#include "home.h"
#include "ui_home.h"
#include "part_1.h"
#include "my_database.h"


Home::Home(QWidget *parent) : QWidget(parent), ui(new Ui::Home)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint); //frameless window
    setFixedSize(1100, 571); //setting fix size of the frame

    //setting background and profile image on the login
    ui->bg_image->setPixmap(QPixmap("D:/Projects/AppDev/Pharmacy_2/images/n33.jpg"));
    ui->lbl_dp->setPixmap(QPixmap("D:/Projects/AppDev/Pharmacy_2/images/n1.jpg"));

    //hidding login form when app starts
    ui->login_frame->close();

    //setting the range and the initial value of the progressBar
    ui->flash_progress->setRange(1, 100);
    ui->flash_progress->setValue(1);

    //creating timer object to update the progressBar
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        int CurrentValue = ui->flash_progress->value();

        if(CurrentValue < 100)
        {
            ui ->flash_progress->setValue(CurrentValue + 1);
            if(CurrentValue == 20)
            {
                ui->lbl_loading_text->setText("Initializing system....");
            }if(CurrentValue == 40)
            {
                ui->lbl_loading_text->setText("connecting database....");
            }if(CurrentValue == 60)
            {
                ui->lbl_loading_text->setText("Almost there....");
            }if(CurrentValue == 90)
            {
                ui->lbl_loading_text->setText("Setup finished....");
            }

        }else
        {
            timer->stop();//stop timer when progressBar reach 100

            //open login window when progressBar reach 100% while some widgets are being hidden
            ui->login_frame->show();
            ui->flash_progress->setHidden(true);
            ui->lbl_loading_text->setHidden(true);
            ui->line_2->setHidden(true);
            ui->label_4->setHidden(true);
            ui->label_5->setHidden(true);
            ui->label_3->setHidden(true);
        }
    });

    //timer interval in milliseconds
    timer->setInterval(100);
    timer->start();

    //adding items to the comboBox
    QStringList list ={"--select status--", "Employee", "Admin"};
    ui->cmb_status->addItems(list);
    ui->cmb_status->setCurrentIndex(0);
}

Home::~Home()
{
    delete ui;
}

//function to show error messsage
void Home::My_Eroor_Message(QString *title, QString *mess)
{
    QMessageBox message;

    message.setInformativeText(*title);
    message.setText(*mess);
    message.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    message.setIcon(QMessageBox::Critical);
    message.exec();
}

//connecting LOGIN form to ADD_USER table in the database
//open the main form after login button is clicked
void Home::on_btn_login01_clicked()
{
    My_Database manager("SMART-BRAIN");

    if(manager.Open_Connection())
    {
        //database operations
        QSqlQuery query(manager.getDatabase());

        QString login_comboBox = ui->cmb_status->currentText();
        QString login_email= ui->txt_email->text();
        QString login_password = ui->txt_password->text();
        QString login_photo = ui->lbl_dp->text();

        query.exec("SELECT Status, Email, Password, Photo FROM ADD_USER WHERE Status =:Status, Email =:Email, Password =:Password, Photo =: Photo");
        query.bindValue(":Status", login_comboBox);
        query.bindValue(":Email", login_email);
        query.bindValue(":Password", login_password);
        query.bindValue(":Photo", login_photo);

        if(query.exec() && query.next())
        {
            int Login_result = query.value(0).toInt();

            //assign query values to these variables and then compare them to user inputs
            QString storeProc_cmb_status = query.value(login_comboBox).toString();
            QString storeProc_email= query.value(login_email).toString();
            QString storeProc_password = query.value(login_password).toString();

            if(Login_result == 1)
            {
                //opening main form i.e Part_1 window by creating an instance of the class Part_1
                static Part_1 *form_1 = nullptr;

                //validating the comboBox
                int currentIndex = ui->cmb_status->currentIndex();

                //email and password too are being validated
                if( (currentIndex == 0) || (login_email.isEmpty()) || login_password.isEmpty() || (login_comboBox != storeProc_cmb_status) || (login_email != storeProc_email) || (storeProc_password != storeProc_password))
                {
                    QString t = "validating failed !";
                    QString m = "Enter valid credentials...!";
                    My_Eroor_Message(&t, &m);

                }else if((currentIndex == 1) && (!login_email.isEmpty()) && (!login_password.isEmpty()) && (login_comboBox == storeProc_cmb_status) && (login_email == storeProc_email) && (storeProc_password == storeProc_password))
                {

                    //if it is not open then open the form_1 for employee
                    if(!form_1)
                    {
                        form_1 = new Part_1();
                    }

                    close(); //close the login form

                    //disable some features when you login as "Employee"
                    form_1->DisableFeaturesForEmployee();


                    form_1->show(); //show the Part_1 form

                }else if((currentIndex == 2) && (!login_email.isEmpty()) && (!login_password.isEmpty()) && (login_comboBox == storeProc_cmb_status) && (login_email == storeProc_email) && (storeProc_password == storeProc_password))
                {
                    //if it is not open then open the form_1 for admin
                    if(!form_1)
                    {
                        form_1 = new Part_1();
                    }

                    close(); //close the login form

                    //disable some features when you login as "Employee"
                    form_1->Enable_All_Features_For_Admin();
                    form_1->show(); //show the Part_1 form
                }

            }else
            {
                qWarning()<<"Validation Failed...\n";
            }

        }else
        {
            QString t = "Store Procedure: ";
            QString m = "query execution failed: Store procedure returned unexpected values";
            My_Eroor_Message(&t, &m);
        }
    }else
    {
        // Display an error message if the connection fails
        QMessageBox::critical(nullptr, "Database Error", "Failed to connect to the database");

    }

    // Both "manager" and "query" are destroyed before they go outta scope
    manager.Close_Database();
}

//slot mehtod to hide some labels on the login page
void Home::hideLoginLables()
{
    ui->flash_progress->setHidden(true);
    ui->lbl_loading_text->setHidden(true);
    ui->line_2->setHidden(true);
    ui->label_4->setHidden(true);
    ui->label_5->setHidden(true);
    ui->label_3->setHidden(true);
    ui->login_frame->show();
}


//quitting the application
void Home::on_btn_exit_clicked()
{
    qApp->exit();
}
