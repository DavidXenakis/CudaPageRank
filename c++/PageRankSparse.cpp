///*
//M: sparse adjacency matrix,
//get(M, i, j) = the link from 'j' to 'i', such that for all 'j':
//   sum(i, get(M, i, j)) = 1
//
//d: damping factor
//
//v_quadratic_error: quadratic error allowed
//
//returns a dense vector of ranks
//*/
//
//float *pagerank(M, d, v_quadratic_error) {
//   int n = M.numRows();
//   float *v =
//      /*float * of size n, filled with random numbers*/
//      malloc(sizeof(float) * n);
//   v =/* v ./ norm(v, 1) =
//         the vector from dividing elements of v / norm(v, 1),
//         where norm(v, 1) = sum(abs(v)) = sum of element magnitudes
//      */
//      v;
//
//   float *last_v =
//      /*
//        ones(N, 1) * inf =
//        length n vector of infinite values
//      */
//      malloc(sizeof(float) * n);
//
//
//   M_hat =
//      /*
//        (d .* M) + (((1 - d) / N) .* ones(N, N))
//                where d .* M = multiply each element of M by d
//        and ones(N, N) = an N by N array of 1s
//
//        M_hat = d * colNormalize(M) + k
//
//      */
//      malloc(sizeof(float *) * n);
//
//   int k = (1 - d) / n;
//   // add matrix d .* M to n by n matrix of all k's
//   // == add k to every element of d .* M
//
//   // d * M + k = (.85 * M) + .03
//
//
//   /*
//   while(norm(v - last_v, 2) > v_quadratic_error)
//   where norm(v - last_v, 2) = sum( (diff[i])^2 ) ^ (1/2)
//   where diff[i] = v[i] - last_v[i]
//   */
//   while(error > v_quadratic_error) {
//      /*last_v = v;
//      v = M_hat * v;
//         % removed the L2 norm of the iterated PR
//         end*/
//   }
//
//}
