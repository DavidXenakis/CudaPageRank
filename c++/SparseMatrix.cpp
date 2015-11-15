//
// Created by Matthew Austin on 11/14/15.
//


#include "SparseMatrix.h"

using namespace std;

SparseMatrix::SparseMatrix(float *cVA, int *cRIA, int *cCIA, int w, int n, std::string * iTN) {
   cooValA = cVA;
   cooRowIndA = cRIA;
   cooColIndA = cCIA;
   width = w;
   nnz = n;
   indexToName = iTN;
}
