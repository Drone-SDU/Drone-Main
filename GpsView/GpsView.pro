# Project created by QtCreator 2016-09-28T14:10:29
#
#-------------------------------------------------

# webkitwidgets
# webkitwidgets
QT += core gui webenginewidgets
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GpsView
TEMPLATE = app


RESOURCES += \
    resource.qrc
SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui


DESTDIR  = $$PWD/bin


#INCLUDEPATH += /usr/include/opencv2 \
#                /usr/include/opencv
INCLUDEPATH += /usr/local/include/opencv2 \
                /usr/local/include/opencv

#LIBS            += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
#                    /usr/lib/x86_64-linux-gnu/libopencv_core.so
LIBS            += /usr/local/lib/libopencv_highgui.so \
                    /usr/local/lib/libopencv_core.so \
                     /usr/local/lib/libopencv_videoio.so
