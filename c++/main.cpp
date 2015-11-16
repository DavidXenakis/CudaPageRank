#include "main.h"
#include "Graph.h"
#include "SparseMatrix.h"
#include "PageRankSparse.h"

using namespace std;

int main(int argc, char** argv) {
   Graph graph = Graph::Graph(string(argv[1]));
   SparseMatrix sm = graph.createSparseMatrix();
   /*int *array;
   array = (int *) malloc(sm.width * sizeof(int));*/
   //pageRank(&sm, array);

   /*for(int i = 0; i < sm.width; i++) {
      printf("%s, %.5e\n", sm.indexToName[array[i]].c_str(), sm.sortedPrestigeVector[i]);
   }*/

   //cout << graph.indexToName.size() << endl;

//
//   for(int i = 0; i < graph.indexToName.size(); i++) {
//      cout << graph.indexToName[i] << endl;
//   }

   cout << "SIZE " << graph.indexToName.size() << endl;
   return 0;
}
