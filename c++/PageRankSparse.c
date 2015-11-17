#include "SparseMatrix.h"
#include <mkl_types.h>
#include <mkl.h>
#include <omp.h>

#define QUADRATIC_ERROR .01
#define DAMPING_FACTOR .95
#define THREADS_PER_BLOCK 1024

float vectorSubtractAndNormalize(float *v, float *last_v, int n) {
   float sum = 0;

   #pragma omp parallel for reduction(+:sum)
   for(int i = 0; i < n; i++) {
      sum = sum + (v[i] - last_v[i]);
   }
   
   return sum;
}


void sparse_MatrixVectorMultiply(SparseMatrix *M, float * vect, float * vectNew) {
      void mkl_scoosv ( const char *transa ,    // choose 'n'
                        const MKL_INT *m ,      // Num Rows
                        const float *alpha ,    // scalar 
                        const char *matdescra , // choose ['G', 'x', 'x', 'C']
                        const float *val ,      // Array of length nnz
                        const MKL_INT *rowind , // row index
                        const MKL_INT *colind , // column index
                        const MKL_INT *nnz ,    // length of array
                        const float *x ,        // input vector
                        float *y );             // output vector
      MKL_INT m = SparseMatrix->width;
      float alpha = 1.0f;


      mkl_scoosv("n", M->width, &m, "GxxC", M->cooValA, M->cooRowIndA, M->cooColIndA, M->nnz, vect, vectNew);
}

void sparse_MX(SparseMatrix *m, float m) {
   #pragma omp parallel for
   for(int i = 0; i < m->nnz; i++) {
      m->cooValA[i] *= m;  
   }
}

void sparse_XplusB(float *vect, int width, float b) {
   #pragma omp parallel for
   for(int i = 0; i < width; i++) {
      vect[i] += b;
   }
}

void pageRank(SparseMatrix *M, int *array) {
   int n = M->width;
   float *vect = (float *) malloc(sizeof(float) * n);
   std::fill(vect, vect + n, 1.0f / n);
   float *newVect = (float *) malloc(sizeof(float) * n);
   float *difference = (float *) malloc(n * sizeof(float));
 
   float b = (1.0f - DAMPING_FACTOR) / n;

   int iter = 0;
   float error;

   do {
      sparseMatrixVectorMultiply(M, vect, newVect);
      sparse_XplusB(newVect, M->width, b);
      error = vectorSubtractAndNormalize(vect, newVect, M->width);

      float *temp = newVect;
      newVect = vect;
      vect = newVect;


   } while (error > QUADRATIC_ERROR);

   for(int i = 0; i < M->width; i++) {
      array[i] = i;
   }

   m->sortedPrestigeVector = vect;
}
