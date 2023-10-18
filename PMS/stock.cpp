#include "stock.h"
#include "ui_stock.h"
#include "home.h"
#include "part_1.h"
#include "my_database.h"

Stock::Stock(QWidget *parent) : QWidget(parent), ui(new Ui::Stock)
{
    ui->setupUi(this);

    /*setting windows size and removing any border and controlBox*/
    //detecting system screen size automatically and fill the whole form into it
    setContentsMargins(0, 0, 0, 0);
    QDesktopWidget desktop;
    QRect system_screen = desktop.availableGeometry(this);
    setFixedSize(system_screen.width(), system_screen.height());

    //making the stock form framless
    setWindowFlags(Qt::FramelessWindowHint);

    //set background color or image
    ui->stock_bg_pic->setPixmap(QPixmap("D:/Projects/AppDev/Pharmacy_2/images/STOCK.jpg"));

    //loading stock quantity combox
    QStringList QStatus_Box = {"--select quantity--", "Half pack", "Piece", "Pack"};
    ui->quantityStatus_Box->addItems(QStatus_Box);
    ui->quantityStatus_Box->setCurrentIndex(0);

    //loading stock drug type combox
    QStringList drugType_Box = {"--drug type--", "Pain Killer", "Malaria", "Contraceptive", "Infection", "General"};
    ui->show_status_comboBox->addItems(drugType_Box);
    ui->show_status_comboBox->setCurrentIndex(0);


    //linking css file i.e stock_StyleSheet.css to the login UI
    QFile beauty_file("D:/Projects/AppDev/Pharmacy_2/stock_StyleSheet.css"); //load the file css file
    if(beauty_file.open(QIODevice::ReadOnly | QIODevice::Text)) //if open & read successful
    {
        QTextStream stream(&beauty_file); //convert to text stream
        qApp->setStyleSheet(stream.readAll()); //then app invokes it
    }
    beauty_file.close(); //close openned css file
}

Stock::~Stock()
{
    delete ui;
    camera->stop(); //stoping camera
}


//close the application
void Stock::on_btn_exit_clicked()
{
    camera->stop(); //stoping camera
    qApp->quit();
}


//go to login form
void Stock::on_btn_logout_clicked()
{
    static Home *login = nullptr;
    if(!login)//if login is not open
    {
        login = new Home();
    }else
    {
        close();
    }

    camera->stop(); //stoping camera

    login->show(); //open it
    login->hideLoginLables();
    close();
}


//function to hide some features by default when stock form is opened
void Stock::Hide_some_stock_feature()
{
    ui->stock_tabs->setVisible(true);
}


//return to main screen
void Stock::on_part_1_clicked()
{
    static Part_1 *part_1 = nullptr;
    if(!part_1)//if login is not open
    {
        part_1 = new Part_1();
    }else
    {
        close();
    }
    camera->stop(); //stoping camera
    part_1->show();

    //some feature must be disable for admin
    part_1->Enable_All_Features_For_Admin();

    close();
}

//setting camera for live preview and capturing
void Stock::setUpCamera()
{
    camera = new QCamera(this);
    capture = new QCameraImageCapture(camera);
    viewfinder = new QCameraViewfinder(this);

    ui->item_pic->setLayout(new QVBoxLayout);
    ui->item_pic->layout()->addWidget(viewfinder);

    connect(capture, &QCameraImageCapture::imageCaptured, this, &Stock::handle_imageCaptured);
};


//capturing codes
void Stock::handle_imageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);

    ui->item_pic->setPixmap(QPixmap::fromImage(preview));
}


//setting up camera for snap button
void Stock::on_btn_capture_clicked()
{
    if(capture->isReadyForCapture())
    {
        capture->capture();
    }
}


