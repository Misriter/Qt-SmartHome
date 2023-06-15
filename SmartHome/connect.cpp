#include "connect.h"

Connect::Connect(QWidget *parent) : QWidget(parent)
{
    this->resize(800, 400);
    timer = new QTimer(this);
    timer->start(60000);

    sp = new QSerialPort(this);
    tb = new QTextBrowser();
    te = new QTextEdit();
    textArea=new QGridLayout();
    t[0]=new QLabel("接收:");
    t[1]=new QLabel("发送:");
    textArea->addWidget(t[0],0,0);
    textArea->addWidget(t[1],0,1);
    textArea->addWidget(tb,1,0);
    textArea->addWidget(te,1,1);
    vbl = new QVBoxLayout();
    textwidget = new QWidget();
    funcwidget = new QWidget();
    tcpwidget = new QWidget();
    tab = new QTabWidget();
    sgl = new QGridLayout();

    QList<QString> list1;
    list1<<"接口:"<<"波特率:"<<"字节宽度:"<<"校验位:"<<"停止位:"<<"信号:";

    for(int i=0;i<6;i++) {
        l[i] = new QLabel(list1[i]);
        l[i]->setMaximumSize(100, 30);
        l[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sgl->addWidget(l[i], 0, i);
    }

    for(int i=0;i<6;i++) {
        cb[i] = new QComboBox();
        cb[i]->setMaximumSize(100, 30);
        cb[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sgl->addWidget(cb[i], 1, i);
    }

    QList<QString> list2;
    list2<<"发送"<<"打开"<<"命令";
    for(int i=0;i<3;i++) {
        pb[i] = new QPushButton(list2[i]);
        pb[i]->setMaximumSize(100, 30);
        pb[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sgl->addWidget(pb[i], 2, 3+i);
    }

    LED1=new QLabel("LED1:");
    LED2=new QLabel("LED2:");
    DHT11=new QLabel("DHT11:");
    pb[0]->setEnabled(false);
    pb[2]->setEnabled(false);

    sgl->addWidget(LED1,2,0);
    sgl->addWidget(LED2,2,1);
    sgl->addWidget(DHT11,2,2);



    tgl = new QGridLayout();
    l_localip = new QLabel();
    l_client = new QLabel("客户端:");
    QLabel *porttips = new QLabel("端口号:");
    le_port = new QLineEdit("1234");//只能输入整数
    le_port->setValidator(new QIntValidator(le_port));
    le_port->setMaximumSize(100,30);
    pb_open = new QPushButton("打开监听");
    pb_open->setMaximumSize(100,30);
    pb_close = new QPushButton("关闭监听");
    pb_close->setMaximumSize(100,30);
    pb_send = new QPushButton("发送信息");
    pb_send->setMaximumSize(100,30);
    pb_close->setEnabled(false);
    pb_send->setEnabled(false);
    tgl->addWidget(l_localip, 0,0);
    tgl->addWidget(l_client, 1,0);
    tgl->addWidget(porttips, 0, 4);
    tgl->addWidget(le_port, 0, 5);
    tgl->addWidget(pb_send, 1,3);
    tgl->addWidget(pb_open, 1,4);
    tgl->addWidget(pb_close, 1, 5);

    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, list) {
        QList<QNetworkAddressEntry> entrylist = interface.addressEntries();
        foreach(QNetworkAddressEntry entry, entrylist) {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                address = entry.ip();
                addr = address.toString();
            }
        }
    }
    l_localip->setText("本地 IP：" + addr);
    server = new QTcpServer(this);
    client = new QTcpSocket(this);



    mqttwidget=new QWidget();
    mgl=new QGridLayout();
    conn=new QPushButton("连接OneNET");
    conn->setMaximumSize(100,30);
    discon=new QPushButton("断开连接");
    discon->setMaximumSize(100,30);
    send=new QPushButton("发送Json");
    send->setMaximumSize(100,30);
    space=new QLabel();
    ONE_LED1=new QLabel("LED1:");
    ONE_LED2=new QLabel("LED2:");
    ONE_DHT11=new QLabel("DHT11:");
    mgl->addWidget(space,0,2);
    mgl->addWidget(send,0,3);
    mgl->addWidget(conn,0,4);
    mgl->addWidget(discon,0,5);
    mgl->addWidget(ONE_LED1,0,0);
    mgl->addWidget(ONE_LED2,0,1);
    mgl->addWidget(ONE_DHT11,0,2);
    ct = new QMQTT::Client();
    send->setEnabled(false);
    discon->setEnabled(false);




    textwidget->setLayout(textArea);
    funcwidget->setLayout(sgl);
    tcpwidget->setLayout(tgl);
    mqttwidget->setLayout(mgl);
    QList <QString>tabList;
    tabList<<"串口"<<"TCP"<<"OneNET";
    tab->addTab(funcwidget, tabList[0]);
    tab->addTab(tcpwidget, tabList[1]);
    tab->addTab(mqttwidget, tabList[2]);
    vbl->addWidget(textwidget,1);
    vbl->addWidget(tab,1);
    this->setLayout(vbl);

    tb->setPlaceholderText("Message Received");
    te->setText("Message Send");

    connect(pb[0], SIGNAL(clicked()), this, SLOT(sendPB_slot()));
    connect(pb[1], SIGNAL(clicked()), this, SLOT(openPB_slot()));
    connect(pb[2], SIGNAL(clicked()), this, SLOT(order()));
    connect(sp, SIGNAL(readyRead()), this, SLOT(readSB_slot()));

    connect(pb_open, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(pb_close, SIGNAL(clicked()), this, SLOT(closeServer()));
    connect(pb_send, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));

    connect(ct, &Client::connected, this, &Connect::connslot);
    connect(ct, &Client::disconnected, this, &Connect::disconslot);
    connect(ct, &Client::received, this, &Connect::recvslot);
    connect(ct, &Client::error, this, &Connect::errorslot);
    connect(conn, &QPushButton::clicked, this, &Connect::connclicked);
    connect(discon, &QPushButton::clicked, this, &Connect::disconclicked);
    connect(send, &QPushButton::clicked, this, &Connect::sendclicked);

    scanSerial();
    initBaudItem();
    initByteItem();
    initParityItem();
    initStopItem();
    initSignalItem();
}

Connect::~Connect(){}

void Connect::scanSerial() {
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        cb[0]->addItem(info.portName());
    }
}

