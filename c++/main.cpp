#include "main.h"
#include "Graph.h"
#include "SparseMatrix.h"

using namespace std;

int main(int argc, char** argv) {
    Graph graph = Graph::Graph(string(argv[1]));
   SparseMatrix sm = graph.createSparseMatrix();
   graph.printMatrix(sm);
    return 0;
}