#pragma once 
#include <list>
#include <unordered_map>

template <typename T>
class Graph{
    int n;
    T root;
    std::unordered_map<T, std::list<T>> adj;
public:
    Graph();
    void add_edge(T from, T to);
    void print();
};

