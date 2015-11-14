
/*
M: sparse adjacency matrix, 
get(M, i, j) = the link from 'j' to 'i', such that for all 'j':
   sum(i, get(M, i, j)) = 1

d: damping factor

v_quadratic_error: quadratic error allowed

returns a dense vector of ranks
*/

#include "SparseMatrix.h"
#define QUADRATIC_ERROR .001
#define DAMPING_FACTOR .85

float * PageRankSparse::pagerank(SparseMatrix M) {
   int n = M.numRows();
   float *v = malloc(sizeof(float) * n);
   std::fill(v, v+n, 1/n);

   float *last_v = malloc(sizeof(float) * n);

   M_hat = sparse_MXplusB(M, DAMPING_FACTOR, (1-DAMPING_FACTOR)/n);

   float error;
   do {
      last_v = v;
      v = matrixVectorMultiply(M_hat, v);

      error = vectorSubtractAndNormalize2(v, last_v, n);
      //    = sum( (v[i] - last_v[i])^2 ) ^ .5 = norm(v - last_v, 2)

   } while (error > QUADRATIC_ERROR);


   free(v);
   free(M_hat);
   free(last_v);
}

/* TODO

float *sparse_MatrixVectorMultiply(SparseMatrix M, float *v, int n)

SparseMatrix *sparse_MXplusB(SparseMatrix X, float m, float b, int n)

float vectorSubtractAndNormalize2(float *v, float *last_v, int n)

*/


//TODO test coo2csr on test data
//TODO test csrmv on test data
float * PageRankSparse::sparse_MatrixVectorMultiply(SparseMatrix M, float *v, int n) {
#ifdef GPU
  // cusparseStatus_t cusparseXcoo2csr(cusparseHandle_t handle, const int *cooRowInd,
   //                          int nnz, int m, int *csrRowPtr, cusparseIndexBase_t idxBase);
   int *devRowInd, *devRowPtr, *devColInd;
   float *devVals, *devVect, *devVectNew;

   cusparseHandle_t handle;
   cusparseStatus_t status;
   status = cusparseCreate();
   if (status != CUSPARSE_STATUS_SUCCESS) {
      cout << "Failed to set up cusparse";
      exit(1);
   }

   cudaMalloc(&devRowInd, M.nnz * sizeof(int));
   cudaMemcpy(devRowInd, M.cooRowInd, M.nnz * sizeof(int), cudaMemcpyHostToDevice); 
   cudaMalloc(&devColInd, M.nnz * sizeof(int));
   cudaMemcpy(devColInd, M.cooColInd, M.nnz * sizeof(int), cudaMemcpyHostToDevice); 
   cudaMalloc(&devVals, M.nnz * sizeof(float));
   cudaMemcpy(devVals, M.cooValA, m.nnz * sizeof(float), cudaMemcpyHostToDevice);

   int *devRowPtr;
   cudaMalloc(&devRowPtr, M.nnz * sizeof(int));

   status = cusparseXcoo2csr(handle, devRowInd, M.nnz, M.width, devRowPtr, CUSPARSE_INDEX_BASE_ZERO);

   cusparseOperation_t op = CUSPARSE_OPERATION_TRANSPOSE;
   cusparseMatDescr_t descr = 0;
   status = cusparseCreateMatDescr(&descr);

   cudaMalloc(&devVect, n * sizeof(float));
   cudaMemcpy(devVect, v, n * sizeof(float), cudaMemcpyHostToDevice);
   cudaMalloc(&devVect, n * sizeof(float));

   status = cusparseScsrmv(handle, op, M.width, M.width, M.nnz, 1.0,
         descr, devVals, devRowPtr, devColInd, devVect, (float *) 0, devVectNew); 
                                                                           
#endif

}
