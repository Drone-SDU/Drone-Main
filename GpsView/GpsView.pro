#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T14:10:29
#
#-------------------------------------------------

QT       += core gui webkitwidgets
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GpsView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui


DESTDIR  = $$PWD/bin

RESOURCES += \
    resource.qrc

INCLUDEPATH += /usr/include/opencv2 \
                /usr/include/opencv

LIBS            += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
                    /usr/lib/x86_64-linux-gnu/libopencv_core.so
