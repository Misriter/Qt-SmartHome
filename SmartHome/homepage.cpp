#include "homepage.h"

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    this->resize(800, 400);
    this->setMinimumSize(800, 400);

    caltabvbl = new QVBoxLayout();
    weatherBox=createBox();
    stateBar=new QGridLayout();
    stateWidget=new QWidget();

    cityList<<"tianjin"<<"beijing"<<"shanghai"<<"chongqing";

    QFont font;
    font.setPixelSize(30);

    number = new QLCDNumber();
    number->setSegmentStyle(QLCDNumber::Flat);
    number->setDigitCount(8);
    number->setFrameStyle(0);
    QDateTime curDateTime=QDateTime::currentDateTime();
    number->display(curDateTime.time().toString("HH:mm:ss"));
    timeLabel=new QLabel();
    timeLabel->setText(curDateTime.toString("yyyy-MM-dd ddd"));
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignHCenter);

    timer = new QTimer(this);
    timer->start(1000);

    wifiLabel=new QLabel();
    serialLabel=new QLabel();
    stateBar->addWidget(wifiLabel,0,0);
    stateBar->addWidget(serialLabel,0,1,0,6);
    stateWidget->setLayout(stateBar);

    connect(timer, &QTimer::timeout, this, &HomePage::ontimeoutslot);
    connect(timer, &QTimer::timeout, this, &HomePage::isConnect);

    caltabvbl->addWidget(stateWidget, 1);
    caltabvbl->addWidget(number, 2);
    caltabvbl->addWidget(timeLabel, 1);
    caltabvbl->addWidget(weatherBox, 5);

    this->setLayout(caltabvbl);
    indexchangedslot(0);
}

HomePage::~HomePage() {}

void HomePage::ontimeoutslot() {
    QDateTime curDateTime=QDateTime::currentDateTime();
    number->display(curDateTime.toString("HH:mm:ss"));
}

void HomePage::indexchangedslot(int index) {
    QString weatherNow = WEATHER;
    QString nowurl = weatherNow.arg(cityList[index]);
    qDebug() << "now url: " << nowurl;
    QNetworkReply *reply = getRequest(nowurl);
    connect(reply, &QNetworkReply::readyRead, this, &HomePage::nowreadslot);
}

QNetworkReply* HomePage::getRequest(QString url) {
    QNetworkRequest request;
    QNetworkAccessManager *manager;

    manager = new QNetworkAccessManager(this);
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, &HomePage::finishedslot);
    return reply;
}

void HomePage::finishedslot() {
    QNetworkReply *reply = (QNetworkReply *)sender();
    reply->deleteLater();
}

void HomePage::nowreadslot() {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray bytes = reply->readAll();
    QString data = QString::fromUtf8(bytes);

    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(data.toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() << jperr.errorString();
        return;
    }
    QJsonObject jobject = jdoc.object();

    QJsonArray jaresults = jobject["results"].toArray();
    QJsonObject joresult = jaresults[0].toObject();
    QJsonObject jolocation = joresult["location"].toObject();
    QString city = jolocation["name"].toString();
    QJsonObject jonow = joresult["now"].toObject();
    QString text = jonow["text"].toString();
    QString temp = jonow["temperature"].toString();

    weatherLabel->setText(city+"   "+text+"   "+temp+"℃");
}

QGroupBox *HomePage::createBox(){
    QGroupBox *gb = new QGroupBox();
    QHBoxLayout *hbl = new QHBoxLayout;
    QFont font;
    font.setPixelSize(30);

    weatherLabel=new QLabel();
    weatherLabel->setFont(font);
    change=new QPushButton("切换城市");
    space=new QLabel();
    hbl->setAlignment(Qt::AlignTop);

    hbl->addWidget(space,3);
    hbl->addWidget(change,1);
    hbl->addWidget(weatherLabel,6);
    gb->setLayout(hbl);

    connect(change, &QPushButton::clicked, this, &HomePage::onChangeCity);
    return gb;
}

void HomePage::onChangeCity(){
    weatherdialog = new WeatherDialog();
    QObject::connect(weatherdialog, SIGNAL(closeDialog(QString)), this, SLOT(changeCity(QString)));
    weatherdialog->exec();
}

void HomePage::changeCity(QString currentCity){
    QString weatherNow = WEATHER;
    QString nowurl = weatherNow.arg(currentCity);
    qDebug() << "now url: " << nowurl;
    QNetworkReply *reply = getRequest(nowurl);
    connect(reply, &QNetworkReply::readyRead, this, &HomePage::nowreadslot);
}

void HomePage::isConnect(){
    update();
    QNetworkConfigurationManager mgr;
    if(!mgr.isOnline()){
        wifiLabel->setPixmap(QPixmap(":/icons/wifi2.png"));
    }
    else{
        wifiLabel->setPixmap(QPixmap(":/icons/wifi1.png"));
    }


}

void HomePage::paintEvent(QPaintEvent *event){
    QPainter paint(this);
    paint.drawLine(QPoint(0, 50), QPoint(800, 50));
}
