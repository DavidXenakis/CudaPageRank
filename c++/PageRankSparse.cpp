/**
 * Implementation by David Xenakis and Max Bendick
 */
#include "SparseMatrix.h"
#include <mkl.h>
#include <mkl_types.h>
#include <omp.h>
#include <mkl_scalapack.h>
#include <stdio.h>
#include <math.h>

#define QUADRATIC_ERROR .001
#define DAMPING_FACTOR .85
#define THREADS_PER_BLOCK 1024

// This function finds the total error across iterations
__attribute__((target(mic))) float vectorSubtractAndSum(float *v, float *last_v, int n) {
   float sum = 0;

   #pragma omp parallel for reduction(+:sum)
   for(int i = 0; i < n; i++) {
      sum = sum + fabs(v[i] - last_v[i]);
   }
   
   return sum;
}

// This function is used to initialize the Matrix with dampening
__attribute__((target(mic))) void sparse_MX(float m, float *x, int nnz) {
   #pragma omp parallel for
   for(int i = 0; i < nnz; i++) {
      x[i] *= m;  
   }
}

// This function is used to normalize the prestige vectors
__attribute__((target(mic))) void sparse_XplusB(float *x, int width, float b) {
   #pragma omp parallel for 
   for(int i = 0; i < width; i++) {
      x[i] += b;
   }
}

void pageRank(SparseMatrix *M, int *array) {
   //Get values from M
   int n = M->width;
   float *val = M->cooValA;
   int *colInd = M->cooColIndA;
   int *rowInd = M->cooRowIndA;
   int nnz = M->nnz;

   // Create the prestige vectors for calculation
   float *vect = (float *) malloc(sizeof(float) * n);
   std::fill(vect, vect + n, 1.0f / n);
   float *newVect = (float *) malloc(sizeof(float) * n);
 
   // Initialize value for normalizing prestige vectors 
   float b = (1.0f - DAMPING_FACTOR) / n;

   // Initialize Matrix with dampening
   #pragma offload target(mic) inout(val: length(nnz)) in(nnz)
   sparse_MX(DAMPING_FACTOR, val, nnz);

   // Used for counting iterations and tallying error
   int iter = 0;
   float error;

   #pragma offload target(mic) in(newVect: length(n)) \
                               in(val, colInd, rowInd: length(nnz))\
                               inout(vect: length(n)) \
                               in(error, b, n)\
                               inout(iter)
   {
   do {
      // Find new Prestige vector
      mkl_cspblas_scoogemv("n", &n, val, rowInd, colInd, &nnz, vect, newVect);

      // normalize prestige vector
      sparse_XplusB(newVect, n, b);

      // Find error
      error = vectorSubtractAndSum(vect, newVect, n);

      //Swap pointers for reuse
      float *temp = newVect;
      newVect = vect;
      vect = temp;
      iter++;
   } while (error > QUADRATIC_ERROR);
   }

   printf("Iterations: %d\n", iter);

   #pragma omp parallel for 
   for(int i = 0; i < M->width; i++) {
      array[i] = i;
   }
   
   //int failure = -1;
   //slasrt2("D", &n, vect, array, &failure);

   M->sortedPrestigeVector = vect;
}
