#include "Graph.hpp"
#include <QStack>

Graph::Graph(){
    this->n = 0;
}

void Graph::add_edge(QString from, QString to){
    if (this->n == 0)
        this->root = from;
    this->adj[from].push_back(to);
    this->n++;
}


// This function using iterative DFS
void Graph::print(){
    QStack<QString> s;
    s.push(this->root);
    QHash<QString, bool> visited;
    visited[root] = true;
    while(!s.empty()){
        QString current = s.top();
        s.pop();
        for(QString& item : this->adj[current]){
            if(visited.find(item) == visited.end()){
                visited[item] = true;
                s.push(item);
            }
        }   
    }
}

