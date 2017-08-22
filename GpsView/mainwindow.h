#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include "server.h"

#include <highgui.h>  //包含opencv库头文件
#include <cv.h>

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
         // 打开摄像头
    void readFarme();       // 读取当前帧信息
    void closeCamara();     // 关闭摄像头。
    void takingPictures();  // 拍照

private:
    QTimer    *timer;
    QImage    *imag;
    CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数
    IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像

    void openCamara();
};

#endif // MAINWINDOW_H
