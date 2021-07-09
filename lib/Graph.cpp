#include "Graph.hpp"
#include <iostream>
#include <stack>

template<typename T>
Graph<T>::Graph(){
    this->n = 0;
}

template<typename T>
void Graph<T>::add_edge(T from,T to){
    if (this->n == 0)
        this->root = from;
    this->adj[from].push_back(to);
    this->n++;
}

// This function using iterative DFS
template<typename T>
void Graph<T>::print(){
    std::stack<T> st;
    st.push(this->root);
    std::unordered_map<T, bool> visited;
    visited[root] = true;
    while(!st.empty()){
        T current = st.top();
        st.pop();
        std::cout<<current<<' ';
        for(T item : this->adj[current]){
            if(visited.find(item) == visited.end()){
                visited[item] = true;
                st.push(item);      
            }
        }   
    }
    std::cout<<'\n';
}

// For testing only
int main(){
    Graph<std::string> g;
    g.add_edge("a", "b");
    g.add_edge("a", "f");
    g.add_edge("f", "g");
    g.add_edge("b", "c");
    g.add_edge("b", "d");
    g.add_edge("b", "e");
    g.print();
}