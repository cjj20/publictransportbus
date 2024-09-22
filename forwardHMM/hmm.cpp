#include "hmm.h"
#include <iostream>
#include <vector>


std::vector<std::vector<double>>
    alpha(LENGTH,
          std::vector<double>(
              NN, 0.0)); // 2d vector initialized with elements //value being 0

void Forward(const HMM &phmm, const std::vector<int> &O, double &pprob) {
  int T = O.size(); ///////
  int i, j;         // state index
  int t;            // time index
  double sum;       // intermediate value when calculating local probability

  // 1. Initialization
  
  for (i = 0; i < phmm.N; ++i)
    alpha[0][i] = phmm.pi[i] * phmm.B[i][O[0]];
  

  std::cout << "Alpha[0]: ";
  for (int i = 0; i < phmm.N; ++i)
    std::cout << alpha[0][i] << " ";
  std::cout << std::endl;

  // 2. Induction
  
  for (t = 0; t < T - 1; ++t) {
    
    for (j = 0; j < phmm.N; ++j) {
      sum = 0.0;
      
      for (i = 0; i < phmm.N; ++i) {
        
        sum += alpha[t][i] * phmm.A[i][j]; 
        
      }
      alpha[t + 1][j] = sum * phmm.B[j][O[t + 1]];
      
    }
  }

  // 3. Termination
  pprob = 0.0;
  
  for (i = 0; i < phmm.N; ++i)
    pprob += alpha[T - 1][i];

  
}
