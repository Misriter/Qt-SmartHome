QT       += core gui network qmqtt serialport sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH +="/home/pi/Downloads/qmqtt-master/include"

SOURCES += \
    main.cpp \
    myslide.cpp \
    smarthome.cpp \
    homepage.cpp \
    weatherdialog.cpp \
    connect.cpp \
    chart.cpp

HEADERS += \
    myslide.h \
    smarthome.h \
    homepage.h \
    weatherdialog.h \
    connect.h \
    chart.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resouces.qrc

