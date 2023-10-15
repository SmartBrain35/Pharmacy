#ifndef PART_1_H
#define PART_1_H

//desktop view setup
#include <QWidget>
#include <QDesktopWidget>

//time and date
#include <ctime>
#include <QTimer>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <QDebug>

//text to speech
#include <QTextToSpeech>
#include <QLoggingCategory>

//text browsing
#include <QFile>
#include <QTextStream>
#include <QStringListModel>
#include <QCompleter>

namespace Ui {
class Part_1;
}

class Part_1 : public QWidget
{
    Q_OBJECT

public:
    explicit Part_1(QWidget *parent = nullptr);
    ~Part_1();

    //slot functions declarations
private slots:
    void on_btn_exit_clicked();

    void on_btn_logout_clicked();

    void on_btn_Stock_clicked();

    void on_btn_settings_clicked();

    void on_btn_add_clicked();

    void on_btn_search_user_clicked();

    void on_browse_user_photo_clicked();

    void on_btn_sales_clicked();

    void on_btn_browse_logo_clicked();

    void on_btn_save_clicked();

    void on_my_settings_tabs_currentChanged(int index);

public Q_SLOT:   //function declarations
    void updateTime();//slot to update time
    void DisableFeaturesForEmployee(); //slot to disable features
    void Enable_All_Features_For_Admin();
    void Tabs_View();

    void UpdateSearchDrugOutcome(const QString &text);//text browsing slot


    void speak();
    void stop();

    void setRate(int);
    void setPitch(int);
    void setVolume(int volume);

    void stateChanged(QTextToSpeech::State state);
    void engineSelected(int index);
    void languageSelected(int language);
    void voiceSelected(int index);
    void localeChanged(const QLocale &locale);



private:
    Ui::Part_1 *ui;

    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;

    //text browsing class
    QStringList drugsData;
};

#endif // PART_1_H