void Connect::initBaudItem() {
    QList<QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<
          "115200"<<"230400"<<"460800"<<"921600";
    for(int i=0; i<11; i++) {
        cb[1]->addItem(list[i]);
    }
    cb[1]->setCurrentIndex(7);
}

void Connect::initByteItem() {
    QList<QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for(int i=0; i<4; i++) {
        cb[2]->addItem(list[i]);
    }
    cb[2]->setCurrentIndex(3);
}

void Connect::initParityItem() {
    QList<QString> list;
    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    for(int i=0; i<5; i++) {
        cb[3]->addItem(list[i]);
    }
    cb[3]->setCurrentIndex(0);
}

void Connect::initStopItem() {
    QList<QString> list;
    list<<"1"<<"2";
    for(int i=0; i<2; i++) {
        cb[4]->addItem(list[i]);
    }
    cb[4]->setCurrentIndex(0);
}

void Connect::initSignalItem(){
    QList<QString> list;
    list<<"LED1 ON"<<"LED1 OFF"<<"LED2 ON"<<"LED2 OFF"<<"TOGGLE"<<"DHT11"<<"DHT11 ON"<<"DHT11 OFF";
    for(int i=0; i<8; i++) {
        cb[5]->addItem(list[i]);
    }
    cb[5]->setCurrentIndex(0);
}

void Connect::sendPB_slot() {
    QByteArray data = te->toPlainText().toUtf8();
    sp->write(data);
}

void Connect::openPB_slot() {
    if(pb[1]->text() == "Open") {
        sp->setPortName(cb[0]->currentText());
        sp->setBaudRate(cb[1]->currentText().toInt());

        switch(cb[2]->currentText().toInt()) {
        case 5:
            sp->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            sp->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            sp->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            sp->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }

        switch(cb[3]->currentIndex()) {
        case 0:
            sp->setParity(QSerialPort::NoParity);
            break;
        case 1:
            sp->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            sp->setParity(QSerialPort::OddParity);
            break;
        case 3:
            sp->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            sp->setParity(QSerialPort::MarkParity);
            break;
        default:
            break;
        }

        switch(cb[2]->currentText().toInt()) {
        case 1:
            sp->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            sp->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }

        sp->setFlowControl(QSerialPort::NoFlowControl);
        if(!sp->open(QIODevice::ReadWrite)) {
            QMessageBox::about(NULL, "Error", "Can't Open SerialPort");
        }
        else {
            for(int i=0; i<5; i++) {
                cb[i]->setEnabled(false);
            }

            pb[1]->setText("Close");
            pb[0]->setEnabled(true);
            pb[2]->setEnabled(true);

            serialConnect=true;
            orderled1off();
            orderled2off();
            orderdht11off();
        }
    }
    else {
        sp->close();
        for(int i=0; i<5; i++) {
            cb[i]->setEnabled(true);
        }

        pb[1]->setText("Open");
        pb[0]->setEnabled(false);
        pb[2]->setEnabled(false);
        serialConnect=false;
    }
}

