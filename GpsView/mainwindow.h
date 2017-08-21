#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include "server.h"

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

    QTimer* timer;
    QTimer* timer_2;
};

#endif // MAINWINDOW_H
