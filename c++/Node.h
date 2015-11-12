//
// Created by Matthew Austin on 10/29/15.
//

#ifndef PAGERANK_NODE_H
#define PAGERANK_NODE_H

#include <string>
#include <queue>

/*class Node {
public:
    bool firstVisit;
    std::string element;
    double pageRank;
    int value;
    Node* inlinks[10];
    int outlinksSize;
};*/

class Node {
public:
    std::priority_queue<int> edges;
    Node(int aIndexint);
    Node();
    int index;
    int getIndex();
};

Node::Node(int aIndex) {
   index = aIndex;
}

Node::Node(void) {
   index = -1;
};

int Node::getIndex() {
   return index;
}


#endif //PAGERANK_NODE_H
