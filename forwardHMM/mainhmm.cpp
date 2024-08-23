#include <iostream>
#include <iomanip>
#include "hmm.h"

// Define constants
//const int NN = 3;  // Number of hidden states
//const int MM = 4;  // Number of observed symbols
//const int LENGTH = 3;  // Length of the observation sequence

// Static arrays for HMM parameters

std::vector<std::vector<double>> A(NN, std::vector<double>(NN, 0.0));
///std::vector<std::vector<double>> A(NN, std::vector<double>(NN)) = {
//std::vector<std::vector<double>> A = {
//A = {

   // {0.500, 0.375, 0.125},
   // {0.250, 0.125, 0.625},
   // {0.250, 0.375, 0.375}
//};

std::vector<std::vector<double>> B(NN, std::vector<double>(MM, 0.0));
///std::vector<std::vector<double>> B(NN, std::vector<double>(MM)) = {
//std::vector<std::vector<double>> B = {
//B = {

   // {0.60, 0.20, 0.15, 0.05},
   // {0.25, 0.25, 0.25, 0.25},
   // {0.05, 0.10, 0.35, 0.50}
//};


// Declare the vector
//std::vector<std::vector<double>> B(NN, std::vector<double>(MM));

// Assign values to the vector
//B[0] = {0.60, 0.20, 0.15, 0.05};
//B[1] = {0.25, 0.25, 0.25, 0.25};
//B[2] = {0.05, 0.10, 0.35, 0.50};

///std::vector<double> pi(NN) = {0.63, 0.17, 0.20};
std::vector<double> pi(NN, 0.0);
//pi = {0.63, 0.17, 0.20};
///std::vector<double> pi = {0.63, 0.17, 0.20};
////double pi[NN] = {0.63, 0.17, 0.20};

// Initialize the HMM struct
/* HMM hmm1 = {
    NN,
    MM,
   std::vector<std::vector<double>>(A, A + NN),
    std::vector<std::vector<double>>(B, B + NN),
    std::vector<double>(pi, pi + NN)
}; */


//maybe replace the above with HMM hmm1 = {NN, MM, std::vector<std::vector<double>> A, //std::vector<std::vector<double>> //B, std::vector<double> pi };

//HMM hmm1 = {NN, MM, std::vector<std::vector<double>> A, std::vector<std::vector<double>> B, std::vector<double> pi };

//std::vector<std::vector<double>> A = {
//hmm.A = {
///double A[NN][NN] = {
    //{0.500, 0.375, 0.125},
   // {0.250, 0.125, 0.625},
  //  {0.250, 0.375, 0.375}
   // };


//std::vector<std::vector<double>> B = {
   // hmm.B = {
///double B[NN][MM] = {
   // {0.60, 0.20, 0.15, 0.05},
   // {0.25, 0.25, 0.25, 0.25},
   // {0.05, 0.10, 0.35, 0.50}
   // };

//std::vector<double> pi = {0.63, 0.17, 0.20};

 //hmm.pi = {0.63, 0.17, 0.20};


HMM::HMM() 
    ///: N(3),
     /// M(4),
    : A{
        {0.500, 0.375, 0.125},
        {0.250, 0.125, 0.625},
        {0.250, 0.375, 0.375}
      },
      B{
        {0.60, 0.20, 0.15, 0.05},
        {0.25, 0.25, 0.25, 0.25},
        {0.05, 0.10, 0.35, 0.50}
      },
      pi{0.63, 0.17, 0.20}
{
      N = A.size();
      M = B[0].size();
}

// Define a global instance of HMM
HMM hmm;










//HMM hmm1 = {NN, MM,  A, B,  pi };
///HMM hmm1 = (NN, MM,  A, B,  pi );

// Observation sequence
std::vector<int> Seq = {0, 2, 3};

int main(int argc, char *argv[]) {
    




       



    
    // Probability of the observation sequence
    ////double pprob = 0.0;
       double pprob;


    // Call the Forward algorithm
    ///////Forward(hmm1, LENGTH, Seq, pprob);
    Forward(hmm, Seq, pprob);

    // Print the result
    std::cout << "Probability of the observation sequence: " << pprob << std::endl;
    std::cout << "A stuff" << hmm.A[0][0] << std::endl;
    std::cout << std::setprecision(7) << pprob << std::endl;

    return 0;
}
