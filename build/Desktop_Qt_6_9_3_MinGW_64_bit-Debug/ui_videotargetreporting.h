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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoTargetReporting
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widgetTools;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_10;
    QPushButton *btnBack;
    QPushButton *btnHistoryRecord;
    QPushButton *btnSettings;
    QPushButton *btnMinisize;
    QPushButton *btnExit;
    QWidget *widgetMain;
    QHBoxLayout *horizontalLayout;
    QWidget *leftPanel;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidgetTargetDisplay;
    QWidget *pageChestRingTarget;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget_11;
    QLabel *label_4;
    QWidget *widget_12;
    QWidget *pageIDPA;
    QWidget *pageIPSC;
    QFrame *frame_connectionState;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_connectionState;
    QWidget *rightPanel;
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *pageScoreDisplay;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QWidget *pageScoreHistory;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listWidget;
    QPushButton *pushButton_6;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_2;
    QTableWidget *tableWidget_2;
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
    QPushButton *pushButton_connect;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_xhbDisplay;
    QLabel *label_ipscDiaplay;
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

        pushButton_10 = new QPushButton(widgetTools);
        pushButton_10->setObjectName("pushButton_10");

        horizontalLayout_2->addWidget(pushButton_10);

        btnBack = new QPushButton(widgetTools);
        btnBack->setObjectName("btnBack");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
        btnBack->setIcon(icon);

        horizontalLayout_2->addWidget(btnBack);

        btnHistoryRecord = new QPushButton(widgetTools);
        btnHistoryRecord->setObjectName("btnHistoryRecord");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpenRecent));
        btnHistoryRecord->setIcon(icon1);

        horizontalLayout_2->addWidget(btnHistoryRecord);

        btnSettings = new QPushButton(widgetTools);
        btnSettings->setObjectName("btnSettings");

        horizontalLayout_2->addWidget(btnSettings);

        btnMinisize = new QPushButton(widgetTools);
        btnMinisize->setObjectName("btnMinisize");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
        btnMinisize->setIcon(icon2);

        horizontalLayout_2->addWidget(btnMinisize);

        btnExit = new QPushButton(widgetTools);
        btnExit->setObjectName("btnExit");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit));
        btnExit->setIcon(icon3);

        horizontalLayout_2->addWidget(btnExit);

        horizontalLayout_2->setStretch(0, 70);
        horizontalLayout_2->setStretch(2, 6);
        horizontalLayout_2->setStretch(3, 6);
        horizontalLayout_2->setStretch(4, 6);
        horizontalLayout_2->setStretch(5, 6);
        horizontalLayout_2->setStretch(6, 6);

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
        stackedWidgetTargetDisplay = new QStackedWidget(leftPanel);
        stackedWidgetTargetDisplay->setObjectName("stackedWidgetTargetDisplay");
        pageChestRingTarget = new QWidget();
        pageChestRingTarget->setObjectName("pageChestRingTarget");
        verticalLayout_9 = new QVBoxLayout(pageChestRingTarget);
        verticalLayout_9->setObjectName("verticalLayout_9");
        widget_11 = new QWidget(pageChestRingTarget);
        widget_11->setObjectName("widget_11");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_11->sizePolicy().hasHeightForWidth());
        widget_11->setSizePolicy(sizePolicy);
        label_4 = new QLabel(widget_11);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 10, 391, 381));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/project01/images/chest_ring_target.jpg")));
        label_4->setScaledContents(true);
        widget_12 = new QWidget(widget_11);
        widget_12->setObjectName("widget_12");
        widget_12->setGeometry(QRect(100, 120, 120, 80));

        verticalLayout_9->addWidget(widget_11);

        stackedWidgetTargetDisplay->addWidget(pageChestRingTarget);
        pageIDPA = new QWidget();
        pageIDPA->setObjectName("pageIDPA");
        stackedWidgetTargetDisplay->addWidget(pageIDPA);
        pageIPSC = new QWidget();
        pageIPSC->setObjectName("pageIPSC");
        stackedWidgetTargetDisplay->addWidget(pageIPSC);

        verticalLayout_2->addWidget(stackedWidgetTargetDisplay);

        frame_connectionState = new QFrame(leftPanel);
        frame_connectionState->setObjectName("frame_connectionState");
        frame_connectionState->setFrameShape(QFrame::Shape::StyledPanel);
        frame_connectionState->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_11 = new QHBoxLayout(frame_connectionState);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_connectionState = new QLabel(frame_connectionState);
        label_connectionState->setObjectName("label_connectionState");
        QFont font;
        font.setPointSize(22);
        label_connectionState->setFont(font);
        label_connectionState->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_11->addWidget(label_connectionState);


        verticalLayout_2->addWidget(frame_connectionState);

        verticalLayout_2->setStretch(0, 9);
        verticalLayout_2->setStretch(1, 1);

        horizontalLayout->addWidget(leftPanel);

        rightPanel = new QWidget(widgetMain);
        rightPanel->setObjectName("rightPanel");
        verticalLayout_3 = new QVBoxLayout(rightPanel);
        verticalLayout_3->setObjectName("verticalLayout_3");
        stackedWidget = new QStackedWidget(rightPanel);
        stackedWidget->setObjectName("stackedWidget");
        pageScoreDisplay = new QWidget();
        pageScoreDisplay->setObjectName("pageScoreDisplay");
        verticalLayout_4 = new QVBoxLayout(pageScoreDisplay);
        verticalLayout_4->setObjectName("verticalLayout_4");
        widget = new QWidget(pageScoreDisplay);
        widget->setObjectName("widget");
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setFont(font);

        horizontalLayout_4->addWidget(label);


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
        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName("pushButton");
        pushButton->setFont(font);

        verticalLayout_5->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget_3);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setFont(font);

        verticalLayout_5->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(widget_3);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setFont(font);

        verticalLayout_5->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setFont(font);

        verticalLayout_5->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setFont(font);

        verticalLayout_5->addWidget(pushButton_5);


        horizontalLayout_3->addWidget(widget_3);

        horizontalLayout_3->setStretch(0, 8);
        horizontalLayout_3->setStretch(1, 2);

        verticalLayout_4->addWidget(widget_2);

        stackedWidget->addWidget(pageScoreDisplay);
        pageScoreHistory = new QWidget();
        pageScoreHistory->setObjectName("pageScoreHistory");
        horizontalLayout_5 = new QHBoxLayout(pageScoreHistory);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        widget_4 = new QWidget(pageScoreHistory);
        widget_4->setObjectName("widget_4");
        verticalLayout_6 = new QVBoxLayout(widget_4);
        verticalLayout_6->setObjectName("verticalLayout_6");
        listWidget = new QListWidget(widget_4);
        listWidget->setObjectName("listWidget");

        verticalLayout_6->addWidget(listWidget);

        pushButton_6 = new QPushButton(widget_4);
        pushButton_6->setObjectName("pushButton_6");

        verticalLayout_6->addWidget(pushButton_6);


        horizontalLayout_5->addWidget(widget_4);

        widget_5 = new QWidget(pageScoreHistory);
        widget_5->setObjectName("widget_5");
        verticalLayout_7 = new QVBoxLayout(widget_5);
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_2 = new QLabel(widget_5);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        verticalLayout_7->addWidget(label_2);

        tableWidget_2 = new QTableWidget(widget_5);
        tableWidget_2->setObjectName("tableWidget_2");

        verticalLayout_7->addWidget(tableWidget_2);


        horizontalLayout_5->addWidget(widget_5);

        horizontalLayout_5->setStretch(0, 4);
        horizontalLayout_5->setStretch(1, 6);
        stackedWidget->addWidget(pageScoreHistory);
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

        pushButton_connect = new QPushButton(widget_7);
        pushButton_connect->setObjectName("pushButton_connect");
        pushButton_connect->setFont(font);

        horizontalLayout_9->addWidget(pushButton_connect);

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
        label_xhbDisplay = new QLabel(widget_9);
        label_xhbDisplay->setObjectName("label_xhbDisplay");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_xhbDisplay->sizePolicy().hasHeightForWidth());
        label_xhbDisplay->setSizePolicy(sizePolicy1);
        label_xhbDisplay->setPixmap(QPixmap(QString::fromUtf8(":/project01/images/chest_ring_target.jpg")));
        label_xhbDisplay->setScaledContents(true);
        label_xhbDisplay->setWordWrap(false);
        label_xhbDisplay->setOpenExternalLinks(false);

        horizontalLayout_7->addWidget(label_xhbDisplay);

        label_ipscDiaplay = new QLabel(widget_9);
        label_ipscDiaplay->setObjectName("label_ipscDiaplay");
        sizePolicy1.setHeightForWidth(label_ipscDiaplay->sizePolicy().hasHeightForWidth());
        label_ipscDiaplay->setSizePolicy(sizePolicy1);
        label_ipscDiaplay->setPixmap(QPixmap(QString::fromUtf8(":/project01/images/IPSC.png")));
        label_ipscDiaplay->setScaledContents(true);
        label_ipscDiaplay->setWordWrap(false);

        horizontalLayout_7->addWidget(label_ipscDiaplay);

        label_idpaDiaplay = new QLabel(widget_9);
        label_idpaDiaplay->setObjectName("label_idpaDiaplay");
        sizePolicy1.setHeightForWidth(label_idpaDiaplay->sizePolicy().hasHeightForWidth());
        label_idpaDiaplay->setSizePolicy(sizePolicy1);
        label_idpaDiaplay->setPixmap(QPixmap(QString::fromUtf8(":/project01/images/IDPA.png")));
        label_idpaDiaplay->setScaledContents(true);

        horizontalLayout_7->addWidget(label_idpaDiaplay);

        horizontalLayout_7->setStretch(0, 6);
        horizontalLayout_7->setStretch(1, 5);
        horizontalLayout_7->setStretch(2, 4);

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
        stackedWidget->addWidget(pageSettings);

        verticalLayout_3->addWidget(stackedWidget);


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

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(VideoTargetReporting);
    } // setupUi

    void retranslateUi(QMainWindow *VideoTargetReporting)
    {
        VideoTargetReporting->setWindowTitle(QCoreApplication::translate("VideoTargetReporting", "VideoTargetReporting", nullptr));
        pushButton_10->setText(QCoreApplication::translate("VideoTargetReporting", "PushButton", nullptr));
        btnBack->setText(QString());
        btnHistoryRecord->setText(QString());
        btnSettings->setText(QString());
        btnMinisize->setText(QString());
        btnExit->setText(QString());
        label_4->setText(QString());
        label_connectionState->setText(QCoreApplication::translate("VideoTargetReporting", "\345\275\223\345\211\215\351\235\266\345\236\213\357\274\232\345\276\205\351\200\211\346\213\251", nullptr));
        label->setText(QCoreApplication::translate("VideoTargetReporting", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232\345\276\205\345\274\200\345\247\213", nullptr));
        pushButton->setText(QCoreApplication::translate("VideoTargetReporting", "\345\274\200\345\247\213", nullptr));
        pushButton_2->setText(QCoreApplication::translate("VideoTargetReporting", "\346\232\202\345\201\234", nullptr));
        pushButton_3->setText(QCoreApplication::translate("VideoTargetReporting", "\347\273\223\346\235\237", nullptr));
        pushButton_4->setText(QCoreApplication::translate("VideoTargetReporting", "\351\207\215\347\275\256", nullptr));
        pushButton_5->setText(QCoreApplication::translate("VideoTargetReporting", "\344\277\235\345\255\230", nullptr));
        pushButton_6->setText(QString());
        label_2->setText(QString());
        label_3->setText(QCoreApplication::translate("VideoTargetReporting", "\346\234\254\346\234\272\347\212\266\346\200\201", nullptr));
        label_HID->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\350\277\236\346\216\245", nullptr));
        pushButton_HID->setText(QCoreApplication::translate("VideoTargetReporting", "HID\350\277\236\346\216\245", nullptr));
        label_5->setText(QCoreApplication::translate("VideoTargetReporting", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
        label_connect->setText(QCoreApplication::translate("VideoTargetReporting", "\345\276\205\350\277\236\346\216\245", nullptr));
        pushButton_connect->setText(QCoreApplication::translate("VideoTargetReporting", "\351\207\215\346\226\260\350\277\236\346\216\245", nullptr));
        label_7->setText(QCoreApplication::translate("VideoTargetReporting", "\351\235\266\345\236\213\351\200\211\346\213\251", nullptr));
        label_xhbDisplay->setText(QString());
        label_ipscDiaplay->setText(QString());
        label_idpaDiaplay->setText(QString());
        pushButton_9->setText(QCoreApplication::translate("VideoTargetReporting", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoTargetReporting: public Ui_VideoTargetReporting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOTARGETREPORTING_H
