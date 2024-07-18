#include <list>
#include <iostream> 
#include <string>
#ifndef READGT_H
#define READGT_H

struct Stop {
     std::string stop_id;
     std::string stop_name;
     std::string stop_lat;
     std::string stop_lon;
     ///double stop_lat;
     ////double stop_lon;


};

void readgtfs(std::list<Stop>& stops, std::string filename);
/////////void readgtfs(std::list<std::string>& myli);

#endif