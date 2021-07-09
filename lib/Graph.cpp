#include "Graph.hpp"
#include <QStack>
#include <QDebug>

Node::Node(QString name, QString value){
    this->name = name;
    this->value = value;
}

Graph::Graph(){
    this->n_edges = 0;
    this->n_nodes = 0;
}

Graph::~Graph(){
    for(auto it = this->adj.begin(); it != this->adj.end(); it++){
        if (it.key()){
            if (this->adj[it.key()])
                delete this->adj[it.key()];
            delete it.key();
        }
    }
}

void Graph::add_edge(Node* from, Node* to){
    if(this->n_nodes == 0)
        this->root = from;
    if(to){
        if (this->adj.find(from) == this->adj.end()){   // Key not found
            this->n_nodes++;
            this->adj[from] = new QList<Node*>;
        }
        else if (this->adj[from] == nullptr){   
            this->adj[from] = new QList<Node*>;
        }
        this->adj[from]->push_back(to);
        this->n_edges++;
    }
    else{
        if (this->adj.find(from) == this->adj.end()){    //Key not found
            this->adj[from] = nullptr;
            this->n_nodes++;
        }
    }
}


//This function using iterative DFS
void Graph::print(){
    QStack<Node*> s;
    s.push(this->root);
    QHash<Node*, bool> visited;
    visited[root] = true;
    while(!s.empty()){
        Node* current = s.top();
        s.pop();
        qDebug() << current->name << ' ';
        if(this->adj[current]){
            for(auto it = this->adj[current]->begin(); it!=this->adj[current]->end(); it++){
                if(visited.find(*it) == visited.end()){
                    visited[*it] = true;
                    s.push(*it);
                }
            }   
        }
    }
    qDebug() << '\n';
}

Graph build_tree(QString xml_file){
    Graph tree;
    //QStack <QString> tags;
    QStack <Node*> tags;
    QString current_tag = "";
    QString tag_value = "";
    
    int len = xml_file.length();
    for (int i = 0; i < len; i++){
        while(xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f'){
            if (i < len)
                i++;
            else{
                // throw EOF
                break;
            }
        }
        
        if (xml_file[i] == '<' && xml_file[i + 1] != '/'){
            current_tag = "";
            tag_value = "";
           
            while(xml_file[i] != '>'){
                current_tag += xml_file[i++];
            }
            current_tag += xml_file[i++];
            Node* parent = new Node(current_tag.trimmed(), "");
            tags.push(parent);
            while(xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f'){
                if (i < len)
                    i++;
                else{
                    // throw EOF
                    break;
                }
            }
            if (xml_file[i] != '<'){
                while(xml_file[i] != '<'){
                    tag_value += xml_file[i++];
                } // after the content, the closed tag comes so we will pop
                tags.pop();
                Node* child = new Node(current_tag, tag_value);
                tree.add_edge(tags.top(), child);
                while(xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f'){
                    if (i < len)
                        i++;
                    else{
                        // throw EOF
                        break;
                    }
                }           
            } else if (xml_file[i] == '<' && xml_file[i + 1] == '/'){
                tags.pop();
            }

        }
        
    }
    return tree;
}
