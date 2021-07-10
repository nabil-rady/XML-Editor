#pragma once 
#include <QList>
#include <QHash>
#include <QString>

struct Node{
    QString name;
    QString value;
    Node(QString name, QString value);
};

class Graph{
    int n_edges;
    int n_nodes;
    Node* root;
    QHash<Node*, QList<Node*>*> adj;
public:
    Graph();
    ~Graph();
    Node* get_root();
    void add_edge(Node* from, Node* to); //
    void print();
    QString _convert_to_json(Node* node,QString& json ,QHash<Node*, bool>& visited, int level, bool);
//    QString convert_to_json();
    QString convert_to_json(Node* node, int tab, QString& s, bool last);
};

Graph build_tree(QString xml_file);
