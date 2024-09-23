// vincenty inverse c++ implementation
// Compile:
//*     $ g++ vincentyinversedist.cpp -o vincentydistance
// * Run:
// *     $ ./vincentydistance
// lat1

// lon1

#include "vinceinv.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdexcept>
#include <string>

double lonn1 = 0;
double latt1 = 0;

double convertToRadians(double deg) { return deg * M_PI / 180; }

double s;
double alpha1;
double alpha2;

double stringToDouble(const std::string &str) {
  double result;
  std::istringstream iss(str);

  iss >> result;
  

  return result;
}


double vincentydistt(std::string lon1, std::string lat1, double lon2,
                     double lat2) {
  double a = 6378137.0; // meters semi-major axis of the earth
  double f =
      1.0 / 298.257223563; // flattening
  double b = (1 - f) * a;  
  

  lonn1 = stringToDouble(lon1);
  latt1 = stringToDouble(lat1);

  
  double capL = convertToRadians(lon2 - lonn1);

  
  double u1 = std::atan(
      (1.0 - f) * std::tan(convertToRadians(
                      latt1))); 
                                

  double u2 = std::atan((1.0 - f) *
                        std::tan(convertToRadians(
                            lat2))); 
  
  double sinu1 = std::sin(u1); 
  double sinu2 = std::sin(u2); 
  double cosu1 = std::cos(u1);
  double cosu2 = std::cos(u2);

  int iterationLimit = 100;
  double sinSigma;
  double sinLambda;
  double cosSigma;
  double cosLambda;
  double sigma;
  double sinAlpha;
  double cosSqAlpha;
  double cos2sig;
  double capC;
  double lambda = capL;
  double prevLambda;
  

  do {
    
    sinLambda = std::sin(lambda);
    cosLambda = std::cos(lambda);
    sinSigma = std::sqrt(pow(cosu2 * sinLambda, 2) +
                         pow(cosu1 * sinu2 - sinu1 * cosu2 * cosLambda, 2));
    if (sinSigma == 0) {
      return 0; // coincident points
    }
    cosSigma = sinu1 * sinu2 + cosu1 * cosu2 * cosLambda;
    sigma = std::atan2(
        sinSigma,
        cosSigma); 
                   
    sinAlpha = cosu1 * cosu2 * sinLambda /
               sinSigma; 
    cosSqAlpha = 1 - sinAlpha * sinAlpha; 
    

    if (cosSqAlpha == 0) {
      cos2sig = 0;
    } else {
      cos2sig = cosSigma - 2 * sinu1 * sinu2 / cosSqAlpha;
    }
    capC = f / 16 * cosSqAlpha *
           (4 + f * (4 - 3 * cosSqAlpha)); 
                                           
    prevLambda = lambda;
    lambda =
        capL + (1 - capC) * f * sinAlpha *
                   (sigma + capC * sinSigma *
                                (cos2sig + capC * cosSigma *
                                               (-1 + 2 * cos2sig * cos2sig)));

  } while (std::abs(lambda - prevLambda) > 1e-12 &&
           --iterationLimit > 0); 

  if (iterationLimit == 0) {
    return 0; // failed to converge
  }

  double uSq = cosSqAlpha * (a * a - b * b) /
               (b * b); 
  double capA = 1 + uSq / 16384.0 *
                        (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
  double capB =
      uSq / 1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq)));
  double deltaSigma =
      capB * sinSigma *
      (cos2sig + capB / 4.0 *
                     (cosSigma * (-1.0 + 2.0 * pow(cos2sig, 2)) -
                      capB / 6 * cos2sig * (-3.0 + 4.0 * pow(sinSigma, 2)) *
                          (-3.0 + 4.0 * pow(cos2sig, 2))));
  s = b * capA * (sigma - deltaSigma); // result distance
  alpha1 =
      std::atan2(cosu2 * sinLambda, cosu1 * sinu2 - sinu1 * cosu2 * cosLambda) *
      180 / M_PI; 
  alpha2 = std::atan2(cosu1 * sinLambda,
                      -sinu1 * cosu2 + cosu1 * sinu2 * cosLambda) *
           180 / M_PI; 
  
/*
  if (s < 30) {
    std::cout << "forward azimuth 2 for less than 30m: " << alpha2 << std::endl;
    std::cout << "forward azimuth 1 for less than 30m: " << alpha1 << std::endl;
    std::cout << "distance less than 30m: " << s << std::endl;
  }
*/
  

  return s;
}

