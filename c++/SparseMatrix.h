#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

using namespace std;

class SparseMatrix {
public:
   float *cooValA;
   int *cooRowIndA;
   int *cooColIndA;
   int width, nnz;
   SparseMatrix(float *cVA, int *cRIA, int *cCIA, int w, int n);
};

SparseMatrix::SparseMatrix(float *cVA, int *cRIA, int *cCIA, int w, int n) {
   cooValA = cVA;
   cooRowIndA = cRIA;
   cooColIndA = cCIA;
   width = w;
   nnz = n;
}
#endif
