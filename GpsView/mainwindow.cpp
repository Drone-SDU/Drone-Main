#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebChannel>
#include <QDebug>


#include <math.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_view(new QWebEngineView(this))
{
    ui->setupUi(this);


    timer   = new QTimer(this);
    imag    = new QImage();         // 初始化

    server_ = new Ui::Server(this);
    dock_server_ = new QDockWidget("Info", this);
    dock_server_->setWidget(server_);
    //addDockWidget(Qt::LeftDockWidgetArea, dock_server_);
    dock_server_->setFloating(1);

    QString strPath = "file://";
    strPath += qApp->applicationDirPath();
    strPath += "/index.html";
    qDebug() << strPath;

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("MainWindow", this);
    ui->webView->page()->setWebChannel(channel);
    ui->webView->page()->load(QUrl(strPath));

    timer_1 = new QTimer(this);
    timer->start(100);

    timer_2 = new QTimer(this);
   //
    timer_2->start(1000);


    cam = cv::VideoCapture(2);//打开摄像头，从摄像头中获取视频
    if(!cam.isOpened())
        std::cerr << "Can't open camera!" <<std::endl;;

    timer->start(33);              // 开始计时，超时则发出timeout()信号

    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
   // connect(ui->pic, SIGNAL(clicked()), this, SLOT(takingPictures()));
    //connect(ui->closeCam, SIGNAL(clicked()), this, SLOT(closeCamara()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeCountsFunction()
{
    ui->lineEditLng->setText(QString::number(server_->jsonGPS["longitude"].toDouble()));
    //qDebug() <<"\n->"<< server_->jsonGPS["altitude"].toString();
    ui->lineEditLat->setText(QString::number(server_->jsonGPS["latitude"].toDouble()));
    ui->Alt->setText(QString::number(server_->jsonGPS["altitude"].toDouble()));

    ui->VelH->setText(QString::number(sqrt(pow(server_->jsonGPS["velocityY"].toDouble(),2)+pow(server_->jsonGPS["velocityX"].toDouble(), 2))));
    ui->VelV->setText(QString::number(server_->jsonGPS["velocityZ"].toDouble()));

    ui->Bat->setText(QString::number(server_->jsonBattery["BatteryEnergyRemainingPercent"].toDouble()));
}

void MainWindow::QtTest()
{
    qDebug() << "Qt Hello world!";
}

void MainWindow::on_pushButton_clicked()
{
    connect(timer_1,SIGNAL(timeout()),this,SLOT(timeCountsFunction()));
    connect(timer_2,SIGNAL(timeout()),this,SLOT(callJava()));
}

void MainWindow::callJava()
{
    QString strJs_ = "myFunction(%1, %2, ";
    strJs_ += QString::number(server_->jsonGPS["yaw"].toDouble(), 10, 1);
    strJs_ += ")";
    QString strJs = strJs_
            .arg(ui->lineEditLng->text().toDouble()*0.01+116)
            .arg(ui->lineEditLat->text().toDouble()*0.01+29);
    ui->webView->page()->runJavaScript(strJs);

    qDebug() << "\n->" <<strJs;
}



/*********************************
********* 读取摄像头信息 ***********
**********************************/
void MainWindow::readFarme()
{
    cam >> frame;// 从摄像头中抓取并返回每一帧
    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage image = QImage(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888).rgbSwapped();
      ui->label_3->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上

}

/*************************
********* 拍照 ***********

void MainWindow::takingPictures()
{
    frame = cvQueryFrame(cam);// 从摄像头中抓取并返回每一帧

    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();

    ui->label_2->setPixmap(QPixmap::fromImage(image));  // 将图片显示到label上
}
**************************/
/*******************************
***关闭摄像头，释放资源，必须释放***
********************************/
void MainWindow::closeCamara()
{
    timer->stop();         // 停止读取数据。

}