//inserting records into stock table
void Stock::on_btn_stock_save_clicked()
{
    //this constructor recieve "SMART-BRAIN" as the server name
    My_Database manage_stock("SMART-BRAIN");

    if(manage_stock.Open_Connection())
    {
        //setup query
        QSqlQuery stock_query(manage_stock.getDatabase());

        //declaring variables to hold the values on the UI
        QString batchCode = ui->show_batch_code->text();
        QString drugName = ui->show_drugName->text();

        QString drugType;
        int currentValue = ui->show_status_comboBox->currentIndex();

        if(currentValue == 1)
        {
            drugType = stock_query.value(currentValue).toString();
        }else if(currentValue == 2)
        {
            drugType = stock_query.value(currentValue).toString();
        }else if(currentValue == 3)
        {
            drugType = stock_query.value(currentValue).toString();
        }else if(currentValue == 4)
        {
            drugType = stock_query.value(currentValue).toString();
        }else
        {
            drugType = stock_query.value(currentValue).toString();
        }

        QString quantityStatus;
        int currentStatus = ui->quantityStatus_Box->currentIndex();

        if(currentStatus == 1)
        {
            quantityStatus = stock_query.value(currentStatus).toString();
        }else if(currentStatus == 2)
        {
            quantityStatus = stock_query.value(currentStatus).toString();
        }else if(currentStatus == 3)
        {
            quantityStatus = stock_query.value(currentStatus).toString();
        }

        int quantity = ui->show_quantity->text().toInt();
        float costprce = ui->show_cost_price->text().toFloat();
        float saleprice = ui->show_drugName->text().toFloat();

        QString recomend;
        if(ui->male_radioButton->isChecked())
        {
            recomend = ui->male_radioButton->text();
        }else if(ui->female_radioButton->isChecked())
        {
            recomend = ui->female_radioButton->text();
        }else if(ui->kids_radioButton->isChecked())
        {
            recomend = ui->kids_radioButton->text();
        }else
        {
            recomend = ui->adult_radioButton->text();
        }

        QDate mfg = QDate::fromString(ui->show_mfg_date->text(), "yyyy-MM-dd");
        QDate expdate = QDate::fromString(ui->show_expired->text(), "yyyy-MM-dd");
        //QString description = ui->description_edit->text();
        // Qimage drugPhoto = ui->item_pic->text();
        QString supplier = ui->show_supplier->text();
        QString contact = ui->show_contact->text();
        QDateTime Date_Added = QDateTime::currentDateTime();

        //prepare and bind query with the variables above
        stock_query.prepare("CALL MyStock :Id, :Name, :Status, :Quantity_Status, :Quantity, :Cost_Price, :Sole_Price, :User, :MFG_date, :Exp_date, :Description, :Photo, :Supplier, :Supplier_Contact");

        stock_query.bindValue("Id", batchCode);
        stock_query.bindValue("Name", drugName);
        stock_query.bindValue("Status", drugType);
        stock_query.bindValue("Quantity_Status", quantityStatus);
        stock_query.bindValue("Quantity", quantity);
        stock_query.bindValue("Cost_Price", costprce);
        stock_query.bindValue("Sole_Price", saleprice);
        stock_query.bindValue("User", recomend);
        stock_query.bindValue("MFG_date", mfg);
        stock_query.bindValue("Exp_date", expdate);
        //stock_query.bindValue("Description", description);
        //stock_query.bindValue("Photo", drugPhoto);
        stock_query.bindValue("Supplier", supplier);
        stock_query.bindValue("Supplier_Contact", contact);
        stock_query.bindValue("Date_Added", Date_Added);

        //checking for successful record insertion
        if(stock_query.exec() && stock_query.next())
        {
            int result = stock_query.value(0).toInt();
            if(result == 1 /*&& !batchCode.isEmpty() && !drugName.isEmpty() && !drugType.isEmpty() && !quantityStatus.isEmpty() && !quantity.isEmpty() && !costprce.isEmpty() && !saleprice.isEmpty() && !recomend.isEmpty() && !mfg.isEmpty() && !expdate.isEmpty() && !description.isEmpty() && !supplier.isEmpty() && !contact.isEmpty()*/)
            {
                QMessageBox::information(this, "Inserting Record", "successfully inserted record");
            }else
            {
                QMessageBox::critical(this, "Inserting Record", "fill all the require fields");
            }

        }else
        {
            QMessageBox::warning(this, "Query Execution", "query fail to execute!!");
        }

    }else
    {
        QMessageBox::critical(this, "Database Connectivity", "failed to open database connection !!");
    }

    manage_stock.Close_Database();
}


