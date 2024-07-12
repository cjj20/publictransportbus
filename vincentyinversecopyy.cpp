//vincenty inverse c++ implementation
//Compile:
 //*     $ g++ vincentyinversedist.cpp -o vincentydistance
// * Run:
// *     $ ./vincentydistance
//lat1

//lon1

#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <sstream>
#include <stdexcept>
#include "vinceinv.h"


double lonn1 = 0;
double latt1 = 0;


double convertToRadians(double deg) {
  return deg * M_PI / 180;

}

double s;
double alpha1; 
double alpha2; 


  double stringToDouble(const std::string& str) {
    double result;
    std::istringstream iss(str);
    
     iss >> result;
   // if (!(iss >> result)) {
   //     throw std::invalid_argument("Invalid input: " + str);
   // }

    return result;
  }


//double vincentydistt(double lon1, double lat1, double lon2, double lat2) {
double vincentydistt(std::string lon1, std::string lat1, double lon2, double lat2) {
double a = 6378137.0; //meters semi-major axis of the earth
double f = 1.0 / 298.257223563; // should it be 1.0 / 298.257222101?, flattening
double b =  (1 - f) * a; //meters, semi-minor axis of the earth
//double lonn1 = std::stod(lon1);
//double latt1 = std::stod(lat1);

//lonn1 = std::stod(lon1);
//latt1 = std::stod(lat1);


//lonn1 = std::stod(lon1.c_str());
//latt1 = std::stod(lat1.c_str());


lonn1 = stringToDouble(lon1);
latt1 = stringToDouble(lat1);




////stod::stod(lon2);
//lat1 = lat1 * M_PI / 180;
//lon1 = lon1 * M_PI / 180;
//lat2 = lat2 * M_PI / 180;
//lon2 = lon2 * M_PI / 180;

//double capL = ((lon2 - lon1) * pi/180);
////double capL = convertToRadians(lon2 - lon1);  //?
double capL = convertToRadians(lon2 - lonn1);


//pi = 
//M_PI may be pi from #include <cmath>
//double u1 = std::atan((1.0 - f) * std::tan(convertToRadians(lat1))); // atan((1 - f) * tan(lat1 * M_PI/180));  // double u1 = (1-f) * tan(lat1 * pi/180);

double u1 = std::atan((1.0 - f) * std::tan(convertToRadians(latt1))); // atan((1 - f) * tan(lat1 * M_PI/180));  // double u1 = (1-f) * tan(lat1 * pi/180);


double u2 = std::atan((1.0 - f) * std::tan(convertToRadians(lat2))); // atan((1 - f) * tan(lat2 * M_PI/180));
//double u1 = atan((1 - f) * tan(lat1)); // atan((1 - f) * tan(lat1 * M_PI/180));  // double u1 = (1-f) * tan(lat1 * pi/180);
//double u2 = atan((1 - f) * tan(lat2)); // atan((1 - f) * tan(lat2 * M_PI/180));
double sinu1 = std::sin(u1); //sinu1 = tanu1 * cosu1;
double sinu2 = std::sin(u2); //sinu2 = tanu2 * cosu2;
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
//int k;

do {
 //prevLambda = lambda;
 sinLambda = std::sin(lambda);
 cosLambda = std::cos(lambda);
 sinSigma = std::sqrt(pow(cosu2 * sinLambda, 2)  + pow(cosu1 * sinu2 - sinu1 * cosu2 * cosLambda, 2));
 if(sinSigma == 0) {
   return 0; //coincident points
}
 cosSigma = sinu1 * sinu2 + cosu1 * cosu2 * cosLambda;
 sigma = std::atan2(sinSigma, cosSigma);   //std::atan(sinSigma / cosSigma); //std::atan2(sinSigma, cosSigma)?   perhaps use atan2 instead here
 sinAlpha = cosu1 * cosu2 * sinLambda / sinSigma; ////? cosu1 * cosu2 * sinLambda / sinSigma; 
 cosSqAlpha = 1 - sinAlpha * sinAlpha;  //1 - pow(sinAlpha, 2); //
 //cos2sig = cosSigma - 2 * sinu1 * sinu2 / cosSqAlpha; //??
// if(isnan(cos2sig)) {
// cos2sig = 0; //equatorial line, this here may not be necessary  //possibly replace this with an if else 
//}

if(cosSqAlpha == 0) {
 cos2sig = 0;
} else {
 cos2sig = cosSigma - 2 * sinu1 * sinu2 / cosSqAlpha;

}
 capC = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));//f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha))
 prevLambda = lambda;
 lambda = capL + (1 - capC) * f * sinAlpha * (sigma + capC * sinSigma * (cos2sig + capC * cosSigma * (-1 + 2 * cos2sig * cos2sig)));  

} while ( std::abs(lambda - prevLambda) > 1e-12 && --iterationLimit > 0); ///? std::abs here or fabs

if(iterationLimit == 0) {
return 0;  //failed to converge
}

double uSq = cosSqAlpha * ( a * a - b * b) / (b * b);  //cosSqAlpha * ( pow(a, 2) - pow(b, 2)) / pow(b, 2); 
double capA = 1 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq))); 
double capB = uSq / 1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq))); 
double deltaSigma = capB * sinSigma * (cos2sig + capB / 4.0 * (cosSigma * ( -1.0 + 2.0 * pow(cos2sig, 2)) - capB / 6 * cos2sig * (-3.0 + 4.0 * pow(sinSigma, 2)) * (-3.0 + 4.0 * pow(cos2sig, 2)))); 
s =  b * capA * (sigma - deltaSigma); //result distance
alpha1 = std::atan2(cosu2 * sinLambda, cosu1 * sinu2 - sinu1 * cosu2 * cosLambda) * 180 / M_PI;  //initial bearing, direction, forward azimuth?
alpha2 = std::atan2(cosu1 * sinLambda, -sinu1 * cosu2 + cosu1 * sinu2 * cosLambda) * 180 / M_PI; //final bearing, rev azimuth? reverse azimuth?
//for (k = 0; k < 100; k++) {
 ///}

//result[0] = 
//result[1] = 


////try{ //?


if (s < 30) {
   std::cout << "forward azimuth 2 for less than 30m: " << alpha2 << std::endl;
std::cout << "forward azimuth 1 for less than 30m: " << alpha1 << std::endl;
std::cout << "distance less than 30m: " <<  s << std::endl;
} 


//std::cout << "forward azimuth 2: " << alpha2 << std::endl;
//std::cout << "forward azimuth 1: " << alpha1 << std::endl;
//std::cout << "distance" <<  s << std::endl;






return s;





}

//int main() {
   //double lon1 = 120.335066;//10.7486;
   //double lat1 = 23.205402;//-23.2089;
  // std::string lon1 = "120.335066";//10.7486;
 //  std::string lat1 = "23.205402";//-23.2089;
 //  double lon2 = 120.339733;//-14.97214;
 //  double lat2 = 23.202188;//-24.97532;
   //vincentydistt(lon1, lat1, lon2, lat2);
   
 //   vincentydistt(lon1, lat1, lon2, lat2);

 /////  vincentydistt(120.335066, 23.205402, 120.339733, 23.202188); ///?
//   std::cout << "distance" <<  s << std::endl;
  // std::cout << "forward azimuth" << alpha1 <<std::endl;
  // std::cout << "rev azimuth" << alpha2 << std::endl;
 //  return 0;


//}