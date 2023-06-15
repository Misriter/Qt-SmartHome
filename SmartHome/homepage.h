#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLCDNumber>
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QLabel>
#include <QComboBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QPainter>
#include <QPaintEvent>
#include <QLineEdit>
#include <QPixmap>
#include <QSerialPort>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QGroupBox>
#include <QPushButton>
#include "weatherdialog.h"

#define WEATHER "https://api.thinkpage.cn/v3/weather/now.json?key=SK0LJ8FI2TP0L-IsQ&location=%1&language=zh-Hans&unit=c"

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
    void isConnect();

public:
    QVBoxLayout *caltabvbl;
    QGridLayout *stateBar;
    QWidget *stateWidget;
    QLCDNumber *number;
    QTimer *timer;
    QDate *date;
    QLabel *timeLabel;
    QLabel *weatherLabel;
    QLabel *wifiLabel;
    QLabel *serialLabel;
    QLabel *space;
    QList<QString> cityList;//城市名列表
    QPushButton *change;
    QGroupBox *weatherBox;
    WeatherDialog *weatherdialog;

    QNetworkReply* getRequest(QString);//GET请求

public:
    QGroupBox *createBox();
    void paintEvent(QPaintEvent *event);

signals:

private slots:
    void ontimeoutslot();
    void indexchangedslot(int);//下拉框改变槽
    void finishedslot(); //HTTP响应结束槽
    void nowreadslot();  //请求实时天气读取槽
    void changeCity(QString);
    void onChangeCity();

};



#endif // HOMEPAGE_H
