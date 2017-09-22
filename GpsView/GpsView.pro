#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T14:10:29
#
#-------------------------------------------------
# webkitwidgets
QT += core gui webenginewidgets
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

INCLUDEPATH += /usr/local/include/opencv2 \
                /usr/local/include/opencv

LIBS            += /usr/local/lib/libopencv_highgui.so \
                    /usr/local/lib/libopencv_core.so \
                     /usr/local/lib/libopencv_videoio.so
