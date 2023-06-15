#ifndef CONNECT_H
#define CONNECT_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QHostAddress>
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
#include <QTimer>

#define ONE_IP "183.230.40.39" //服务器 IP
#define ONE_PORT 6002 //服务器端口
#define ONE_AUTH "mis" //鉴权信息，即密码
#define ONE_DEV "1095661380"//设备 ID
#define ONE_PRO "550170" //产品 ID
using QMQTT::Client;

class Connect : public QWidget
{
    Q_OBJECT
public:
    explicit Connect(QWidget *parent = nullptr);
    ~Connect();

    void scanSerial();    //scan serial ports
    void initBaudItem();  //combobox items for baudrate options
    void initByteItem();  //combobox items for bytewidth options
    void initParityItem();//combobox items for parity options
    void initStopItem();  //combobox items for stopbits options
    void initSignalItem();
    void sendOrder();

public:
    QSerialPort *sp;    //serial port pointer
    bool serialConnect=false;
    QTextBrowser *tb;   //used to show received text
    QTextEdit *te;      //used to input sent text
    QPushButton *pb[3]; //buttons include "Open" "Send"
    QComboBox *cb[6];   //combobox for serial options
    QLabel *t[2];
    QLabel *l[6];       //label for options' tip
    QVBoxLayout *vbl;   //main layout
    QGridLayout *sgl;    //bottom layout
    QGridLayout *textArea;
    QWidget *textwidget;
    QWidget *funcwidget;
    QTabWidget *tab; //选项页控件

    QWidget *tcpwidget;
    QGridLayout *tgl;
    QLineEdit *le_port; //端口号输入框（只能输入整数）
    QPushButton *pb_open; //开启监听按钮
    QPushButton *pb_close;//关闭监听按钮
    QPushButton *pb_send; //发送数据按钮
    QLabel *l_client; //显示接入客户端套接字的标签
    QLabel *l_localip; //显示本地 IP 的标签
    QHostInfo info; //本地主机信息
    QHostAddress address; //本地主机地址
    QTcpServer *server; //本地 TCP Server
    QTcpSocket *client; //接入 Client
    QString addr; //本地 IP 字符串
    quint16 port; //本地 TCP 端口
    QList<QString> tempList;
    QList<QString> humiList;

    QWidget *mqttwidget;
    QGridLayout *mgl;
    QPushButton *conn; //连接云平台按钮
    QPushButton *discon;//断开连接按钮
    QPushButton *send; //向云平台发送信息按钮
    Client *ct; //MQTT 客户端
    bool isover; //表示结束连接
    QLabel *space;

    QLabel *LED1;
    QLabel *LED2;
    QLabel *ONE_LED1;
    QLabel *ONE_LED2;
    QLabel *DHT11;
    QLabel *ONE_DHT11;
    QTimer *timer;

    QSqlDatabase db;

private slots:
    void sendPB_slot();   //send button slot func
    void openPB_slot();   //open button slot func
    void readSB_slot();   //serial receive slot func
    void order();
    void orderled1on();
    void orderled1off();
    void orderled2on();
    void orderled2off();
    void orderledtoggle();
    void orderdht11();
    void orderdht11on();
    void orderdht11off();

    void onServerNewConnection();//有新客户端接入
    void startServer(); //开启服务器监听
    void closeServer(); //关闭服务器监听
    void sendMsg(); //向客服端发送信息
    void onReceive(); //接收到客户端信息
    void onChange(QAbstractSocket::SocketState st);//连接状态改变

    void connslot(); //连接回调槽
    void disconslot(); //断开回调槽
    void recvslot(const QMQTT::Message&); //云平台接收槽
    void errorslot(const QMQTT::ClientError);//连接错误槽
    void connclicked(); //连接云平台按钮槽
    void disconclicked();//断开云平台按钮槽
    void sendclicked(); //向云平台发送按钮槽

};

#endif // CONNECT_H
