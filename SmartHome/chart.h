#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QtQmqtt/qmqtt.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QtCharts>
#include <QDateTime>

class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);
    void scanSql(QSqlQuery *q);

public:
    QSqlDatabase db;
    QHBoxLayout *hbl;
    QGroupBox *lgb;
    QGroupBox *rgb;
    QTextBrowser *tb;
    QTableWidget *tw;
    QTableWidget *vtw;
    QListWidget *lw;
    QLineEdit *le;
    QDialog *dialog;
    QPushButton *searchTemp;
    QPushButton *searchHumi;
    QPushButton *searchTime;
    QPushButton *createChart;
    QList<int> tempList;
    QList<int> humiList;
    QList<QString> timeList;


signals:

public slots:
    void flushTable();
    void findTable();
    void showChart();
};

#endif // CHART_H
