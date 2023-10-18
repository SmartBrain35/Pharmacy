#include "part_1.h"
#include "ui_part_1.h"
#include "home.h"
#include "stock.h"
#include "my_database.h"


Part_1::Part_1(QWidget *parent) :QWidget(parent), ui(new Ui::Part_1)
{
    ui->setupUi(this);

    //detecting screen size and fill the form wholy with it
    setContentsMargins(0, 0, 0, 0);
    QDesktopWidget desktop;
    QRect screen = desktop.availableGeometry(this);
    setFixedSize(screen.width(), screen.height());

    //render the form framless
    setWindowFlags(Qt::FramelessWindowHint);

    //set background image
    ui->bg_pic->setPixmap(QPixmap("D:/Projects/AppDev/Pharmacy_2/images/n44.jpg"));

    //setting current  time via timer class
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Part_1::updateTime);
    timer->start();

    //setting visibility for settings_tabs and sales_frame
    ui->my_settings_tabs->setVisible(false);
    ui->my_sales_tabs->setVisible(false);

    //starting codes to open and load drug name from drug_list.txt to ui
    QFile file("D:/Projects/AppDev/Pharmacy_2/Drugs/drugs_List.txt");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            drugsData.append(line);
        }
        file.close();
    }
    connect(ui->drug_name, &QLineEdit::textChanged, this, &Part_1::UpdateSearchDrugOutcome);

    //codes ended...


    // loading sales combox values on app startup
    QStringList list_Box = {"Piece", "Half Pack", "pack"};
    ui->quantity_statusBox->addItems(list_Box);
    ui->quantity_statusBox->setCurrentIndex(0);

    //linking css file i.e part_1_StyleSheet.css to the login UI
    QFile beauty_file("D:/Projects/AppDev/Pharmacy_2/part_1_StyleSheet.css"); //load the file css file
    if(beauty_file.open(QIODevice::ReadOnly | QIODevice::Text)) //if open & read successful
    {
        QTextStream stream(&beauty_file); //convert to text stream
        qApp->setStyleSheet(stream.readAll()); //then app invokes it
    }
    beauty_file.close(); //close opened css file
}

Part_1::~Part_1()
{
    delete ui;
}

//calling database class
My_Database manager("SMART-BRAIN");


//function slot to update lable in every 1 seconds
void Part_1::updateTime()
{
    auto current_time = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::to_time_t(current_time);
    struct tm local_time;

    localtime_s(&local_time, &now);
    std::stringstream ss;

    ss<<std::put_time(&local_time, "%F  %T");
    ui->lbl_time_2->setText(QString::fromStdString(ss.str()));
}


//returning to login page
static Home *login = nullptr; //globally called home class
void Part_1::on_btn_logout_clicked()
{
    if(!login)
    {
        login = new Home();
    }

    close(); //close part_1 form

    login->hideLoginLables();//calling hideLoginLables() slot method;
    login->show(); //open login form
}

//slot function to disable certain features on employee page
void Part_1::DisableFeaturesForEmployee()
{
    ui->btn_Stock->setEnabled(false); //stock button
    ui->btn_settings->setEnabled(false); //settings button
    ui->btn_statistics->setEnabled(false); //statistics button
    ui->my_settings_tabs->setVisible(false);
}

//slot function to disable certain features on admin page
void Part_1::Enable_All_Features_For_Admin()
{
    ui->btn_Stock->setEnabled(true); //stock button
    ui->btn_settings->setEnabled(true); //settings button
    ui->btn_statistics->setEnabled(true); //statistics button
    ui->my_sales_tabs->setHidden(true); //sales_frame
    ui->my_settings_tabs->setHidden(true);

    ui->btn_save->setEnabled(false); //save button
    ui->btn_delete->setEnabled(false); //delete button
    ui->btn_edit->setEnabled(false); //update button
    // ui->btn_load->setEnabled(false); //load button
}

// if stock button is clicked and it is not open already then open the stock form
void Part_1::on_btn_Stock_clicked()
{
    static Stock *stock = nullptr;
    if(!stock)
    {
        stock = new Stock();
    }
    //close current window
    close();

    stock->show();//now open stock
    stock->Hide_some_stock_feature();
}

