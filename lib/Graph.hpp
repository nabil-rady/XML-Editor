#pragma once 
#include <QList>
#include <QHash>
#include <QString>

struct Node{
    QString name;
    QString value;
    QString properties;
    Node(QString name, QString value, QString properties);
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
    QString convert_to_json();
    void _convert_to_json(Node* node, int tab, QString& s, bool last);
    void _beautify_xml(Node* node, int tab, QString& s);
    QString beautify_xml();
};

Graph build_tree(QString xml_file);
