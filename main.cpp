#include "videotargetreporting.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoTargetReporting w;
    w.show();
    return a.exec();
}
