#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("视频报靶系统"));
    app.setApplicationVersion(QStringLiteral("2.0"));

    MainWindow w;
    w.show();

    return app.exec();
}