//handling settings function
void Part_1::on_btn_settings_clicked()
{
    //linking css file i.e settings_StyleSheet.css to the login UI
    QFile beauty_file("D:/Projects/AppDev/Pharmacy_2/settings_StyleSheet.css"); //load the file css file
    if(beauty_file.open(QIODevice::ReadOnly | QIODevice::Text)) //if open & read successful
    {
        QTextStream stream(&beauty_file); //convert to text stream
        qApp->setStyleSheet(stream.readAll()); //then app invokes it
    }
    beauty_file.close(); //close openned css file

    ui->my_settings_tabs->setVisible(true); //show settings tabs
    ui->my_sales_tabs->setVisible(false); //display sales tabs


}

//text browsing from sale page while typing drug name
void Part_1::UpdateSearchDrugOutcome(const QString &text)
{
    //ui->outcome->clear();
    QStringList My_Drugs;

    QStringList::ConstIterator be = drugsData.constBegin();
    QStringList::ConstIterator en = drugsData.constEnd();

    for(; be != en; ++be)
    {
        if(be->toLower().contains(text.toLower()))
        {
            My_Drugs.append(*be);
        }
    }
    ui->outcome->setPlainText(My_Drugs.join("\n"));
}

//add user buttton clicked from settings tap
void Part_1::on_btn_add_user_clicked()
{

    if(manager.Open_Connection())
    {
        //setting up query
        QSqlQuery query(manager.getDatabase());

        //variables to hold Ui values
        int user_Id = ui->show_Id->text().toInt();
        QString user_name = ui->show_name->text();
        QString User_status;

        if(ui->admin_radioButton->isChecked())
        {
            User_status = ui->admin_radioButton->text();
        }else if(ui->sales_person_radioButton->isChecked())
        {
            User_status = ui->sales_person_radioButton->text();
        } else if(ui->supervisor_radioButton->isChecked())
        {
            User_status = ui->supervisor_radioButton->text();
        }else
        {
            User_status = ui->other_radioButton->text();
        }

        QString user_phone = ui->show_phone->text();
        QString user_email = ui->show_email->text();
        QString user_password = ui->show_password->text();
        QString user_address = ui->show_user_address->text();
        QString user_photo = ui->user_photo->text();
        QDateTime Issued_Date = QDateTime::currentDateTime();

        query.prepare("CALL MyUser(:Id, :Name, :Status, :Email, :Password, Phone, :Address, :Photo, :Issued_Date)");
        query.bindValue(":Id", user_Id);
        query.bindValue(":Name", user_name);
        query.bindValue(":Status", User_status);
        query.bindValue(":Email", user_email);
        query.bindValue(":Password", user_password);
        query.bindValue(":Phone", user_phone);
        query.bindValue(":Address", user_address);
        query.bindValue(":Photo", user_photo);
        query.bindValue(":Issued_Date", Issued_Date);

        if(query.exec() && query.next())
        {
            int Login_result = query.value(0).toInt();
            if(Login_result == 1)
            {
                QMessageBox::information(this, "Record Saving", "Record successfully saved");
            }
        }else
        {
            qDebug()<<"Query fail to execute.";
        }

    }else
    {
        QString t = "Database Connectivity.";
        QString m = "Connection failed to establised";
        login->My_Eroor_Message(&t, &m);
    }
    manager.Close_Database();
}

//quitting the application
void Part_1::on_btn_exit_clicked()
{
    qApp->quit();
}


//search for user by name
void Part_1::on_btn_search_user_clicked()
{
    QSqlQuery searchingQuery(manager.getDatabase());
    QString username = ui->show_user_seached->text();
    searchingQuery.prepare("SELECT * FROM ADD_USER WHERE NAME LIKE %':Name'");
    searchingQuery.bindValue(":Name", username);

    if(searchingQuery.exec() && searchingQuery.next())
    {

    }

}

//browse photo for new user
void Part_1::on_browse_user_photo_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(this, "Select Image", "", "Images(*.jpg, *.png, *.jpeg, *.bmp)");

    if(!image_path.isEmpty())
    {
        QPixmap image(image_path);

        if(!image.isNull())
        {
            ui->user_photo->setPixmap(image.scaled(ui->user_photo->size(), Qt::KeepAspectRatio));
        }else
        {
            QMessageBox::critical(this, "Browsing image", "Fail to select image");
        }
    }
}

