#include "mainwindow.h"
#include "lib/Graph.hpp"
#include <QApplication>
#include <QFile>
void basics()
{
    QFile file("tony.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        qCritical()<<"couldn't open the file";
        qCritical()<<file.errorString();
        return;



    }
    qInfo()<<"writing file";
    file.write(QByteArray("hello world"));
    file.flush();


     qInfo()<<"reading file";
     file.seek(0);
      qInfo()<<file.readAll();
      file.close();

}


int main(int argc, char *argv[])
{
    basics();
    // Graph t = build_tree("<html><head></head><body><ul><li>a</li><li>b</li><li>c</li></ul><a>aaa</a><p>ppp</p></body></html>");
    QString s = "<bookstore><book><title>Everyday Italian</title><author>Giada De Laurentiis</author><year>2005</year><price>30.00</price></book><book><title>Harry Potter</title><author>J K. Rowling</author><year>2005</year><price>29.99</price></book><book><title>Learning XML</title><author>Erik T. Ray</author><year>2003</year><price>39.95</price></book></bookstore>";
    Graph t = build_tree(s);

    // Graph t = build_tree("<html><head></head><body><ul><li>a</li><li>b</li><li>c</li></ul><a>aaa</a><p>ppp</p></body></html>");
    // Node *aa = new Node("A", "");
    // Node *b = new Node("B", "");
    // Node *c = new Node("C", "");
    // Node *d = new Node("D", "");
    // Node *e = new Node("E", "");
    // Node *f = new Node("F", "");
    // Node *g = new Node("G", "");
    // t.add_edge(aa,b);
    // t.add_edge(aa,c);
    // t.add_edge(aa,d);
    // t.add_edge(b,e);
    // t.add_edge(c,f);
    // t.add_edge(d,g);
//    t.print();
    QString value = "";
    qDebug().noquote() << t.convert_to_json(t.get_root(), 1, value, true);
    // delete aa;
    // delete b;
    // delete c;
    // delete d;
    // delete e;
    // delete f;
    // delete g;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
