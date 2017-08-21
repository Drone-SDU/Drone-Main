#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QWebFrame>
#include <QDebug>


#include <math.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server_ = new Ui::Server(this);
    dock_server_ = new QDockWidget("Info", this);
    dock_server_->setWidget(server_);
    //addDockWidget(Qt::LeftDockWidgetArea, dock_server_);
    dock_server_->setFloating(1);

    QString strPath = "file://";
    strPath += qApp->applicationDirPath();
    strPath += "/index.html";
    qDebug() << strPath;
    ui->webView->load(QUrl(strPath));
//    ui->webView->load(QUrl("https://hao.360.cn/?wd_xp1"));
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("MainWindow", this);

    timer = new QTimer(this);

       timer->start(100);

    timer_2 = new QTimer(this);
   //
    timer_2->start(1000);

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
    connect(timer,SIGNAL(timeout()),this,SLOT(timeCountsFunction()));
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
    ui->webView->page()->mainFrame()->evaluateJavaScript(strJs);

    qDebug() << "\n->" <<strJs;
}