void Connect::readSB_slot() {
    QByteArray buf = sp->readAll();
    tb->insertPlainText(QString(buf));

    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(QString(buf).toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() << jperr.errorString();
        return;
    }

    QJsonObject jo = jdoc.object();
    QString temp = jo["temp"].toString();
    QString humi = jo["humi"].toString();
    qDebug()<<"temp: "<<temp<<"humi: "<<humi;

    if(QSqlDatabase::contains("qt_sql_default_connection"))
          db = QSqlDatabase::database("qt_sql_default_connection");
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("th.db");
    }
    QSqlQuery q(db);

    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//获取当前时间

    if(!db.open())
        qDebug() << "Error: Failed to connect db";
    else
        qDebug() << "Succeed to connect db";
    if(!q.exec("insert into temphumi values(\"" +temp + "\",\"" +humi + "\",\""+dateTime+"\")")) {
        qDebug() << "Error: Failed to insert";
    }
    else {
        qDebug() << "Succeed to insert";
    }
    db.close();

//    tempList.clear();
//    humiList.clear();
//    tb->clear();

}

void Connect::order() {
    switch (cb[5]->currentIndex()) {
        case 0:orderled1on();break;
        case 1:orderled1off();break;
        case 2:orderled2on();break;
        case 3:orderled2off();break;
        case 4:orderledtoggle();break;
        case 5:orderdht11();break;
        case 6:orderdht11on();break;
        case 7:orderdht11off();break;
    }
}
void Connect::orderled1on() {
    sp->write("led1on\r\n");
    LED1->setText("LED1:ON");
    ONE_LED1->setText("LED1:ON");
}
void Connect::orderled1off() {
    sp->write("led1off\r\n");
    LED1->setText("LED1:OFF");
    ONE_LED1->setText("LED1:OFF");
}
void Connect::orderled2on() {
    sp->write("led2on\r\n");
    LED2->setText("LED2:ON");
    ONE_LED2->setText("LED2:ON");
}
void Connect::orderled2off() {
    sp->write("led2off\r\n");
    LED2->setText("LED2:OFF");
    ONE_LED2->setText("LED2:OFF");
}
void Connect::orderledtoggle() {
    sp->write("ledtoggle\r\n");

    if(LED1->text()=="LED1:ON"){
        LED1->setText("LED1:OFF");
        ONE_LED1->setText("LED1:OFF");
    }
    else{
        LED1->setText("LED1:ON");
        ONE_LED1->setText("LED1:ON");
    }

    if(LED2->text()=="LED2:ON"){
        LED2->setText("LED2:OFF");
        ONE_LED2->setText("LED2:OFF");
    }
    else{
        LED2->setText("LED2:ON");
        ONE_LED2->setText("LED2:ON");
    }

}
void Connect::orderdht11() {
    sp->write("dht11\r\n");
}
void Connect::orderdht11on() {
    connect(timer, &QTimer::timeout, this, &Connect::orderdht11);
    DHT11->setText("DHT11:ON");
    ONE_DHT11->setText("DHT11:ON");
}
void Connect::orderdht11off() {
    disconnect(timer, &QTimer::timeout, this, &Connect::orderdht11);
    DHT11->setText("DHT11:OFF");
    ONE_DHT11->setText("DHT11:OFF");
}



void Connect::onServerNewConnection() {
    client = server->nextPendingConnection();
    QString ip = client->peerAddress().toString();
    quint16 p = client->peerPort();
    l_client->setText("客户端:"+ip + ":" + QString::number(p));
    connect(client, SIGNAL(readyRead()), this, SLOT(onReceive()));
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this, SLOT(onChange(QAbstractSocket::SocketState)));
}

