#include "chart.h"
#include <QHeaderView>

Chart::Chart(QWidget *parent) : QWidget(parent)
{
    this->resize(800, 400);

    hbl=new QHBoxLayout();
    dialog = new QDialog(this);

    lgb=new QGroupBox();
    tw = new QTableWidget();
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tw->resizeRowsToContents();
    tw->setColumnCount(3);
    tw->setHorizontalHeaderLabels(QStringList()<<"temp"<<"humi"<<"time");
    flushTable();
    QHBoxLayout *autohbl = new QHBoxLayout;
    autohbl->addWidget(tw);
    lgb->setLayout(autohbl);

    rgb = new QGroupBox();
    QVBoxLayout *vbl = new QVBoxLayout();
    vtw=new QTableWidget();
    vtw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vtw->resizeRowsToContents();
    vtw->setColumnCount(3);
    vtw->setHorizontalHeaderLabels(QStringList()<<"temp"<<"humi"<<"time");
    lw = new QListWidget();
    QGridLayout *gl = new QGridLayout();
    searchTemp = new QPushButton("搜索温度");
    searchHumi = new QPushButton("搜索湿度");
    searchTime = new QPushButton("搜索时间");
    createChart = new QPushButton("生成折线图");
    le = new QLineEdit();
    le->setPlaceholderText("输入要查找的内容...");
    le->setClearButtonEnabled(true);
    gl->addWidget(searchTemp,0,0);
    gl->addWidget(searchHumi,0,1);
    gl->addWidget(searchTime,1,0);
    gl->addWidget(createChart,1,1);
    vbl->addWidget(vtw, 2);
    vbl->addWidget(le, 1);
    vbl->addLayout(gl, 1);
    rgb->setLayout(vbl);

    connect(searchTemp, SIGNAL(clicked()), this, SLOT(findTable()));
    connect(searchHumi, SIGNAL(clicked()), this, SLOT(findTable()));
    connect(searchTime, SIGNAL(clicked()), this, SLOT(findTable()));
    connect(createChart, SIGNAL(clicked()), this, SLOT(showChart()));

    hbl->addWidget(lgb,2);
    hbl->addWidget(rgb,1);
    this->setLayout(hbl);

}

void Chart::flushTable() {
    disconnect(tw, SIGNAL(cellChanged(int,int)), 0, 0);
    if(QSqlDatabase::contains("qt_sql_default_connection"))
          db = QSqlDatabase::database("qt_sql_default_connection");
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("th.db");
    }
    QSqlQuery q(db);
    if(!db.open())
        qDebug() << "Error: Failed to connect db";
    else
        qDebug() << "Succeed to connect db";
    tw->setRowCount(0);
    scanSql(&q);
    q.first();
    do {
        tw->setRowCount(tw->rowCount() + 1);
        for(int i=0; i<3; i++)
            tw->setItem(tw->rowCount()-1, i, new QTableWidgetItem(q.value(i).toString()));
    } while(q.next());
    db.close();
}

void Chart::findTable(){
    qint32 count = tw->rowCount();
    bool finded = false;
    vtw->setRowCount(0);
    int kind;
    QPushButton *obj = qobject_cast<QPushButton *>(sender());
    if(obj==searchTemp)
        kind=0;
    else if(obj==searchHumi)
        kind=1;
    else if(obj==searchTime)
        kind=2;
    if(count > 0) {
        for(qint32 i=0; i<count; i++) {
            QString text = tw->item(i, kind)->text();
            if(kind!=2){
                if(text == le->text()) {
                    tw->selectRow(i);
                    vtw->setRowCount(vtw->rowCount() + 1);
                    for(int j=0; j<3; j++)
                        vtw->setItem(vtw->rowCount()-1, j, new QTableWidgetItem(tw->item(i, j)->text()));
                    finded=true;
                }
            }
            else {
                QDateTime time=QDateTime::fromString(text,"yyyy-MM-dd hh:mm:ss");
                QDateTime selectTime=QDateTime::fromString(le->text(),"yyyy-MM-dd hh:mm:ss");
                if(time >= selectTime) {
                    tw->selectRow(i);
                    vtw->setRowCount(vtw->rowCount() + 1);
                    for(int j=0; j<3; j++)
                        vtw->setItem(vtw->rowCount()-1, j, new QTableWidgetItem(tw->item(i, j)->text()));
                    tempList.append(tw->item(i, 0)->text().toInt());
                    humiList.append(tw->item(i, 1)->text().toInt());
                    timeList.append(tw->item(i, 2)->text());
                    finded=true;
                }
            }

        }
        if(finded == false)
            QMessageBox::information(this, "失败", "未找到");
    }
}


void Chart::scanSql(QSqlQuery *q) {
    if(!q->exec("select * from temphumi")) {
        qDebug() << "Error: " << q->lastError();
    }
    else {
    while(q->next()) {
        for(int i=0; i<7; i++)
            qDebug() << q->value(i).toString();
        }
    }
}

void Chart::showChart() {
    if(tempList.size()==0||humiList.size()==0){
        QMessageBox::information(this, "失败", "先搜索时间再创建折线图！");
        return;
    }
    dialog->close();
    dialog->setWindowTitle("Chart Dialog");
    QLineSeries *tempSeries = new QLineSeries();
    QLineSeries *humiSeries = new QLineSeries();
    tempSeries->setName("temp");
    humiSeries->setName("humi");
    for(int i=0; i<tempList.size(); i++){
        tempSeries->append(i, tempList[i]);
        humiSeries->append(i, humiList[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(tempSeries);
    chart->addSeries(humiSeries);
    chart->createDefaultAxes();
    chart->setTitle("temphumi");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->createDefaultAxes();
    chartView->chart()->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

    chartView->setParent(dialog);
    chartView->resize(800, 600);
    dialog->show();
    tempList.clear();
    humiList.clear();
    timeList.clear();
}
