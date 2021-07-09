#include "mainwindow.h"
#include "lib/Graph.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "ANA ESMY HMADA\n";
    Graph t = build_tree("<ul><li>btgan</li><li>oaiweoqaiwejd</li></ul><a>this is link </a> <p> this is p </p>");
    t.print();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