void Connect::startServer() {
    port = (quint16)le_port->text().toUInt();
    server->listen(address, port);
    //修改按钮状态，“打开监听”不可用，“关闭监听”可用
    pb_open->setEnabled(false);
    pb_close->setEnabled(true);
    pb_send->setEnabled(true);
}

void Connect::closeServer() {
    //关闭服务器监听，但是已有 Socket 还存在
    server->close();
    //如果有客户端则断开连接
    if(client != NULL) {
        if(client->state() == client->ConnectedState) {
            disconnect(client, 0, 0, 0);
            client->disconnectFromHost();
        }
    }
    //修改按钮状态，“打开监听”可用，“关闭监听”不可用
    pb_open->setEnabled(true);
    pb_close->setEnabled(false);
    pb_send->setEnabled(false);
    l_client->setText("客户端:");
}

void Connect::sendMsg() {
    if(client == NULL) return;
    //发送文本框的输入转成字节流输出
    if(client->state() == client->ConnectedState) {
        QString msg = te->toPlainText();
        client->write(msg.toUtf8().data());
    }
}

void Connect::onReceive() {
    QString msg = client->readAll();
    tb->append(msg);

    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(msg.toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() << jperr.errorString();
        return;
    }

    QJsonObject jo = jdoc.object();
    QString id = jo["id"].toString();
    QList<QString> list;
    list<<"LED1 ON"<<"LED1 OFF"<<"LED2 ON"<<"LED2 OFF"<<"TOGGLE"<<"DHT11 ON"<<"DHT11 OFF";
    int j=0;
    foreach(QString i,list){
        if(i==id){
            switch (j) {
                case 0:orderled1on();break;
                case 1:orderled1off();break;
                case 2:orderled2on();break;
                case 3:orderled2off();break;
                case 4:orderledtoggle();break;
                case 5:orderdht11on();break;
                case 6:orderdht11off();break;
            }
            return;
        }
        else
            j++;
    }
    QString temp = jo["temp"].toString();
    QString humi = jo["humi"].toString();
    qDebug()<<"temp: "<<temp<<"humi: "<<humi;

    if(QSqlDatabase::contains("qt_sql_default_connection"))
          db = QSqlDatabase::database("qt_sql_default_connection");
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("th.db");
    }
    QSqlQuery q(db);

    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//获取当前时间

    if(!db.open())
        qDebug() << "Error: Failed to connect db";
    else
        qDebug() << "Succeed to connect db";
    if(!q.exec("insert into temphumi values(\"" +temp + "\",\"" +humi + "\",\""+dateTime+"\")")) {
        qDebug() << "Error: Failed to insert";
    }
    else {
        qDebug() << "Succeed to insert";
    }
    db.close();

}

void Connect::onChange(QAbstractSocket::SocketState st) {
    //客户端连接状态改变，断开则关闭监听
    switch(st) {
    case QAbstractSocket::UnconnectedState:
        l_client->setText("客户端:");//客户端断开则清楚客户端套接字显示
        break;
    case QAbstractSocket::ConnectingState:
        break;
    case QAbstractSocket::ConnectedState:
        break;
    case QAbstractSocket::ClosingState:
        break;
    case QAbstractSocket::ListeningState:
        break;
    case QAbstractSocket::BoundState:
        break;
    default:
        break;
    }
}

//云端连接成功槽函数
void Connect::connslot() {
    qDebug() << "Connect to OneNET Done\n";
    send->setEnabled(true);
    conn->setEnabled(false);
    discon->setEnabled(true);
}

//云端断开后槽函数，如果是非正常断开再重连
void Connect::disconslot() {
    if(isover) {
        qDebug() << "Disconnect from OneNET Done\n";
        send->setEnabled(false);
        conn->setEnabled(true);
        discon->setEnabled(false);
    }
    else {
        ct->connectToHost();
    }
}

//云端信息接收到槽函数，在文本浏览框中显示
void Connect::recvslot(const QMQTT::Message& message) {
    QString str = QString::fromUtf8(message.payload());
    tb->append(str + "\n");
    QList<QString> list;
    list<<"LED1 ON"<<"LED1 OFF"<<"LED2 ON"<<"LED2 OFF"<<"TOGGLE"<<"DHT11 ON"<<"DHT11 OFF";
    int j=0;
    foreach(QString i,list){
        if(i==str){
            switch (j) {
                case 0:orderled1on();break;
                case 1:orderled1off();break;
                case 2:orderled2on();break;
                case 3:orderled2off();break;
                case 4:orderledtoggle();break;
                case 5:orderdht11on();break;
                case 6:orderdht11off();break;
            }
            sendOrder();
            break;
        }
        else
            j++;
    }
}

