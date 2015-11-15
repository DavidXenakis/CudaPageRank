#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

class SparseMatrix {
public:
   float *cooValA;
   int *cooRowIndA;
   int *cooColIndA;
   int *csrRowPtrA;
   int width, nnz;
   int *devRowPtr, *devColInd, *devRowInd;
   float *devVal;
   SparseMatrix(float *, int *, int *, int , int );
};

#endif
