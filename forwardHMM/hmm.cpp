#include <iostream>
#include <vector>
#include "hmm.h"

// Define the global alpha array
////std::vector<std::vector<double>> alpha(LENGTH + 1, std::vector<double>(NN + 1, 0.0));  // +1 not needed here 
std::vector<std::vector<double>> alpha(LENGTH, std::vector<double>(NN, 0.0)); //2d vector initialized with elements //value being 0
//std::vector<std::vector<double>> alpha(LENGTH, std::vector<double>(NN)); //may need to change
//////void Forward(const HMM &phmm, int T, const std::vector<int> &O, double &pprob) {
void Forward(const HMM &phmm, const std::vector<int> &O, double &pprob) {
    int T = O.size(); ///////
    int i, j; // state index
    int t; // time index
    double sum; // intermediate value when calculating local probability

    // 1. Initialization
    /////for (i = 1; i <= phmm.N; ++i)
      //////  alpha[1][i] = phmm.pi[i] * phmm.B[i][O[1]]; 
    for (i = 0; i <= phmm.N; ++i)
        alpha[0][i] = phmm.pi[i] * phmm.B[i][O[0]]; 
    /// replace above with this//     alpha(1, i) = phmm.pi(i) * phmm.B(i, O(1));
     ///  alpha(1, i) = phmm.pi(i) * phmm.B(i,O(1));

        std::cout << "Alpha[0]: ";
   for (int i = 0; i < phmm.N; ++i)
        std::cout << alpha[0][i] << " ";
   std::cout << std::endl;

    // 2. Induction
  ///////  for (t = 1; t < T; ++t) {  //should this be T - 1 ??
    for (t = 0; t < T - 1; ++t) {      
       ////// for (j = 1; j <= phmm.N; ++j) {
        for (j = 0; j <= phmm.N; ++j) {
            sum = 0.0;
           //////// for (i = 1; i <= phmm.N; ++i)
            for (i = 0; i <= phmm.N; ++i) {
              ///////  sum += alpha[t][i] * phmm.A[i][j];
                sum += alpha[t][i] * phmm.A[i][j];   // should t - 1 be here??
                // replace above with this sum += alpha(t, i) * phmm.A(i, j);
               /// sum += alpha(t, i) * phmm.A(i, j);
             }
            alpha[t + 1][j] = sum * phmm.B[j][O[t + 1]];
            //replace the above with this alpha(t + 1, j) = sum * phmm.B(j,O(t + 1));
            /// alpha(t + 1, j) = sum * phmm.B(j,O(t + 1));
        }
    }

    // 3. Termination
    pprob = 0.0;
   ///// for (i = 1; i <= phmm.N; ++i)
    /////     pprob += alpha[T][i];
    for (i = 0; i <= phmm.N; ++i) 
         pprob += alpha[T - 1][i];
     
     
        //replace the above with this pprob += alpha(T, i);
        ///pprob += alpha(T, i);
}
