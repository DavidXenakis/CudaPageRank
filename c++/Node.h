//
// Created by Matthew Austin on 10/29/15.
//

#ifndef PAGERANK_NODE_H
#define PAGERANK_NODE_H
#include <string>

class Node {
public:
    bool firstVisit = false;
    std::string element;
    double pageRank;
    int value = 0;
    Node* inlinks[10];
    int outlinksSize = 0;
};

#endif //PAGERANK_NODE_H
