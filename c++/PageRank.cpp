//
// Created by Matthew Austin on 10/29/15.
//

#include "PageRank.h"
#include <cmath>
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))


double d = 0.85;
int size = 0.0;
long startTime = 0.0;
long endTime = 0.0;

int numIter = 0;
Node nodes[10];

double PageRank::pageRank(Node &node) {
    double newPagerank = 0.0;
    double sum = 0.0;
    if(node.firstVisit == false) {
        newPagerank = node.pageRank = 1.0 / size;
        node.firstVisit = true;
        return newPagerank;
    }

    for(int i = 0; i < ARRAY_SIZE(node.inlinks); i++) {
        sum += (1.0 / (*node.inlinks[i]).outlinksSize) * (*node.inlinks[i]).pageRank;
    }
    newPagerank = (1 - d) * (1.0 / size) + d*sum;

    return newPagerank;
}

void PageRank::getPageRanks() {
    bool done = false;
    double oldSum;
    double newSum;
    bool firstIter = false;
    while(!done) {
        numIter++;
        oldSum = 0.0;
        newSum = 0.0;
        for(int i = 0; i < ARRAY_SIZE(nodes); i++) {
            Node node = nodes[i];
            double num = PageRank::pageRank(node);
            newSum += num;
            oldSum += node.pageRank;
            node.pageRank = num;
        }
        if(std::abs(newSum - oldSum) < 0.0001 && !firstIter) {
            done = true;
        }
        firstIter = false;
    }
}