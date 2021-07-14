#include "mainwindow.h"
#include "lib/Graph.hpp"
#include "lib/check.hpp"
#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
