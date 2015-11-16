#include "main.h"
#include "Graph.h"
#include "SparseMatrix.h"
#include "PageRankSparse.h"
#include <sys/time.h>

using namespace std;

double getTimeMillis() {
   struct timeval  tv;
   gettimeofday(&tv, NULL);

   double time_in_mill = 
               (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; 

   return time_in_mill;

}

int main(int argc, char** argv) {
   double prevTime, nextTime;


   prevTime = getTimeMillis();
   Graph graph = Graph::Graph(string(argv[1]));
   nextTime = getTimeMillis();
   printf("Graph creation took %.2f milliseconds\n", nextTime - prevTime);

   prevTime = getTimeMillis();
   SparseMatrix sm = graph.createSparseMatrix();
<<<<<<< HEAD
   nextTime = getTimeMillis();
   printf("Sparse Matrix creation took %.2f milliseconds\n", nextTime - prevTime);

   int *array;
   array = (int *) malloc(sm.width * sizeof(int));

   prevTime = getTimeMillis();
   pageRank(&sm, array);
   nextTime = getTimeMillis();
   printf("Calculating the Page Rank took %.2f milliseconds\n", nextTime - prevTime);
   int *array;
   array = (int *) malloc(sm.width * sizeof(int));*/
   pageRank(&sm, array);

   for(int i = 0; i < sm.width; i++) {
      printf("%s, %.5e\n", sm.indexToName[array[i]].c_str(), sm.sortedPrestigeVector[i]);
   }

//   graph.printMatrix(sm);
//   for(int i = 0; i < graph.indexToName.size(); i++) {
//      cout << graph.indexToName[i] << endl;
//   }
   //cout << graph.indexToName.size() << endl;
   return 0;
}
