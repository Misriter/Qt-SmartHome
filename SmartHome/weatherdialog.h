#ifndef WEATHERDIALOG_H
#define WEATHERDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QDebug>
class WeatherDialog : public QDialog
{
    Q_OBJECT
public:
    WeatherDialog();

signals:
    void closeDialog(QString);

public slots:
    void changeCity();

public:
    QPushButton *submit;
    QLineEdit *line;
    QComboBox *comboBox;  //选择城市的下拉框
    QList<QString> cityList;//城市名列表
    QString chosenCity;

};
#endif // WEATHERDIALOG_H
