#include "smarthome.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartHome w;
    w.show();
    return a.exec();
}
