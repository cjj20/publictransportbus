#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <float.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cassert>

#ifndef HMM_H
#define HMM_H


const int NN = 3;
const int MM = 4;
const int LENGTH = 3;
//#define NN = 3
//#define MM = 4
//#define LENGTH = 3

struct HMM {
   int N; //hidden states
   int M; //observed
   //double **A;
   //double **B;
   //double *pi;
   std::vector<std::vector<double>> A;
   std::vector<std::vector<double>> B;
   std::vector<double> pi;
   HMM();//////
};

///extern HMM hmm; /////may be unnecessary here

//global alpha, can maybe put in hmm.cpp instead
extern std::vector<std::vector<double>> alpha;


//forward function declared 
////////void Forward(const HMM &phmm, int T, const std::vector<int> &O, double &pprob);
void Forward(const HMM &phmm, const std::vector<int> &O, double &pprob);



#endif