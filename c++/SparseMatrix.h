#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <string>
#include <vector>

using namespace std;

class SparseMatrix {
public:
   float *cooValA;
   int *cooRowIndA;
   int *cooColIndA;
   int *csrRowPtrA;
   int width, nnz;
   int *devRowPtr, *devColInd, *devRowInd;
   float *devVal;
   float *sortedPrestigeVector;
   std::string * indexToName;
   SparseMatrix(float *cVA, int *cRIA, int *cCIA, int w, int n, std::string * iTN);
};

#endif
