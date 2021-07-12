#include "Graph.hpp"

#include <QStack>

#include <QDebug>

Node::Node(QString name, QString value, QString properties, bool self_closing) {
    this->name = name;
    this->value = value;
    this->properties = properties;
    this->self_closing = self_closing;
    this->parent_of_array = false;
}

Graph::Graph() {
    this->n_edges = 0;
    this->n_nodes = 0;
}

Graph::~Graph() {
    for (auto it = this->adj.begin(); it != this->adj.end(); it++) {
        if (it.key()) {
            if (this->adj[it.key()])
                delete this->adj[it.key()];
            delete it.key();
        }
    }
}

Node * Graph::get_root() {
    return this->root;
}

void Graph::add_edge(Node * from, Node * to) {
    if (this->n_nodes == 0)
        this->root = from;
    if (to) {
        if (this->adj.find(from) == this->adj.end()) { // Key not found
            this->n_nodes++;
            this->adj[from] = new QList < Node * > ;
        } else if (this->adj[from] == nullptr) {
            this->adj[from] = new QList < Node * > ;
        }
        this->adj[from]->push_back(to);
        this->n_edges++;
    } else {
        if (this-> adj.find(from) == this->adj.end()) { //Key not found
            this-> adj[from] = nullptr;
            this-> n_nodes++;
        }
    }
}


//This function using iterative DFS
void Graph::print() {
    QStack < Node * > s;
    s.push(this->root);
    QHash < Node * , bool > visited;
    visited[root] = true;
    while (!s.empty()) {
        Node * current = s.top();
        s.pop();
        qDebug() << current->name << ' ';
        if (this->adj[current]) {
            for (auto it = this-> adj[current]->begin(); it != this->adj[current]->end(); it++) {
                if (visited.find( * it) == visited.end()) {
                    visited[ * it] = true;
                    s.push( * it);
                }
            }
        }
    }
    qDebug() << '\n';
}

/*
{
    "name": "mohamed",
    "hobbies": {
        "swimming": "true"
    }
}
*/

//QString Graph::_convert_to_json(Node* node, QString& json ,QHash<Node*, bool>& visited, int level, bool last){
//    QString tab = "";
//    for(int i = 0; i < level; i++)
//        tab+='\t';
//    if (this->adj.find(node) == this->adj.end() || !this->adj[node]){ // leaf node
//        json = json + tab + QString("\"") + node->name + QString("\"") + QString(": ")+ QString("\"") + node->value + QString("\",\n");
//    } else {
//        json = json + tab + QString("\"") + node->name + QString("\"") + QString(": ")+ QString("{\n");
//    }

//    visited[node] = true;
//    if(this->adj[node]){
//        for(auto it = this->adj[node]->begin(); it != this->adj[node]->end(); it++){
//            if(visited.find(*it) == visited.end()){
//                if (it + 1 == this->adj[node]->end() && this->adj[node]->size() != 1 ) // Last and not the only one
//                    this->_convert_to_json(*it, json, visited, level+1, true);
//                else
//                    this->_convert_to_json(*it, json, visited, level+1, false);
//            }
//        }
//    }
//    // if last ems7, else mtms74
//    if (!(this->adj.find(node) == this->adj.end() || !this->adj[node])){ //Not a leaf node
//        if (last){
//            json = json.left(json.length()-2);
//            json += QString("\n") + tab + QString("}\n");
//        } else {
//            json += tab + QString("},\n");
//        }
//        // if (json[json.length()-2] == ','){
//        //     json = json.left(json.length()-2);
//        //     json += QString("\n") + tab + QString("}\n");
//        // } else {
//        //     json += tab + QString("}\n");
//        // }

//    }
//    return json;
//}


//QString Graph::convert_to_json(){
//    QHash<Node*, bool> visited;
//    QString json = "{\n";
//    return this->_convert_to_json(this->root, json, visited, 1, true) + QString("}");
//}