//连接错误槽函数，error 仅是错误类型编号
void Connect::errorslot(const QMQTT::ClientError error) {
    qDebug() << "error: " << error;
}

//点击连接按钮后执行云端接入
void Connect::connclicked() {
    isover = false;//主动断开标志，点击断开按钮会赋值为 true
    QHostAddress host(ONE_IP);
    ct->setKeepAlive(120);
    ct->setHost(host); //设置 EMQ 代理服务器 IP
    ct->setPort(ONE_PORT);//设置 EMQ 代理服务器端口号
    ct->setClientId(ONE_DEV);//onenet 设备 ID
    ct->setUsername(ONE_PRO);//onenet 的产品 ID
    ct->setPassword(ONE_AUTH);//设置一个客户端密码
    ct->cleanSession(); //清除缓存
    //设置 mqtt 版本,onenet 平台规定为 mqtt3.1.1 版本
    ct->setVersion(QMQTT::MQTTVersion::V3_1_1);
    ct->connectToHost(); //连接 OneNET 服务器
    qDebug() << "Ready to Connect to OneNET Done";
    orderled1off();
    orderled2off();
    orderdht11off();
}

//点击断开按钮后，执行断联云端，并置位标志，表示主动断开
void Connect::disconclicked() {
    if(ct->isConnectedToHost()) {
        isover = true;
        ct->disconnectFromHost();
        qDebug() << "Ready to Disconnect from OneNET";
    }
}

void Connect::sendclicked() {
    QString text = te->toPlainText();
    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(text.toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() << jperr.errorString();
        QMessageBox::critical(this, "Warning","输入Json字符串!");
        return;
    }

    QJsonObject jo = jdoc.object();
    QString temp = jo["temp"].toString();
    QString humi = jo["humi"].toString();

    char msgJson[75];
    char pack[] = ",;temp,%.2f;humi,%.2f";//OneNET 数据格式 5
    //获取输入框值转换为浮点数

    float tempNum = temp.toFloat();
    float humiNum = humi.toFloat();
    //拼接为 OneNET 格式 5 数据包
    sprintf(msgJson, pack, tempNum,humiNum);
    //组合 MQTT 报文，附加类型号 0x05 和数据包长度
    QByteArray buf;
    buf.resize(strlen(msgJson)+4);
    buf[0] = char(0x05);
    buf[1] = char(strlen(msgJson) >> 8);
    buf[2] = char(strlen(msgJson) & 0xff);
    //数据包插入报文
    for(int i=0; i<strlen(msgJson); i++) buf[i+3] = msgJson[i];
    buf[3 + strlen(msgJson)] = 0;//报文结尾加 0
    qDebug() << "buf:" << buf;
    //发布，OneNET 默认主题$dp
    if(ct->isConnectedToHost()) {
        QMQTT::Message msg(18, "$dp", buf);
        ct->publish(msg);
    }
    qDebug()<<buf;
}

void Connect::sendOrder() {
    char msgJson[75];
    char pack[] = ",;LED1,%d;LED2,%d;DHT11,%d";

    int led1;
    int led2;
    int dht11;

    if(LED1->text()=="LED1:ON")
        led1=1;
    else
        led1=0;
    if(LED2->text()=="LED2:ON")
        led2=1;
    else
        led2=0;
    if(DHT11->text()=="DHT11:ON")
        dht11=1;
    else
        dht11=0;

    //拼接为 OneNET 格式 5 数据包
    sprintf(msgJson, pack, led1,led2,dht11);
    //组合 MQTT 报文，附加类型号 0x05 和数据包长度
    QByteArray buf;
    buf.resize(strlen(msgJson)+4);
    buf[0] = char(0x05);
    buf[1] = char(strlen(msgJson) >> 8);
    buf[2] = char(strlen(msgJson) & 0xff);
    //数据包插入报文
    for(int i=0; i<strlen(msgJson); i++) buf[i+3] = msgJson[i];
    buf[3 + strlen(msgJson)] = 0;//报文结尾加 0
    qDebug() << "buf:" << buf;
    //发布，OneNET 默认主题$dp
    if(ct->isConnectedToHost()) {
        QMQTT::Message msg(18, "$dp", buf);
        ct->publish(msg);
    }
    qDebug()<<buf;
}
