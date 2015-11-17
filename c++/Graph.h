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
   Graph(string fileName, bool directed = true);
   void scanFile(string fileName, bool csvFile, bool directed);
   int size();
   SparseMatrix createSparseMatrix();
   void printMatrix(SparseMatrix sm);
   vector<string> indexToName;
   bool findInVector(string str);
private:
    char* parseWord(char *fromString, char *toString);
};


#endif //GRAPH_H