void Graph::_convert_to_json(Node * node, int& tab, QString & s, bool last, bool array, bool f_arr, bool l_arr) {
    QString t = "    ";
    if (array && f_arr) {
        if (s[s.length() - 2] == '{') {
            s = s.left(s.length() - 2);
        }
        s += "[\n";
        tab++;
        for (int i = 0; i < tab; i++)
            s += t;
    } else {
        for (int i = 0; i < tab; i++)
            s += t;
    }


    if (!array) // part of an array and doesn't have value
        s += "\"" + QString(node->name) + "\": ";



    if (node->self_closing) {
        s += "{},\n";
        return;
    } else if (node->value != "") {
        s += "\"" + QString(node->value) + "\",\n";
        if (array && l_arr) {
            if (s[s.length() - 2] == ',') {
                s = s.left(s.length() - 2);
                s += "\n";
            }
            for (int i = 0; i < tab; i++)
                s += t;
            if(last)
                s += "]\n";
            else
                s += "],\n";
            tab--;
        }
    } else if (this->adj[node]) {
        tab++;
        s += "{\n";
        for (auto i = this->adj[node]->begin(); i != this->adj[node]->end(); i++) {
            if ((i + 1) == this->adj[node]->end()) { // Last child
                if ((i - 1) >= this->adj[node]->begin()) { // Last child isn't the only one
                    if (( * i)->name == ( * (i - 1))->name) { // same tag as previous one
                        _convert_to_json( * i, tab, s, true, true, false, true);
                    }
                    else
                        _convert_to_json( * i, tab, s, true, false, false, false);
                } else
                    _convert_to_json( * i, tab, s, true, false, false, false);
            } else { // Not the last child
                if (i == this ->adj[node]->begin()) { // First child
                    if (( * i)->name == ( * (i + 1))->name) { // Same tag as the next one
                        node->parent_of_array = true;
                        _convert_to_json( * i, tab, s, false, true, true, false);
                    }
                    else
                        _convert_to_json( * i, tab, s, false, false, false, false);
                } else { // Not the first
                    if (( * i)->name == ( * (i + 1))->name && ( * i)->name == ( * (i - 1))->name) { // At the middle of the array
                        _convert_to_json( * i, tab, s, false, true, false, false);
                    }
                    else if (( * i)->name == ( * (i + 1))->name) { // At the begining of the array
                        node->parent_of_array = true;
                        _convert_to_json( * i, tab, s, false, true, true, false);
                    }
                    else if (( * i)->name == ( * (i - 1))->name) { // At the end of the array
                        _convert_to_json( * i, tab, s, false, true, false, true);
                    }
                    else // Not in array
                        _convert_to_json( * i, tab, s, false, false, false, false);
                }

            }
        }

        if (s[s.length() - 2] == ',' && s[s.length() - 3] != ']') {
            s = s.left(s.length() - 2);
            s += "\n";
        }
        tab--;

        if (last){
            if (!node->parent_of_array){
                for (int i = 0; i < tab; i++)
                    s += t;
                s += "}\n";
            } else if (s[s.length() - 3] != ']' && s[s.length() - 2] != ']'){
                    for (int i = 0; i < tab; i++)
                        s += t;
                    s += "]\n";
            }
        } else {
            if (!node->parent_of_array){
                for (int i = 0; i < tab; i++)
                    s += t;
                s += "},\n";
            } else if (s[s.length() - 3] != ']' && s[s.length() - 2] != ']'){
                    for (int i = 0; i < tab; i++)
                        s += t;
                    s += "],\n";
            }
        }
    }
}


QString Graph::convert_to_json() {
    QString s = "";
    int tab = 1;
    _convert_to_json(this->root, tab, s, true, false, false, false);
    QString json = "{\n" + s + QString("}");
    return json;
}

void Graph::_beautify_xml(Node * node, int tab, QString & s) {
    QString t = "    ";
    for (int i = 0; i < tab; i++)
        s += t;
    s += "<" + QString(node->name);
    if (node->properties != "") {
        s += QString(node->properties) + ">\n";
        if (node->self_closing)
            return;
    } else
        s += ">\n";
    tab++;
    if (node->value != "") {
        for (int i = 0; i < tab; i++)
            s += t;
        s += QString(node->value) + "\n";
        tab--;
        for (int i = 0; i < tab; i++)
            s += t;
        s += "</" + QString(node->name) + ">\n";
    } else if (this->adj[node]) {
        for (auto i = this->adj[node]->begin(); i != this->adj[node]->end(); i++) {
            if ((i + 1) == this-> adj[node]->end())
                _beautify_xml( * i, tab, s);
            else
                _beautify_xml( * i, tab, s);
        }
        tab--;
        for (int i = 0; i < tab; i++)
            s += t;
        s += "</" + QString(node->name) + ">\n";
    }
}

