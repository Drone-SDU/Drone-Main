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

#INCLUDEPATH += /usr/include/opencv2 \
#                /usr/include/opencv
INCLUDEPATH += /usr/local/include

#LIBS            += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
#                    /usr/lib/x86_64-linux-gnu/libopencv_core.so
LIBS            += /usr/local/lib/libopencv_highgui.dylib \
                    /usr/local/lib/libopencv_core.dylib \
                     /usr/local/lib/libopencv_video.dylib
