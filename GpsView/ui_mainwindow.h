/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QLabel *label;
    QLineEdit *lineEditLat;
    QLabel *label_2;
    QLineEdit *lineEditLng;
    QLabel *label_4;
    QLabel *Alt;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_10;
    QLabel *Bat;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLabel *VelH;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLabel *VelV;
    QLabel *label_12;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1062, 439);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setBaseSize(QSize(0, 0));
        label_3->setLineWidth(1);

        horizontalLayout_3->addWidget(label_3);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(groupBox_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEditLat = new QLineEdit(groupBox);
        lineEditLat->setObjectName(QStringLiteral("lineEditLat"));

        horizontalLayout->addWidget(lineEditLat);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEditLng = new QLineEdit(groupBox);
        lineEditLng->setObjectName(QStringLiteral("lineEditLng"));

        horizontalLayout->addWidget(lineEditLng);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        Alt = new QLabel(groupBox);
        Alt->setObjectName(QStringLiteral("Alt"));

        horizontalLayout->addWidget(Alt);


        gridLayout_2->addWidget(groupBox, 1, 1, 1, 4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_2->addWidget(label_10);

        Bat = new QLabel(groupBox_3);
        Bat->setObjectName(QStringLiteral("Bat"));

        horizontalLayout_2->addWidget(Bat);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_2->addWidget(label_13);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 3, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_6->addWidget(label_6);

        VelH = new QLabel(groupBox_3);
        VelH->setObjectName(QStringLiteral("VelH"));

        horizontalLayout_6->addWidget(VelH);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_6->addWidget(label_14);


        gridLayout_2->addLayout(horizontalLayout_6, 2, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_8->addWidget(label_8);

        VelV = new QLabel(groupBox_3);
        VelV->setObjectName(QStringLiteral("VelV"));

        horizontalLayout_8->addWidget(VelV);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_8->addWidget(label_12);


        gridLayout_2->addLayout(horizontalLayout_8, 2, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        gridLayout_2->addLayout(verticalLayout_2, 0, 1, 1, 1);


        horizontalLayout_3->addWidget(groupBox_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1062, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_3->setText(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Map", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "GPS Data", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Map_Refresh", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Lat", Q_NULLPTR));
        lineEditLat->setText(QApplication::translate("MainWindow", "29.6169", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Lng", Q_NULLPTR));
        lineEditLng->setText(QApplication::translate("MainWindow", "106.498", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", " Alt ", Q_NULLPTR));
        Alt->setText(QApplication::translate("MainWindow", "000.0", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Battery:", Q_NULLPTR));
        Bat->setText(QApplication::translate("MainWindow", "99", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Velocity_H:", Q_NULLPTR));
        VelH->setText(QApplication::translate("MainWindow", "3.3", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "m/s", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Velocity_V:", Q_NULLPTR));
        VelV->setText(QApplication::translate("MainWindow", "3.3", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "m/s", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