//show sales form for the employee
void Part_1::on_btn_sales_clicked()
{

    //linking css file i.e sales_styleSheet.css to the login UI
    QFile beauty_file("D:/Projects/AppDev/Pharmacy_2/sales_styleSheet.css"); //load the file css file
    if(beauty_file.open(QIODevice::ReadOnly | QIODevice::Text)) //if open & read successful
    {
        QTextStream stream(&beauty_file); //convert to text stream
        qApp->setStyleSheet(stream.readAll()); //then app invokes it
    }
    beauty_file.close(); //close openned css file


    ui->my_settings_tabs->setVisible(false);
    ui->my_sales_tabs->setVisible(true);
}


//browse for business logo
void Part_1::on_btn_browse_logo_clicked()
{
    //upload business logo here
}

//saving records into sales table
void Part_1::on_btn_save_clicked()
{
    if(manager.Open_Connection())
    {
        QSqlQuery sales_query(manager.getDatabase());

        //QString cust_id = ui->txt_id->text();
        //QString cust_name = ui->txt_name->text();
        //QString cust_phone = ui->txt_phone->text();
       // QString cust_address = ui->txt_address->text();
        QString drugCode = ui->txt_batchCode->text();
        QString drugName = ui->drug_name->text();
        QString drugtype = ui-> txt_drugType->currentText();
        int drugquanity = ui->quantity_counter->text().toInt();
        float drugPrice = ui->txt_price->text().toFloat();

        QString quantity_status = ui->quantity_statusBox->currentText();

        int currentValue = ui->quantity_statusBox->currentIndex();
        if(currentValue == 0)
        {
            quantity_status = sales_query.value(currentValue).toString();
        }else if(currentValue == 1)
        {
            quantity_status = sales_query.value(currentValue).toString();
        }else if(currentValue == 2)
        {
            quantity_status = sales_query.value(currentValue).toString();
        }
        QDate drugDate = QDate::fromString(ui->ex_date->text(), "yyyy-MM-dd");

        QString recomend;
        if(ui->kids_radiaButton->isChecked())
        {
            recomend = ui->kids_radiaButton->text();
        }else if(ui->adult_radiaButton->isChecked())
        {
            recomend = ui->adult_radiaButton->text();
        }else if(ui->male_radiaButton->isChecked())
        {
            recomend = ui->male_radiaButton->text();
        }else
        {
            recomend = ui->female__radiaButton->text();
        }
        QString description = ui->txt_desccribe->text();
        QString drug_pic;

        QString payment_method;
        if(ui->Mtn_radioButton->isChecked())
        {
            payment_method = ui->Mtn_radioButton->text();

        }else if(ui->vodafone_radioButton->isChecked())
        {
            payment_method = ui->vodafone_radioButton->text();

        }else if(ui->creditCard_radioButton->isChecked())
        {
            payment_method = ui->creditCard_radioButton->text();
        }else
        {
            payment_method = ui->cash__radioButton->text();
        }
        QDateTime add_date = QDateTime::currentDateTime();

        //preparing query to insert records i.e insert into table
        sales_query.prepare("CALL MySales (:BatchCode, :Name, :Status, :Quantity_Status, :Quantity, :Sole_Price, :User, :Description, :Exp_date, :Photo, :Issued_Date)");

        // sales_query.bindValue(":Cu_Id", cust_id);
        //sales_query.bindValue(":Cu_Name", cust_name);
        //sales_query.bindValue(":Cu_Phone", cust_phone);
        //sales_query.bindValue(":Cu_Address", cust_address);

        sales_query.bindValue(":BatchCode", drugCode);
        sales_query.bindValue(":Name", drugName);
        sales_query.bindValue(":Status", drugtype);
        sales_query.bindValue(":Quantity_Status", quantity_status);
        sales_query.bindValue(":Quantity", drugquanity);
        sales_query.bindValue(":Sole_Price", drugPrice);
        sales_query.bindValue(":User", recomend);
        sales_query.bindValue(":Description", description);
        sales_query.bindValue(":MFG_date", drugDate);
        sales_query.bindValue(":Photo", drug_pic);
        sales_query.bindValue(":Issued_Date", add_date);
        sales_query.bindValue("", payment_method);

        if(sales_query.exec() && sales_query.next())
        {
            int result = sales_query.value(0).toInt();
            if(result == 1)
            {
                QMessageBox::information(this, "Records Status", "records saved successfully");
            }else
            {
                QMessageBox::critical(this, "Records Status", "records failed to save");
            }

        }else
        {
            QMessageBox::critical(this, "Query Execution", "Database query fail to execute");
        }
    }else
    {
        QMessageBox::critical(this, "Database Connectivity", "Database connection failed to establish");
    }
    manager.Close_Database();//close database connection
}


