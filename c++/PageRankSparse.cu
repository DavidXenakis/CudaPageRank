/**
 * Implementation by David Xenakis and Max Bendick
 */
#include "SparseMatrix.h"
#include <stdio.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include <thrust/reduce.h>
#include <thrust/sort.h>
#include "cusparse.h"

#define QUADRATIC_ERROR .001
#define DAMPING_FACTOR .85
#define THREADS_PER_BLOCK 1024

// This function does vector subtraction and absolute value
__global__ void subtractAndSquare(float *vectNew, float *vect, float *dest, int n) {
   int idx = blockIdx.x * blockDim.x + threadIdx.x;
   float val;
   if (idx < n){
      val = (vectNew[idx] - vect[idx]);
      dest[idx] = val < 0 ? val * -1.0 : val;
   }
}

// This function finds the total error across iterations
float vectorSubtractAndNormalize2(float *devVectNew, float *devVect, float *devDifference, float *difference, int n) {
   //to be parallelized
   float sum = 0;

   //allocate on gpu

   int numBlocks = (n + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(THREADS_PER_BLOCK, 1);

   //launch kenerl
   subtractAndSquare <<<gridDim, blockDim>>> (devVectNew, devVect, devDifference, n);

   //copy result from gpu
   cudaMemcpy(difference, devDifference, n * sizeof(float), cudaMemcpyDeviceToHost);

   sum = thrust::reduce(difference, difference + n);

   return sum;
}

// This function converts our COO sparse matrix into CSR format
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

// This function finds the new prestige vector by multiplying the matrix times the prestige vector
void sparse_MatrixVectorMultiply(SparseMatrix *M, cusparseHandle_t handle, float *devVect, float *devVectNew) {
   float alpha = 1.0f;
   float beta = 0.0f;

   cusparseMatDescr_t descr;
   cusparseCreateMatDescr(&descr);
   cusparseSetMatType(descr,CUSPARSE_MATRIX_TYPE_GENERAL);
   cusparseSetMatIndexBase(descr,CUSPARSE_INDEX_BASE_ZERO);

   cusparseOperation_t op = CUSPARSE_OPERATION_NON_TRANSPOSE;
   cusparseScsrmv(handle, op, M->width, M->width, M->nnz, &alpha,
         descr, M->devVal, M->devRowPtr, M->devColInd, devVect, &beta, devVectNew); 
}

// This function is used to initialize the Matrix with dampening
__global__ void MXKernel(int n, float m, float *x) {
   int i = blockIdx.x*blockDim.x + threadIdx.x;
   if (i < n) {
      x[i] = m * x[i];
   }
}

// This function is used to normalize the prestige vectors
__global__ void XplusBKernel(int n, float *x, float b) {
   int i = blockIdx.x*blockDim.x + threadIdx.x;
   if (i < n) {
      x[i] = x[i] + b;
   }
}

// This function is used to initialize the Matrix with dampening
void sparse_MX(SparseMatrix *M, float m) {
   int numBlocks = (M->nnz + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(THREADS_PER_BLOCK, 1);

   MXKernel<<<gridDim, blockDim>>>(M->nnz, m, M->devVal);
}

// This function is used to normalize the prestige vectors
void sparse_XplusB(float *devVect, int width, float b) {
   int numBlocks = (width + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

   dim3 gridDim(numBlocks, 1);
   dim3 blockDim(THREADS_PER_BLOCK, 1);

   XplusBKernel<<<gridDim, blockDim>>>(width, devVect, b);
}

// This function takes a sparse matrix and does the page rank algorithm.
// It returns a sorted list of prestige
void pageRank(SparseMatrix *M, int *array) {
   int n = M->width;
   printf("Width: %d\n", n);
   float *vect = (float *) malloc(sizeof(float) * n);
   std::fill(vect, vect + n, 1.0f / n);
   float *difference = (float *) malloc(n * sizeof(float));
   float *devDifference = NULL;
   float *devVect = NULL;
   float *devVectNew = NULL;

   // Create handle to reuse data on GPU
   cusparseHandle_t handle;
   cusparseCreate(&handle);

   // Initialize value for normalizing prestige vectors 
   float b = (1.0f - DAMPING_FACTOR) / n;

   //Send all matrix data to card
   putMatOnDevice(M, handle);

   // Initialize Matrix with dampening
   sparse_MX(M, DAMPING_FACTOR);

   //Convert COO matrix to CSR format
   convertCOO2CSR(M, handle);


   cudaMalloc(&devDifference, n * sizeof(float));
   cudaMalloc(&devVect, n * sizeof(float));
   cudaMalloc(&devVectNew, n * sizeof(float));
   cudaMemcpy(devVect, vect, n * sizeof(float), cudaMemcpyHostToDevice);
   
   int iter = 0;
   float error;

   do {
      // Find new Prestige vector
      sparse_MatrixVectorMultiply(M, handle, devVect, devVectNew);

      // normalize prestige vector
      sparse_XplusB(devVectNew, n, b);

      // Find error
      error = vectorSubtractAndNormalize2(devVectNew, devVect, devDifference, difference, n);

      //Swap pointers for reuse
      float *temp = devVectNew;
      devVectNew = devVect;
      devVect = temp;
      printf("Iteration: %d... Error: %.7f\n", iter++, error);
      
   } while (error > QUADRATIC_ERROR);

   cudaMemcpy(vect, devVect, n * sizeof(float), cudaMemcpyDeviceToHost);

   cudaFree(devVectNew);
   cudaFree(devVect);
   cudaFree(M->devVal);
   cudaFree(M->devRowPtr);
   cudaFree(M->devColInd);
   cudaFree(devDifference);

   free(difference); 
   //free(newVect);

   for(int i = 0; i < M->width; i++) {
      array[i] = i;
   }

   // Sort prestige vector
   thrust::stable_sort_by_key(vect, vect + n, array, thrust::greater<float>());

   M->sortedPrestigeVector = vect;
}
