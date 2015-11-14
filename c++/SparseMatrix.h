#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

class SparseMatrix {
public:
   SparseMatrix(float *cVA, int *cRIA, int *cCIA, int w, int n);
    float *cooValA;
    int *cooRowIndA;
    int *cooColIndA;
    int width, nnz;
};


#endif
