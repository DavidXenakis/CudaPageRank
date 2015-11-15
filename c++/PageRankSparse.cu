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
#include <thrust/reduce.h>
#include <thrust/sort.h>
#include "cusparse.h"

#define QUADRATIC_ERROR .0001
#define DAMPING_FACTOR .85
#define THREADS_PER_BLOCK 1024
#ifdef MIC
float vectorSubtractAndNormalize2(float *v, float *last_v, int n) {
   float sum = 0;

   #pragma omp parallel for reduction(+:sum)
   for (int i = 0; i < n; i++) {
      sum = sum + (v[i] - last_v[i]) * (v[i] - last_v[i]);
   }
}
#endif

#ifdef GPU
__global__ void subtractAndSquare(float *vectNew, float *vect, float *dest, int *n) {
   int idx = blockIdx.x * blockDim.x + threadIdx.x;
   if (idx < n[0])
      dest[idx] = (vectNew[idx] - vect[idx]) * (vectNew[idx] - vect[idx]);
}
#endif

#ifdef GPU
float vectorSubtractAndNormalize2(float *devVectNew, float *devVect, int n) {
   //to be parallelized
   float sum = 0;

   //allocate on gpu
   float *d_difference;
   cudaMalloc(&d_difference, n * sizeof(float));
   int *d_n;
   cudaMalloc(&d_n, sizeof(int));

   //copy to gpu
   cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);

   int numBlocks = (n + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(THREADS_PER_BLOCK, 1);

   //launch kenerl
   subtractAndSquare <<<gridDim, blockDim>>> (devVectNew, devVect, d_difference, d_n);

   //copy result from gpu
   float *difference = (float *) malloc(n * sizeof(float));
   cudaMemcpy(difference, d_difference, n * sizeof(float), cudaMemcpyDeviceToHost);

   //free from gpu
   cudaFree(d_difference);
   cudaFree(d_n);

   sum = thrust::reduce(difference, difference + n);
/*
   #else //neither mic nor gpu
   for (int i = 0; i < n; i++) {
      sum += (v[i] - last_v[i]) * (v[i] - last_v[i]);
   }
   #endif
*/
   return sqrtf(sum);
}
#endif

void convertCOO2CSR(SparseMatrix *M, cusparseHandle_t handle) {
   int *devRowPtr;

   //cusparseStatus_t status;
   cudaMalloc(&devRowPtr, M->nnz * sizeof(int));

   cusparseXcoo2csr(handle, M->devRowInd, M->nnz, M->width, devRowPtr, CUSPARSE_INDEX_BASE_ZERO);
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

#ifdef GPU
void sparse_MatrixVectorMultiply(SparseMatrix *M, cusparseHandle_t handle, float *vect, float *newVect, float **devVect, float **devVectNew) {
   float alpha = 1.0f;
   float beta = 0.0f;
   int vectWidth = M->width;

   cusparseMatDescr_t descr;
   cusparseCreateMatDescr(&descr);
   cusparseSetMatType(descr,CUSPARSE_MATRIX_TYPE_GENERAL);
   cusparseSetMatIndexBase(descr,CUSPARSE_INDEX_BASE_ZERO);

   // Check to see if the prestige vector is already on card
   if ( *devVect == NULL ) {
      cudaMalloc(devVect, vectWidth * sizeof(float));
      cudaMemcpy(*devVect, vect, vectWidth * sizeof(float), cudaMemcpyHostToDevice);
   } 
   if ( *devVectNew == NULL ) {
      cudaMalloc(devVectNew, vectWidth * sizeof(float));
   }

   cusparseOperation_t op = CUSPARSE_OPERATION_NON_TRANSPOSE;
   cusparseScsrmv(handle, op, M->width, M->width, M->nnz, &alpha,
         descr, M->devVal, M->devRowPtr, M->devColInd, *devVect, &beta, *devVectNew); 

   cudaMemcpy(newVect, *devVectNew, vectWidth * sizeof(float), cudaMemcpyDeviceToHost);
}
#endif

__global__ void MXplusBKernel(int n, float m, float *x, float b) {
   int i = blockIdx.x*blockDim.x + threadIdx.x;
   if (i < n) {
      x[i] = m * x[i] + b;
   }
}

void sparse_MXplusB(SparseMatrix *M, float m, float b) {
   int numBlocks = (M->nnz + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(THREADS_PER_BLOCK, 1);

   MXplusBKernel<<<gridDim, blockDim>>>(M->nnz, m, M->devVal, b);
}

void pageRank(SparseMatrix *M, int *array) {
   int n = M->width;
   float *vect = (float *) malloc(sizeof(float) * n);
   std::fill(vect, vect + n, 1.0f / n);

   float *newVect = (float *) malloc(sizeof(float) * n);
   float *devVect = NULL;
   float *devVectNew = NULL;

   cusparseHandle_t handle;
   cusparseCreate(&handle);

   putMatOnDevice(M, handle);
   sparse_MXplusB(M, DAMPING_FACTOR, (1.0f - DAMPING_FACTOR) / n);
   convertCOO2CSR(M, handle);
   //int iter = 0;

   float error;
   do {
      sparse_MatrixVectorMultiply(M, handle, vect, newVect, &devVect, &devVectNew);

      error = vectorSubtractAndNormalize2(devVectNew, devVect, n);

      // Swap old and new vectors to reuse space
      float *temp = newVect;
      newVect = vect;
      vect = temp;

      temp = devVectNew;
      devVectNew = devVect;
      devVect = temp;
      //printf("Iteration: %d... Error: %.4f\n", iter++, error);
      
   } while (error > QUADRATIC_ERROR);

   cudaFree(devVectNew);
   cudaFree(M->devVal);
   cudaFree(M->devRowPtr);
   cudaFree(M->devColInd);
   
   free(newVect);

   for(int i = 0; i < M->width; i++) {
      array[i] = i;
   }

   thrust::stable_sort_by_key(vect, vect + M->width, array, thrust::greater<float>());

   M->sortedPrestigeVector = vect;
}
/*
int main() {
   float vals[7] = {.5, .5, .5, .5, .5, 1, .5};
   int rowInd[7] = {0, 0, 1, 1, 2, 2, 3};
   int colInd[7] = {0, 1, 1, 2, 0, 3, 2};

   float vals2[9] = {1.0, .25, 1, 1, .5, .25, .25, .25, .5};
   int rowInd2[9] = {0, 1, 1, 1, 1, 2, 3, 4, 4};
   int colInd2[9] = {1, 0, 2, 3, 4, 0, 0, 0, 4};

   SparseMatrix m (vals, rowInd, colInd, 4, 7);
   SparseMatrix m2(vals2, rowInd2, colInd2, 5, 9);

   pageRank(m);
   pageRank(m2);
     
   return 0;
}*/