QString Graph::beautify_xml() {
    QString s = "";
    _beautify_xml(this->root, 0, s);
    return s;
}

Graph build_tree(QString xml_file) {
    Graph tree;
    QStack < Node * > tags;
    QString current_tag = "";
    QString tag_value = "";
    QString properties = "";
    bool end = false;

    long long len = xml_file.length();
    for (long long i = 0; i < len;) {
        // ignore white spaces
        while (xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f') {
            if (i < len - 1)
                i++;
            else {
                // throw EOF
                end = true;
                break;
            }
        }

        // ignore comments and headings
        if (xml_file[i] == '<' && (xml_file[i + 1] == '?' || xml_file[i + 1] == '!')) {
            while (xml_file[i++] != '>');
        }

        // get tag name and the properties
        if (xml_file[i] == '<' && xml_file[i + 1] != '/') {
            current_tag = QString("");
            tag_value = QString("");
            properties = QString("");

            while (xml_file[i] != '>') {
                if (xml_file[i] == '<') {
                    i++;
                    continue;
                } else if (xml_file[i] == ' ') {
                    while (xml_file[i] != '>') {
                        properties += xml_file[i++];
                    }
                    if (xml_file[i] == '>') {
                        i++;
                        break;
                    }
                } else
                    current_tag += xml_file[i++];
            }
            if (xml_file[i] == '>')
                i++;

            while ((current_tag.length() > 0) && (current_tag[current_tag.length() - 1] == '\n' || current_tag[current_tag.length() - 1] == '\t' || current_tag[current_tag.length() - 1] == ' '))
                current_tag = current_tag.left(current_tag.length() - 1);
            while ((properties.length() > 0) && (properties[properties.length() - 1] == '\n' || properties[properties.length() - 1] == '\t' || properties[properties.length() - 1] == ' '))
                properties = properties.left(properties.length() - 1);

            // add the edges to the graph
            Node * parent;
            if ((properties.length() > 0) && (properties[properties.length() - 1] == '/'))
                parent = new Node(current_tag.trimmed(), QString(""), properties, true);
            else
                parent = new Node(current_tag.trimmed(), QString(""), properties, false);

            if (!tags.empty())
                tree.add_edge(tags.top(), parent);
            if (!parent-> self_closing)
                tags.push(parent);

            // ignore white spaces
            while (xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f') {
                if (i < len - 1)
                    i++;
                else {
                    // throw EOF
                    end = true;
                    break;
                }
            }

            // get the tag value
            if (xml_file[i] != '<') {
                while (xml_file[i] != '<') {
                    tag_value += xml_file[i++];
                }
                while ((tag_value.length() > 0) && (tag_value[tag_value.length() - 1] == '\n' || tag_value[tag_value.length() - 1] == '\t' || tag_value[tag_value.length() - 1] == ' '))
                    tag_value = tag_value.left(tag_value.length() - 1);

                Node * child = tags.top();
                child->value = tag_value;

                tags.pop(); // after the content, the closed tag comes so we will pop

                // ignore white spaces
                while (xml_file[i] == ' ' || xml_file[i] == '\n' || xml_file[i] == '\t' || xml_file[i] == '\r' || xml_file[i] == '\v' || xml_file[i] == '\f') {
                    if (i < len - 1)
                        i++;
                    else {
                        // throw EOF
                        end = true;
                        break;
                    }
                }
            }

        }

        // get closing tags
        if (xml_file[i] == '<' && xml_file[i + 1] == '/') {
            QString closed_tag = "";
            while (xml_file[i] != '>') {
                if (xml_file[i] == '/' || xml_file[i] == '<') {
                    i++;
                    continue;
                }
                closed_tag += xml_file[i++];
            }
            i++;

            // pop if it's not poped at the value step
            if (closed_tag == tags.top()->name)
                tags.pop();
        }

        // if white spaces to the end of the file
        if (end) {
            i++;
            end = false;
        }
    }

    return tree;
}
