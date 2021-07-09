#pragma once 
#include <QList>
#include <QHash>
#include <QString>
class Graph{
    int n;
    QString root;
    QHash<QString, QList<QString>> adj;
public:
    Graph();
    void add_edge(QString from, QString to);
    void print();
};

