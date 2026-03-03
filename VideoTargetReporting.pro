QT       += core gui serialport multimedia

LIBS += -LD:\WorkData\QtWorkspace\project01\hidapi

LIBS += -lhidapi

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BulletHistoryManager.cpp \
    HIDBusinessHandler.cpp \
    HIDCommandParser.cpp \
    HIDResponseGenerator.cpp \
    main.cpp \
    videotargetreporting.cpp

HEADERS += \
    BulletHistoryManager.h \
    HIDBusinessHandler.h \
    HIDCommandDef.h \
    HIDCommandParser.h \
    HIDReadThread.h \
    HIDResponseGenerator.h \
    HoleOverlay.h \
    hidapi.h \
    videotargetreporting.h

FORMS += \
    videotargetreporting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
