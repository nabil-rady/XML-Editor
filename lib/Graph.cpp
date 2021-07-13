#include "Graph.hpp"

#include <QStack>

#include <QDebug>
#include <QByteArray>

Node::Node(QString name, QString value, QString properties, bool self_closing) {
    this->name = name;
    this->value = value;
    this->properties = properties;
    this->self_closing = self_closing;
    this->parent_of_array = false;
    this->array_of_leaf_nodes = false;
    this->oppened = false;
}

Match_Pointer::Match_Pointer(qsizetype begin, qsizetype length){
    this->begin = begin;
    this->length = length;
}

Graph::Graph(){
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

void attributes(QString& s, Node* node, int& tab){
    QString t = "    ";
    QString properties = node->properties;
    s += "{\n";
    tab++;

    for (int i = 0; i < properties.length(); i++){
        if (i < properties.length() - 2 && properties[i] == ' '){
            i++;
            for (int i = 0; i < tab; i++)
                s += t;
            s += "\"@";
            while(properties[i] != '=')
                s += properties[i++];

            if(properties[i] == '='){
                i += 2;
                s += "\": \"";
            }

            while(properties[i] != '\"')
                s += properties[i++];

            if(properties[i] == '\"'){
                s += "\",\n";
            }
        }
    }
}

void Graph::_convert_to_json(Node * node, int& tab, QString & s, bool last, bool array, bool f_arr, bool l_arr) {
    QString t = "    ";
    if (array && f_arr) { // First element in the array
        if (s[s.length() - 2] == '{') { // this object is converted to array
            s = s.left(s.length() - 2);
            tab--;
        } else if (s[s.length() - 2] == ','){ // if there is no key
            for (int i = 0; i < tab; i++)
                s += t;
            s += "\"" + QString(node->name) + "\": ";
        }
        s += "[\n";
        tab++;
        for (int i = 0; i < tab; i++)
            s += t;
    } else { // not array or array but not the first element
        for (int i = 0; i < tab; i++)
            s += t;
    }

    if (!array) // the array key should be exists at this point
        s += "\"" + QString(node->name) + "\": ";

    if (node->self_closing) { // if self closing tag
        if (array && l_arr){
            s += "{}\n";
            tab--;
            for (int i = 0; i < tab; i++)
                s += t;
            s += "],\n";
        } else
            s += "{},\n";
        return;
    } else if (node->value != "") { // if there is a content
        if (array && l_arr){
            s += "\"" + QString(node->value) + "\"\n";
            tab--;
            for (int i = 0; i < tab; i++)
                s += t;
            s += "],\n";
        }
        else
            s += "\"" + QString(node->value) + "\",\n";

    } else if (this->adj[node]) { // if the node has children
        tab++;
        s += "{\n";
        node->object = true;
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
                        node->object = false;
                        if(( * (i + 1))->value.length() > 0)
                            node->array_of_leaf_nodes = true;
                        _convert_to_json( * i, tab, s, false, true, true, false);
                    }
                    else
                        _convert_to_json( * i, tab, s, false, false, false, false);
                } else { // Not the first
                    if (( * i)->name == ( * (i + 1))->name && ( * i)->name == ( * (i - 1))->name) { // At the middle of the array
                        _convert_to_json( * i, tab, s, false, true, false, false);
                    }
                    else if (( * i)->name == ( * (i + 1))->name) { // At the begining of the array
                        // No need to set the node as parent of array as it holds some other children out side the array
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

        if (s[s.length() - 2] == ',' && (s[s.length() - 3] != ']' || (s[s.length() - 3] == ']' && node->object))) {
            // clear the last comma before closing the brackets
            s = s.left(s.length() - 2);
            s += "\n";
        }

        if(node->parent_of_array && !node->array_of_leaf_nodes){ // close the square brackets of an array
            tab--;
            for (int i = 0; i < tab; i++)
                s += t;

            if (last){
                s += "]\n";
            } else
                s += "],\n";
        }

        if (node->object) { // close the curly braces of an object
            tab--;
            for (int i = 0; i < tab; i++)
                s += t;

            if (node->parent_of_array) {
                for (int i = 0; i < tab; i++)
                    s += t;
            }
            if (last) {
                s += "}\n";
            } else
                s += "},\n";
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

Match_Pointer _largest_match(QByteArray::iterator window, QByteArray::iterator look_ahead_buffer){
    QByteArray::iterator i = window, j = look_ahead_buffer;
    QByteArray longest_match("");
    QByteArray current_match("");
    Match_Pointer current_match_pointer(0, 0);
    Match_Pointer longest_match_pointer(0, 0);
    bool in_a_matching_sequence = false;
    while(i != look_ahead_buffer){
        if (*i == *j){
            if(!in_a_matching_sequence)
                current_match_pointer.begin = static_cast<qsizetype>(look_ahead_buffer - i);
            in_a_matching_sequence = true;
            current_match.push_back(*i);
            i++;
            j++;
        }
        else{
            if(in_a_matching_sequence){
                if (current_match.length() > longest_match.length()){
                    longest_match = current_match;
                    longest_match_pointer.begin = current_match_pointer.begin;
                    longest_match_pointer.length = longest_match_pointer.begin - static_cast<qsizetype>(look_ahead_buffer - i);
                }
                current_match = QByteArray("");
                current_match_pointer.begin = 0;
                current_match_pointer.length = 0;
                j = look_ahead_buffer;
                in_a_matching_sequence = false;
            }
            else
                i++;
        }
    }
    if (current_match.length() > longest_match.length()){
        longest_match = current_match;
        longest_match_pointer.begin = current_match_pointer.begin;
        longest_match_pointer.length = longest_match_pointer.begin - static_cast<int>(look_ahead_buffer - i);
    }
    return longest_match_pointer;
}

QString minify(QString file){
    QString minified_file = "";
    for(int i = 0; i < file.length(); i++){
        if ((i < file.length() - 1) && (file[i] == ' ' && file[i + 1] != ' ')){
            minified_file += file[i];
            continue;
        } else if ((i < file.length() - 1) && (file[i] == ' ' && file[i + 1] == ' ')){
            continue;
        }
        if(file[i] == '\n' || file[i] == '\t' || file[i] =='\r' || file[i] == '\v' || file[i] == '\f')
            continue;
        minified_file += file[i];
    }
    return minified_file;
}

QByteArray compress(QString& file){
    QByteArray minified_file(minify(file).toUtf8());
    QByteArray compressed_byte_array("");
    for(QByteArray::iterator it = minified_file.begin(); it != minified_file.end(); it++){
        QByteArray::iterator window = it - 255 > minified_file.begin() ? it - 255 : minified_file.begin();
        Match_Pointer match = _largest_match(window, it);
        compressed_byte_array.push_back(match.begin);
        if (match.begin != 0){
            it += match.length - 1;
            compressed_byte_array.push_back(match.length);
        }
        else{
            compressed_byte_array.push_back(*it);
        }
    }
    return compressed_byte_array;
}

QString decompress(QByteArray& compressed_byte_array){
    QByteArray file("");
    unsigned int file_index = 0;
    for(qsizetype i = 0; i < compressed_byte_array.length();){
        qsizetype begin = static_cast<qsizetype>(static_cast<unsigned char>(compressed_byte_array[i]));
        if (begin == 0){
            file.push_back(compressed_byte_array[i+1]);
            file_index++;
            i+=2;
        }
        else {
           int prev_file_index = file_index;
           qsizetype length = static_cast<qsizetype>(static_cast<unsigned char>(compressed_byte_array[i+1]));
           for(unsigned int j = prev_file_index - begin; j < prev_file_index - begin + length; j++){
               file.push_back(file[j]);
               file_index++;
           }
           i+=2;
        }
    }
    return QString(file);
}
