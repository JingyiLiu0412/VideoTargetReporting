/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *leftPanel;
    QVBoxLayout *leftLayout;
    QWidget *targetDisplayContainer;
    QLabel *statusLabel;
    QWidget *rightPanel;
    QVBoxLayout *rightLayout;
    QWidget *settingsContainer;
    QGroupBox *demoGroup;
    QVBoxLayout *demoLayout;
    QPushButton *btnAddHole;
    QPushButton *btnClearHoles;
    QSpacerItem *rightSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        leftPanel = new QWidget(centralwidget);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setSpacing(0);
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));
        leftLayout->setContentsMargins(8, 8, 8, 8);
        targetDisplayContainer = new QWidget(leftPanel);
        targetDisplayContainer->setObjectName(QString::fromUtf8("targetDisplayContainer"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(targetDisplayContainer->sizePolicy().hasHeightForWidth());
        targetDisplayContainer->setSizePolicy(sizePolicy);

        leftLayout->addWidget(targetDisplayContainer);

        statusLabel = new QLabel(leftPanel);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setMaximumHeight(32);

        leftLayout->addWidget(statusLabel);


        horizontalLayout->addWidget(leftPanel);

        rightPanel = new QWidget(centralwidget);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setSpacing(8);
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        rightLayout->setContentsMargins(8, 8, 8, 8);
        settingsContainer = new QWidget(rightPanel);
        settingsContainer->setObjectName(QString::fromUtf8("settingsContainer"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(settingsContainer->sizePolicy().hasHeightForWidth());
        settingsContainer->setSizePolicy(sizePolicy1);

        rightLayout->addWidget(settingsContainer);

        demoGroup = new QGroupBox(rightPanel);
        demoGroup->setObjectName(QString::fromUtf8("demoGroup"));
        demoLayout = new QVBoxLayout(demoGroup);
        demoLayout->setObjectName(QString::fromUtf8("demoLayout"));
        btnAddHole = new QPushButton(demoGroup);
        btnAddHole->setObjectName(QString::fromUtf8("btnAddHole"));

        demoLayout->addWidget(btnAddHole);

        btnClearHoles = new QPushButton(demoGroup);
        btnClearHoles->setObjectName(QString::fromUtf8("btnClearHoles"));

        demoLayout->addWidget(btnClearHoles);


        rightLayout->addWidget(demoGroup);

        rightSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rightLayout->addItem(rightSpacer);


        horizontalLayout->addWidget(rightPanel);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\346\212\245\351\235\266\347\263\273\347\273\237", nullptr));
        leftPanel->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #1a1a2e;", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\261\347\273\252", nullptr));
        statusLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #ffffff; font-size: 14px; padding: 4px;", nullptr));
        rightPanel->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #f0f0f0;", nullptr));
        demoGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\274\224\347\244\272\346\216\247\345\210\266", nullptr));
        btnAddHole->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\274\271\345\255\224\357\274\210\351\232\217\346\234\272\357\274\211", nullptr));
        btnClearHoles->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\346\211\200\346\234\211\345\274\271\345\255\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
