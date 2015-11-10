#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
// #include "Node.h"

using namespace std;

class Graph {
public:
   //The last three parameters are default parameters. If not called with those parameters,
   //function call will default to giving them these values
   Graph(string fileName, bool undirected = 0, bool invert = 0, FILE* writeTo = NULL);
   void scanCSV(string fileName);
   void scanSNAP(string fileName);
   int size();
// private:
   int parseWord(char *fromString, char *toString);
};


#endif //GRAPH_H
