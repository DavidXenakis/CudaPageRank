//
// Created by Matthew Austin on 10/29/15.
//

#ifndef PAGERANK_NODE_H
#define PAGERANK_NODE_H

#include <string>
#include <queue>
#include <unordered_map>


class CompareEdges {
public:
    bool operator()(int t1, int t2)
    {
       if (t1 > t2) return true;
       return false;
    }
};

class Node {
public:
    std::priority_queue<int, vector<int>, CompareEdges> edges;
    std::unordered_map<int, int> edgeMap;
    Node(int aIndex);
    Node();
    int numOutlinks;
    int index;
};

Node::Node(int aIndex) {
   index = aIndex;
   numOutlinks = 0;
}

Node::Node(void) {
   index = -1;
   numOutlinks = 0;
};

#endif //PAGERANK_NODE_H
