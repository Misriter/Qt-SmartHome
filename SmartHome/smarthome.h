#ifndef SMARTHOME_H
#define SMARTHOME_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include "myslide.h"
#include "homepage.h"
#include "connect.h"
#include "chart.h"

class SmartHome : public QMainWindow
{
    Q_OBJECT

public:
    SmartHome(QWidget *parent = nullptr);
    ~SmartHome();

private:
    MySlide *slide;
    HomePage *homepage;
    Connect *connectpage;
    Chart *chart;
    void resizeEvent(QResizeEvent *ev);
};
#endif // SMARTHOME_H
