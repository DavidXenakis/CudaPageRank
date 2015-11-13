/*
M: sparse adjacency matrix, 
get(M, i, j) = the link from 'j' to 'i', such that for all 'j':
   sum(i, get(M, i, j)) = 1

d: damping factor

v_quadratic_error: quadratic error allowed

returns a dense vector of ranks
*/

#include "SparseMatrix.h"
#include <stdio.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include "cusparse.h"

#define QUADRATIC_ERROR .001
#define DAMPING_FACTOR .85

using namespace std;


/*
float * pagerank(SparseMatrix M) {
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
*/

void convertCOO2CSR(SparseMatrix *M, cusparseHandle_t handle) {
   int *devRowPtr;

   //cusparseStatus_t status;
   cudaMalloc(&devRowPtr, M->nnz * sizeof(int));

   cusparseXcoo2csr(handle, M->devRowInd, M->nnz, M->width, devRowPtr, CUSPARSE_INDEX_BASE_ZERO);
   //status = cusparseXcoo2csr(handle, M->devRowInd, M->nnz, M->width, devRowPtr, CUSPARSE_INDEX_BASE_ZERO);
   M->devRowPtr = devRowPtr;

   cudaFree(M->devRowInd);
}

void putMatOnDevice(SparseMatrix *M, cusparseHandle_t handle) {
   int *devRowInd, *devColInd;
   float *devVal;
   cudaMalloc(&devRowInd, M->nnz * sizeof(int));
   cudaMemcpy(devRowInd, M->cooRowIndA, M->nnz * sizeof(int), cudaMemcpyHostToDevice); 
   cudaMalloc(&devVal, M->nnz * sizeof(float));
   cudaMemcpy(devVal, M->cooValA, M->nnz * sizeof(float), cudaMemcpyHostToDevice);
   cudaMalloc(&devColInd, M->nnz * sizeof(int));
   cudaMemcpy(devColInd, M->cooColIndA, M->nnz * sizeof(int), cudaMemcpyHostToDevice); 

   M->devRowInd = devRowInd;
   M->devVal = devVal;
   M->devColInd = devColInd;
}

/* TODO

float *sparse_MatrixVectorMultiply(SparseMatrix M, float *v, int n)

SparseMatrix *sparse_MXplusB(SparseMatrix X, float m, float b, int n)

float vectorSubtractAndNormalize2(float *v, float *last_v, int n)


*/
//TODO test coo2csr on test data
//TODO test csrmv on test data
void sparse_MatrixVectorMultiply(SparseMatrix *M, cusparseHandle_t handle, float *vect, float *newVect, float **devVect) {
#ifdef GPU
  // cusparseStatus_t cusparseXcoo2csr(cusparseHandle_t handle, const int *cooRowInd,
   //                          int nnz, int m, int *csrRowPtr, cusparseIndexBase_t idxBase);
   float *devVectNew;
   float alpha = 1.0f;
   float beta = 0.0f;
   int vectWidth = M->width;

   //cusparseStatus_t status;
   cusparseMatDescr_t descr;
   //status = cusparseCreateMatDescr(&descr);
   cusparseCreateMatDescr(&descr);
   cusparseSetMatType(descr,CUSPARSE_MATRIX_TYPE_GENERAL);
   cusparseSetMatIndexBase(descr,CUSPARSE_INDEX_BASE_ZERO);

   // Check to see if the prestige vector is already on card
   if ( *devVect == NULL ) {
      cudaMalloc(devVect, vectWidth * sizeof(float));
      cudaMemcpy(*devVect, vect, vectWidth * sizeof(float), cudaMemcpyHostToDevice);
   } 

   cudaMalloc(&devVectNew, vectWidth * sizeof(float));

   cusparseOperation_t op = CUSPARSE_OPERATION_NON_TRANSPOSE;
   cusparseScsrmv(handle, op, M->width, M->width, M->nnz, &alpha,
         descr, M->devVal, M->devRowPtr, M->devColInd, *devVect, &beta, devVectNew); 

   cudaMemcpy(newVect, devVectNew, vectWidth * sizeof(float), cudaMemcpyDeviceToHost);
   *devVect = devVectNew;

   cudaFree(*devVect);
#endif
}

__global__ void MXplusBKernel(int n, float m, float *x, float b) {
   int i = blockIdx.x*blockDim.x + threadIdx.x;
   if (i < n) {
      x[i] = m * x[i] + b;
   }
}

void sparse_MXplusB(SparseMatrix *M, cusparseHandle_t handle, float m, float b) {
   int numBlocks = (M->nnz + 1023) / 1024;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(1024, 1);

   MXplusBKernel<<<gridDim, blockDim>>>(M->nnz, m, M->devVal, b);
}


int main() {
   float vals[7] = {1.0, 4.0, 2.0, 3.0, 5.0, 7.0, 9.0};
   int rowInd[7] = {0, 0, 1, 1, 2, 2, 3};
   int colInd[7] = {0, 1, 1, 2, 0, 3, 2};

   int n = 4;
   float vect[4] = {1.0, 2.0, 3.0, 4.0};
   float newVect[4];
   float *devVect = NULL;

   cusparseHandle_t handle;
   //cusparseStatus_t status;
   cusparseCreate(&handle);
   //status = cusparseCreate(&handle);
   SparseMatrix m (vals, rowInd, colInd, n, 7);

   for (int i = 0; i < n; i++) {
      printf("%.2f ", vect[i]);
   }
   printf("\n");

   putMatOnDevice(&m, handle);
   sparse_MXplusB(&m, handle, DAMPING_FACTOR, (1.0f - DAMPING_FACTOR) / n);
   convertCOO2CSR(&m, handle);

   //float error = 1000000000000.0f;

   // while (error > QUADRATICERROR) {
      sparse_MatrixVectorMultiply(&m, handle,  vect, newVect, &devVect);
      //Calculate error

      //swap pointers
      /*
      float *temp = newVect;
      newVect = vect;
      vect = temp;
      */
   // }
      
   for (int i = 0; i < n; i++) {
      printf("%.2f ", newVect[i]);
      // SHOULD BE [9 13 33 27]
   }
   printf("\n");
   return 0;
}
