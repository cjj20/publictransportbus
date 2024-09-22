#include "hmm.h"
#include <iomanip>
#include <iostream>





std::vector<std::vector<double>> A(NN, std::vector<double>(NN, 0.0));


std::vector<std::vector<double>> B(NN, std::vector<double>(MM, 0.0));

std::vector<double> pi(NN, 0.0);



HMM::HMM()
    ///: N(3),
    /// M(4),
    : A{{0.500, 0.375, 0.125}, {0.250, 0.125, 0.625}, {0.250, 0.375, 0.375}},
      B{{0.60, 0.20, 0.15, 0.05},
        {0.25, 0.25, 0.25, 0.25},
        {0.05, 0.10, 0.35, 0.50}},
      pi{0.63, 0.17, 0.20} {
  N = A.size();
  M = B[0].size();
}

// Define a global instance of HMM
HMM hmm;



// Observation sequence
std::vector<int> Seq = {0, 2, 3};

int main(int argc, char *argv[]) {

  // Probability of the observation sequence
  
  double pprob;

  // Call the Forward algorithm
  
  Forward(hmm, Seq, pprob);

  // Print the result
  std::cout << "Probability of the observation sequence: " << pprob
            << std::endl;
  std::cout << "A stuff" << hmm.A[0][0] << std::endl;
  std::cout << std::setprecision(7) << pprob << std::endl;

  return 0;
}
