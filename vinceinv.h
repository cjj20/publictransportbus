#include <cmath>
#include <iostream>
#include <list>
#include <string>
#ifndef VINCEINV_H
#define VINCEINV_H

double convertToRadians(double deg);
double vincentydistt(std::string lon1, std::string lat1, double lon2,
                     double lat2);

#endif