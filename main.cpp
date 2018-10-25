#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#ifdef Q_OS_WASM
    w.showMaximized();
#else
    w.show();
#endif
    return a.exec();
}
