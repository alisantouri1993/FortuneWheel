#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Ali");
    a.setApplicationName("Fortune Wheel");
    MainWindow w;
    w.setWindowTitle("Fortune Wheel");
    w.show();

    return a.exec();
}
