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

    //basics();
    //Graph t = build_tree("<html><head></head><body><ul><li>a</li><li>b</li><li>c</li></ul><a>aaa</a><p>ppp</p></body></html>");

    // QString s ="<breakfast_menu><food><name>Belgian Waffles</name><price>$5.95</price><description>Two of our famous Belgian Waffles with plenty of real maple syrup</description><calories>650</calories></food><food><name>Strawberry Belgian Waffles</name><price>$7.95</price><description>Light Belgian waffles covered with strawberries and whipped cream</description><calories>900</calories></food><food><name>Berry-Berry Belgian Waffles</name><price>$8.95</price><description>Light Belgian waffles covered with an assortment of fresh berries and whipped cream</description><calories>900</calories></food><food><name>French Toast</name><price>$4.50</price><description>Thick slices made from our homemade sourdough bread</description><calories>600</calories></food><food><name>Homestyle Breakfast</name><price>$6.95</price><description>Two eggs, bacon or sausage, toast, and our ever-popular hash browns</description><calories>950</calories></food></breakfast_menu>";

   // qDebug().noquote() <<check(s) ;

    // Graph t = build_tree(s);
//    check(QString("<html><head></head><body><ul><li>a</li><li>b</li><li>c</li></ul><a>aaa</a><p>ppp</p></body></html>"));
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
    //t.print();
 //   QString json = "", xml = "";
   // qDebug().noquote() << t.convert_to_json(t.get_root(), 1, json, true);
    //qDebug().noquote() << t.beautify_xml(t.get_root(), 0, xml);
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
