#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{    
    Graph t = build_tree("<html><head></head><body><ul><li>a</li><li>b</li><li>c</li></ul><a>aaa</a><p>ppp</p></body></html>");
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
    t.print();
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
