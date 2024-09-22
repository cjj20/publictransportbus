#include <cassert>
#include <float.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#ifndef HMM_H
#define HMM_H

const int NN = 3;
const int MM = 4;
const int LENGTH = 3;


struct HMM {
  int N; // hidden states
  int M; // observed
  
  std::vector<std::vector<double>> A;
  std::vector<std::vector<double>> B;
  std::vector<double> pi;
  HMM(); 
};



// global alpha, can maybe put in hmm.cpp instead
extern std::vector<std::vector<double>> alpha;

// forward function declared

void Forward(const HMM &phmm, const std::vector<int> &O, double &pprob);

#endif