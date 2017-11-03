#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include <QWebEngineView>
#include "server.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/video.hpp>

namespace Ui {
class MainWindow;
class Server;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void QtTest();
private slots:
    void on_pushButton_clicked();
    void timeCountsFunction();
    void callJava();
private:
    Ui::MainWindow *ui;

    Ui::Server *server_;
    QDockWidget* dock_server_;

    QTimer* timer_1;
    QTimer* timer_2;


private slots:
    void readFarme();       // 读取当前帧信息
    void closeCamara();     // 关闭摄像头。

private:
    QTimer    *timer;
    QImage    *imag;
    cv::VideoCapture cam;
    cv::Mat  frame;
};

#endif // MAINWINDOW_H
