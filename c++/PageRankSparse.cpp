/*
M: sparse adjacency matrix, 
get(M, i, j) = the link from 'j' to 'i', such that for all 'j':
   sum(i, get(M, i, j)) = 1

d: damping factor

v_quadratic_error: quadratic error allowed

returns a dense vector of ranks
*/
#define QUADRATIC_ERROR .001
#define DAMPING_FACTOR .85

float *pagerank(SparseMatrix M) {
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
