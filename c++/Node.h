//
// Created by Matthew Austin on 10/29/15.
//

#ifndef PAGERANK_NODE_H
#define PAGERANK_NODE_H
#include <string>

class Node {
public:
    bool firstVisit;
    std::string element;
    double pageRank;
    int value;
    Node* inlinks[10];
    int outlinksSize;
};

#endif //PAGERANK_NODE_H
