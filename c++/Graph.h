#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <vector>
#include "SparseMatrix.h"

using namespace std;

class Graph {
public:
   //The last three parameters are default parameters. If not called with those parameters,
   //function call will default to giving them these values
   Graph(string fileName, bool undirected = 0, bool invert = 0, FILE* writeTo = NULL);
   void scanFile(string fileName, bool csvFile);
   int size();
   SparseMatrix createSparseMatrix();
   void printMatrix(SparseMatrix sm);
private:
   int parseWord(char *fromString, char *toString);
};


#endif //GRAPH_H
