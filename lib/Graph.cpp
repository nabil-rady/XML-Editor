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

QString Graph::_convert_to_json(Node* node, QString& json ,QHash<Node*, bool>& visited, int level){
    QString tab = "";
    for(int i = 0; i < level; i++)
        tab+='\t';
    json = json + tab + QString("\"") + node->name + QString("\"") + QString(": ")+ QString("\"") + node->value + QString("\"");
    visited[node] = true;
    for(auto it = this->adj[node]->begin(); it != this->adj[node]->end(); it++){
        if(visited.find(*it) == visited.end()){
            _convert_to_json(*it, json, visited, level+1);
        }
    }
    return json;
}

QString Graph::convert_to_json(){
    QHash<Node*, bool> visited;
    QString json = "{\n";
    return this->_convert_to_json(this->root, json, visited, 1) + "\n}";
}

Graph build_tree(QString xml_file){
    Graph tree;
    //QStack <QString> tags;
    QStack <Node*> tags;
    QString current_tag = "";
    QString tag_value = "";
    
    int len = xml_file.length();
    for (int i = 0; i < len;){
        while(xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f'){
            if (i < len)
                i++;
            else{
                // throw EOF
                break;
            }
        }
        
        if (xml_file[i] == '<' && xml_file[i + 1] != '/'){
            current_tag = QString("");
            tag_value = QString("");
           
            while(xml_file[i] != '>'){
                current_tag += xml_file[i++];
            }
            current_tag += xml_file[i++];
            Node* parent = new Node(current_tag.trimmed(), QString(""));
            if (!tags.empty())
                tree.add_edge(tags.top(), parent);
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
                Node* child = tags.top();
                child->value = tag_value;
                tags.pop();
                
//                tree.add_edge(tags.top(), child);
                while(xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f'){
                    if (i < len)
                        i++;
                    else{
                        // throw EOF
                        break;
                    }
                }           
            }

        }
        if (xml_file[i] == '<' && xml_file[i + 1] == '/'){
            QString closed_tag = "";
            while(xml_file[i] != '>'){
                if (xml_file[i] == '/'){
                    i++;
                    continue;
                }
                closed_tag += xml_file[i++];
            }
            closed_tag += xml_file[i++];
            if (closed_tag == tags.top()->name)
                tags.pop();
        }
    }
    return tree;
}
