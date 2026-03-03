/********************************************************************************
** Form generated from reading UI file 'videotargetreporting.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOTARGETREPORTING_H
#define UI_VIDEOTARGETREPORTING_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "HoleOverlay.h"

QT_BEGIN_NAMESPACE

class Ui_VideoTargetReporting
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widgetTools;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_back;
    QPushButton *pushButton_history;
    QPushButton *pushButton_settings;
    QPushButton *pushButton_minimize;
    QPushButton *pushButton_exit;
    QWidget *widgetMain;
    QHBoxLayout *horizontalLayout;
    QWidget *leftPanel;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget_target;
    QWidget *pageChestRingTarget;
    QVBoxLayout *verticalLayout_9;
    QWidget *targetContainer;
    QLabel *label_xhb;
    HoleOverlay *widget_xhbHole;
    QWidget *pageIDPA;
    QLabel *label_idpa;
    HoleOverlay *widget_idpaHole;
    QWidget *pageIPSC;
    QLabel *label_ipsc;
    HoleOverlay *widget_ipscHole;
    QFrame *frame_connectionState;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_currentTarget;
    QLabel *label_connectionState;
    QWidget *rightPanel;
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget_right;
    QWidget *pageScoreDisplay;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_currentstate;
    QLabel *label_currentState;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton_start;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_end;
    QPushButton *pushButton_reset;
    QPushButton *pushButton_saveRecord;
    QTextEdit *textEdit_HID;
    QWidget *pageScoreHistory;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_historyList;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listWidget_historyTitles;
    QPushButton *pushButton_deleteRecord;
    QWidget *widget_historyDetail;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_historyDetailTitle;
    QTableWidget *tableWidget_historyDetail;
    QWidget *pageSettings;
    QVBoxLayout *verticalLayout_8;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_3;
    QLabel *label_HID;
    QPushButton *pushButton_HID;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_5;
    QLabel *label_connect;
    QPushButton *pushButton_reconnection;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_xhbSelected;
    QLabel *label_xhbDisplay;
    QLabel *label_ipscSelected;
    QLabel *label_ipscDiaplay;
    QLabel *label_idpaSelected;
    QLabel *label_idpaDiaplay;
    QWidget *widget_10;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_9;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *VideoTargetReporting)
    {
        if (VideoTargetReporting->objectName().isEmpty())
            VideoTargetReporting->setObjectName("VideoTargetReporting");
        VideoTargetReporting->resize(945, 604);
        centralwidget = new QWidget(VideoTargetReporting);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        widgetTools = new QWidget(centralwidget);
        widgetTools->setObjectName("widgetTools");
        horizontalLayout_2 = new QHBoxLayout(widgetTools);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_back = new QPushButton(widgetTools);
        pushButton_back->setObjectName("pushButton_back");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
        pushButton_back->setIcon(icon);

        horizontalLayout_2->addWidget(pushButton_back);

        pushButton_history = new QPushButton(widgetTools);
        pushButton_history->setObjectName("pushButton_history");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpenRecent));
        pushButton_history->setIcon(icon1);

        horizontalLayout_2->addWidget(pushButton_history);

        pushButton_settings = new QPushButton(widgetTools);
        pushButton_settings->setObjectName("pushButton_settings");

        horizontalLayout_2->addWidget(pushButton_settings);

        pushButton_minimize = new QPushButton(widgetTools);
        pushButton_minimize->setObjectName("pushButton_minimize");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
        pushButton_minimize->setIcon(icon2);

        horizontalLayout_2->addWidget(pushButton_minimize);

        pushButton_exit = new QPushButton(widgetTools);
        pushButton_exit->setObjectName("pushButton_exit");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));
        pushButton_exit->setIcon(icon3);

        horizontalLayout_2->addWidget(pushButton_exit);

        horizontalLayout_2->setStretch(0, 70);
        horizontalLayout_2->setStretch(1, 6);
        horizontalLayout_2->setStretch(2, 6);
        horizontalLayout_2->setStretch(3, 6);
        horizontalLayout_2->setStretch(4, 6);
        horizontalLayout_2->setStretch(5, 6);

        verticalLayout->addWidget(widgetTools);

        widgetMain = new QWidget(centralwidget);
        widgetMain->setObjectName("widgetMain");
        widgetMain->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        horizontalLayout = new QHBoxLayout(widgetMain);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        leftPanel = new QWidget(widgetMain);
        leftPanel->setObjectName("leftPanel");
        verticalLayout_2 = new QVBoxLayout(leftPanel);
        verticalLayout_2->setObjectName("verticalLayout_2");
        stackedWidget_target = new QStackedWidget(leftPanel);
        stackedWidget_target->setObjectName("stackedWidget_target");
        pageChestRingTarget = new QWidget();
        pageChestRingTarget->setObjectName("pageChestRingTarget");
        verticalLayout_9 = new QVBoxLayout(pageChestRingTarget);
        verticalLayout_9->setObjectName("verticalLayout_9");
        targetContainer = new QWidget(pageChestRingTarget);
        targetContainer->setObjectName("targetContainer");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(targetContainer->sizePolicy().hasHeightForWidth());
        targetContainer->setSizePolicy(sizePolicy);
        label_xhb = new QLabel(targetContainer);
        label_xhb->setObjectName("label_xhb");
        label_xhb->setGeometry(QRect(20, 30, 381, 351));
        label_xhb->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/chest_ring_target.jpg")));
        label_xhb->setScaledContents(true);
        widget_xhbHole = new HoleOverlay(targetContainer);
        widget_xhbHole->setObjectName("widget_xhbHole");
        widget_xhbHole->setGeometry(QRect(20, 30, 381, 351));

        verticalLayout_9->addWidget(targetContainer);

        stackedWidget_target->addWidget(pageChestRingTarget);
        pageIDPA = new QWidget();
        pageIDPA->setObjectName("pageIDPA");
        label_idpa = new QLabel(pageIDPA);
        label_idpa->setObjectName("label_idpa");
        label_idpa->setGeometry(QRect(90, 0, 251, 411));
        label_idpa->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/IDPA.png")));
        label_idpa->setScaledContents(true);
        widget_idpaHole = new HoleOverlay(pageIDPA);
        widget_idpaHole->setObjectName("widget_idpaHole");
        widget_idpaHole->setGeometry(QRect(90, 0, 251, 411));
        stackedWidget_target->addWidget(pageIDPA);
        pageIPSC = new QWidget();
        pageIPSC->setObjectName("pageIPSC");
        label_ipsc = new QLabel(pageIPSC);
        label_ipsc->setObjectName("label_ipsc");
        label_ipsc->setGeometry(QRect(50, 0, 321, 411));
        label_ipsc->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/IPSC.png")));
        label_ipsc->setScaledContents(true);
        widget_ipscHole = new HoleOverlay(pageIPSC);
        widget_ipscHole->setObjectName("widget_ipscHole");
        widget_ipscHole->setGeometry(QRect(50, 0, 321, 411));
        stackedWidget_target->addWidget(pageIPSC);

        verticalLayout_2->addWidget(stackedWidget_target);

        frame_connectionState = new QFrame(leftPanel);
        frame_connectionState->setObjectName("frame_connectionState");
        frame_connectionState->setFrameShape(QFrame::Shape::StyledPanel);
        frame_connectionState->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_11 = new QHBoxLayout(frame_connectionState);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_currentTarget = new QLabel(frame_connectionState);
        label_currentTarget->setObjectName("label_currentTarget");
        QFont font;
        font.setPointSize(22);
        label_currentTarget->setFont(font);
        label_currentTarget->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_11->addWidget(label_currentTarget);

        label_connectionState = new QLabel(frame_connectionState);
        label_connectionState->setObjectName("label_connectionState");
        label_connectionState->setFont(font);

        horizontalLayout_11->addWidget(label_connectionState);

        horizontalLayout_11->setStretch(0, 1);
        horizontalLayout_11->setStretch(1, 1);

        verticalLayout_2->addWidget(frame_connectionState);

        verticalLayout_2->setStretch(0, 9);
        verticalLayout_2->setStretch(1, 1);

        horizontalLayout->addWidget(leftPanel);

        rightPanel = new QWidget(widgetMain);
        rightPanel->setObjectName("rightPanel");
        verticalLayout_3 = new QVBoxLayout(rightPanel);
        verticalLayout_3->setObjectName("verticalLayout_3");
        stackedWidget_right = new QStackedWidget(rightPanel);
        stackedWidget_right->setObjectName("stackedWidget_right");
        pageScoreDisplay = new QWidget();
        pageScoreDisplay->setObjectName("pageScoreDisplay");
        verticalLayout_4 = new QVBoxLayout(pageScoreDisplay);
        verticalLayout_4->setObjectName("verticalLayout_4");
        widget = new QWidget(pageScoreDisplay);
        widget->setObjectName("widget");
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_currentstate = new QLabel(widget);
        label_currentstate->setObjectName("label_currentstate");
        label_currentstate->setFont(font);

        horizontalLayout_4->addWidget(label_currentstate);

        label_currentState = new QLabel(widget);
        label_currentState->setObjectName("label_currentState");
        label_currentState->setFont(font);

        horizontalLayout_4->addWidget(label_currentState);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 2);

        verticalLayout_4->addWidget(widget);

        widget_2 = new QWidget(pageScoreDisplay);
        widget_2->setObjectName("widget_2");
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        tableWidget = new QTableWidget(widget_2);
        tableWidget->setObjectName("tableWidget");

        horizontalLayout_3->addWidget(tableWidget);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName("widget_3");
        verticalLayout_5 = new QVBoxLayout(widget_3);
        verticalLayout_5->setObjectName("verticalLayout_5");
        pushButton_start = new QPushButton(widget_3);
        pushButton_start->setObjectName("pushButton_start");
        pushButton_start->setFont(font);

        verticalLayout_5->addWidget(pushButton_start);

        pushButton_pause = new QPushButton(widget_3);
        pushButton_pause->setObjectName("pushButton_pause");
        pushButton_pause->setFont(font);

        verticalLayout_5->addWidget(pushButton_pause);

        pushButton_end = new QPushButton(widget_3);
        pushButton_end->setObjectName("pushButton_end");
        pushButton_end->setFont(font);

        verticalLayout_5->addWidget(pushButton_end);

        pushButton_reset = new QPushButton(widget_3);
        pushButton_reset->setObjectName("pushButton_reset");
        pushButton_reset->setFont(font);

        verticalLayout_5->addWidget(pushButton_reset);

        pushButton_saveRecord = new QPushButton(widget_3);
        pushButton_saveRecord->setObjectName("pushButton_saveRecord");
        pushButton_saveRecord->setFont(font);

        verticalLayout_5->addWidget(pushButton_saveRecord);

        textEdit_HID = new QTextEdit(widget_3);
        textEdit_HID->setObjectName("textEdit_HID");

        verticalLayout_5->addWidget(textEdit_HID);


        horizontalLayout_3->addWidget(widget_3);

        horizontalLayout_3->setStretch(0, 8);
        horizontalLayout_3->setStretch(1, 2);

        verticalLayout_4->addWidget(widget_2);

        stackedWidget_right->addWidget(pageScoreDisplay);
        pageScoreHistory = new QWidget();
        pageScoreHistory->setObjectName("pageScoreHistory");
        horizontalLayout_5 = new QHBoxLayout(pageScoreHistory);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        widget_historyList = new QWidget(pageScoreHistory);
        widget_historyList->setObjectName("widget_historyList");
        verticalLayout_6 = new QVBoxLayout(widget_historyList);
        verticalLayout_6->setObjectName("verticalLayout_6");
        listWidget_historyTitles = new QListWidget(widget_historyList);
        listWidget_historyTitles->setObjectName("listWidget_historyTitles");

        verticalLayout_6->addWidget(listWidget_historyTitles);

        pushButton_deleteRecord = new QPushButton(widget_historyList);
        pushButton_deleteRecord->setObjectName("pushButton_deleteRecord");

        verticalLayout_6->addWidget(pushButton_deleteRecord);


        horizontalLayout_5->addWidget(widget_historyList);

        widget_historyDetail = new QWidget(pageScoreHistory);
        widget_historyDetail->setObjectName("widget_historyDetail");
        verticalLayout_7 = new QVBoxLayout(widget_historyDetail);
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_historyDetailTitle = new QLabel(widget_historyDetail);
        label_historyDetailTitle->setObjectName("label_historyDetailTitle");
        label_historyDetailTitle->setFont(font);

        verticalLayout_7->addWidget(label_historyDetailTitle);

        tableWidget_historyDetail = new QTableWidget(widget_historyDetail);
        tableWidget_historyDetail->setObjectName("tableWidget_historyDetail");

        verticalLayout_7->addWidget(tableWidget_historyDetail);


        horizontalLayout_5->addWidget(widget_historyDetail);

        horizontalLayout_5->setStretch(0, 4);
        horizontalLayout_5->setStretch(1, 6);
        stackedWidget_right->addWidget(pageScoreHistory);
        pageSettings = new QWidget();
        pageSettings->setObjectName("pageSettings");
        verticalLayout_8 = new QVBoxLayout(pageSettings);
        verticalLayout_8->setObjectName("verticalLayout_8");
        widget_6 = new QWidget(pageSettings);
        widget_6->setObjectName("widget_6");
        horizontalLayout_8 = new QHBoxLayout(widget_6);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_3 = new QLabel(widget_6);
        label_3->setObjectName("label_3");
        QFont font1;
        font1.setPointSize(22);
        font1.setBold(true);
        label_3->setFont(font1);

        horizontalLayout_8->addWidget(label_3);

        label_HID = new QLabel(widget_6);
        label_HID->setObjectName("label_HID");
        label_HID->setFont(font);

        horizontalLayout_8->addWidget(label_HID);

        pushButton_HID = new QPushButton(widget_6);
        pushButton_HID->setObjectName("pushButton_HID");
        pushButton_HID->setFont(font);

        horizontalLayout_8->addWidget(pushButton_HID);

        horizontalLayout_8->setStretch(0, 1);
        horizontalLayout_8->setStretch(1, 1);
        horizontalLayout_8->setStretch(2, 1);

        verticalLayout_8->addWidget(widget_6);

        widget_7 = new QWidget(pageSettings);
        widget_7->setObjectName("widget_7");
        horizontalLayout_9 = new QHBoxLayout(widget_7);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_5 = new QLabel(widget_7);
        label_5->setObjectName("label_5");
        label_5->setFont(font1);

        horizontalLayout_9->addWidget(label_5);

        label_connect = new QLabel(widget_7);
        label_connect->setObjectName("label_connect");
        label_connect->setFont(font);

        horizontalLayout_9->addWidget(label_connect);

        pushButton_reconnection = new QPushButton(widget_7);
        pushButton_reconnection->setObjectName("pushButton_reconnection");
        pushButton_reconnection->setFont(font);

        horizontalLayout_9->addWidget(pushButton_reconnection);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 1);
        horizontalLayout_9->setStretch(2, 1);

        verticalLayout_8->addWidget(widget_7);

        widget_8 = new QWidget(pageSettings);
        widget_8->setObjectName("widget_8");
        horizontalLayout_10 = new QHBoxLayout(widget_8);
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_7 = new QLabel(widget_8);
        label_7->setObjectName("label_7");
        label_7->setFont(font1);

        horizontalLayout_10->addWidget(label_7);

        horizontalSpacer_2 = new QSpacerItem(210, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);

        horizontalLayout_10->setStretch(0, 1);
        horizontalLayout_10->setStretch(1, 2);

        verticalLayout_8->addWidget(widget_8);

        widget_9 = new QWidget(pageSettings);
        widget_9->setObjectName("widget_9");
        horizontalLayout_7 = new QHBoxLayout(widget_9);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_xhbSelected = new QLabel(widget_9);
        label_xhbSelected->setObjectName("label_xhbSelected");

        horizontalLayout_7->addWidget(label_xhbSelected);

        label_xhbDisplay = new QLabel(widget_9);
        label_xhbDisplay->setObjectName("label_xhbDisplay");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_xhbDisplay->sizePolicy().hasHeightForWidth());
        label_xhbDisplay->setSizePolicy(sizePolicy1);
        label_xhbDisplay->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/chest_ring_target.jpg")));
        label_xhbDisplay->setScaledContents(true);
        label_xhbDisplay->setWordWrap(false);
        label_xhbDisplay->setOpenExternalLinks(false);

        horizontalLayout_7->addWidget(label_xhbDisplay);

        label_ipscSelected = new QLabel(widget_9);
        label_ipscSelected->setObjectName("label_ipscSelected");

        horizontalLayout_7->addWidget(label_ipscSelected);

        label_ipscDiaplay = new QLabel(widget_9);
        label_ipscDiaplay->setObjectName("label_ipscDiaplay");
        sizePolicy1.setHeightForWidth(label_ipscDiaplay->sizePolicy().hasHeightForWidth());
        label_ipscDiaplay->setSizePolicy(sizePolicy1);
        label_ipscDiaplay->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/IPSC.png")));
        label_ipscDiaplay->setScaledContents(true);
        label_ipscDiaplay->setWordWrap(false);

        horizontalLayout_7->addWidget(label_ipscDiaplay);

        label_idpaSelected = new QLabel(widget_9);
        label_idpaSelected->setObjectName("label_idpaSelected");

        horizontalLayout_7->addWidget(label_idpaSelected);

        label_idpaDiaplay = new QLabel(widget_9);
        label_idpaDiaplay->setObjectName("label_idpaDiaplay");
        sizePolicy1.setHeightForWidth(label_idpaDiaplay->sizePolicy().hasHeightForWidth());
        label_idpaDiaplay->setSizePolicy(sizePolicy1);
        label_idpaDiaplay->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/images/IDPA.png")));
        label_idpaDiaplay->setScaledContents(true);

        horizontalLayout_7->addWidget(label_idpaDiaplay);

        horizontalLayout_7->setStretch(1, 6);
        horizontalLayout_7->setStretch(3, 5);
        horizontalLayout_7->setStretch(5, 4);

        verticalLayout_8->addWidget(widget_9);

        widget_10 = new QWidget(pageSettings);
        widget_10->setObjectName("widget_10");
        horizontalLayout_6 = new QHBoxLayout(widget_10);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer_3 = new QSpacerItem(241, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        pushButton_9 = new QPushButton(widget_10);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setFont(font);

        horizontalLayout_6->addWidget(pushButton_9);

        horizontalLayout_6->setStretch(0, 9);
        horizontalLayout_6->setStretch(1, 1);

        verticalLayout_8->addWidget(widget_10);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);

        verticalLayout_8->setStretch(0, 1);
        verticalLayout_8->setStretch(1, 1);
        verticalLayout_8->setStretch(2, 1);
        verticalLayout_8->setStretch(3, 4);
        verticalLayout_8->setStretch(4, 1);
        verticalLayout_8->setStretch(5, 2);
        stackedWidget_right->addWidget(pageSettings);

        verticalLayout_3->addWidget(stackedWidget_right);


        horizontalLayout->addWidget(rightPanel);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addWidget(widgetMain);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 95);
        VideoTargetReporting->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(VideoTargetReporting);
        statusbar->setObjectName("statusbar");
        VideoTargetReporting->setStatusBar(statusbar);

        retranslateUi(VideoTargetReporting);

        stackedWidget_target->setCurrentIndex(0);
        stackedWidget_right->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(VideoTargetReporting);
    } // setupUi

    void retranslateUi(QMainWindow *VideoTargetReporting)
    {
        VideoTargetReporting->setWindowTitle(QCoreApplication::translate("VideoTargetReporting", "VideoTargetReporting", nullptr));
        pushButton_back->setText(QString());
        pushButton_history->setText(QString());
        pushButton_settings->setText(QString());
        pushButton_minimize->setText(QString());
        pushButton_exit->setText(QString());
        label_xhb->setText(QString());
        label_idpa->setText(QString());
        label_ipsc->setText(QString());
        label_currentTarget->setText(QCoreApplication::translate("VideoTargetReporting", "\345\275\223\345\211\215\351\235\266\345\236\213\357\274\232", nullptr));
        label_connectionState->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\351\200\211\346\213\251", nullptr));
        label_currentstate->setText(QCoreApplication::translate("VideoTargetReporting", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232", nullptr));
        label_currentState->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\345\274\200\345\247\213", nullptr));
        pushButton_start->setText(QCoreApplication::translate("VideoTargetReporting", "\345\274\200\345\247\213", nullptr));
        pushButton_pause->setText(QCoreApplication::translate("VideoTargetReporting", "\346\232\202\345\201\234", nullptr));
        pushButton_end->setText(QCoreApplication::translate("VideoTargetReporting", "\347\273\223\346\235\237", nullptr));
        pushButton_reset->setText(QCoreApplication::translate("VideoTargetReporting", "\351\207\215\347\275\256", nullptr));
        pushButton_saveRecord->setText(QCoreApplication::translate("VideoTargetReporting", "\344\277\235\345\255\230", nullptr));
        pushButton_deleteRecord->setText(QString());
        label_historyDetailTitle->setText(QString());
        label_3->setText(QCoreApplication::translate("VideoTargetReporting", "\346\234\254\346\234\272\347\212\266\346\200\201", nullptr));
        label_HID->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\350\277\236\346\216\245", nullptr));
        pushButton_HID->setText(QCoreApplication::translate("VideoTargetReporting", "HID\350\277\236\346\216\245", nullptr));
        label_5->setText(QCoreApplication::translate("VideoTargetReporting", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
        label_connect->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\350\277\236\346\216\245", nullptr));
        pushButton_reconnection->setText(QCoreApplication::translate("VideoTargetReporting", "\351\207\215\346\226\260\350\277\236\346\216\245", nullptr));
        label_7->setText(QCoreApplication::translate("VideoTargetReporting", "\351\235\266\345\236\213\351\200\211\346\213\251", nullptr));
        label_xhbSelected->setText(QCoreApplication::translate("VideoTargetReporting", "xhb", nullptr));
        label_xhbDisplay->setText(QString());
        label_ipscSelected->setText(QCoreApplication::translate("VideoTargetReporting", "ipsc", nullptr));
        label_ipscDiaplay->setText(QString());
        label_idpaSelected->setText(QCoreApplication::translate("VideoTargetReporting", "idpa", nullptr));
        label_idpaDiaplay->setText(QString());
        pushButton_9->setText(QCoreApplication::translate("VideoTargetReporting", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoTargetReporting: public Ui_VideoTargetReporting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOTARGETREPORTING_H
