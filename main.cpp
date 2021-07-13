#include "mainwindow.h"
#include "lib/Graph.hpp"
#include "lib/check.hpp"
#include <QApplication>
#include <QFile>
//void basics()
//{
//    QFile file("tony.txt");
//    if(!file.open(QIODevice::ReadWrite))
//    {
//        qCritical()<<"couldn't open the file";
//        qCritical()<<file.errorString();
//        return;



//    }
//    qInfo()<<"writing file";
//    file.write(QByteArray("hello world"));
//    file.flush();


//     qInfo()<<"reading file";
//     file.seek(0);
//      qInfo()<<file.readAll();
//      file.close();

//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
