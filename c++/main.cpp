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
   bool invert = false, directed = true;
   if(argc == 4) {
      if(strcmp(argv[2], "false") == 0) {
         directed = false;
      }
      if(strcmp(argv[3], "true") == 0) {
         invert = true;
      }
   }
   cout << "directed: " << directed <<endl;
   cout << "inverted: " << invert << endl;
   prevTime = getTimeMillis();
   Graph graph = Graph::Graph(string(argv[1]), directed, invert);
   nextTime = getTimeMillis();
   printf("Graph creation took %.2f milliseconds\n", nextTime - prevTime);

   prevTime = getTimeMillis();
   SparseMatrix sm = graph.createSparseMatrix();
   nextTime = getTimeMillis();
   printf("Sparse Matrix creation took %.2f milliseconds\n", nextTime - prevTime);

   int *array;
   array = (int *) malloc(sm.width * sizeof(int));

   //graph.printMatrix(sm);
   prevTime = getTimeMillis();
   pageRank(&sm, array);
   nextTime = getTimeMillis();
   printf("Calculating the Page Rank took %.2f milliseconds\n", nextTime - prevTime);

   for(int i = 0; i < sm.width; i++) {
      printf("%s, %.12f\n", sm.indexToName[array[i]].c_str(), sm.sortedPrestigeVector[i]);
   }


//   cout << graph.indexToName.size() << endl;

//
//   for(int i = 0; i < graph.indexToName.size() && i < 10; i++) {
//      cout << graph.indexToName[i] << endl;
//   }
   return 0;
}
