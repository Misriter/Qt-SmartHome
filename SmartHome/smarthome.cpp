#include "smarthome.h"

SmartHome::SmartHome(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 400);
    this->setMinimumSize(800, 400);
    this->setWindowTitle("Platform");

    slide = new MySlide(this);
    slide->resize(this->size());

    homepage = new HomePage(this);
    slide->addPage(homepage);

    connectpage = new Connect(this);
    slide->addPage(connectpage);

    chart = new Chart(this);
    slide->addPage(chart);

}

SmartHome::~SmartHome()
{
}

void SmartHome::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev);
    slide->resize(this->size());
}

