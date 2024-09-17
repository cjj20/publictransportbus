#include <list>
#include <iostream> 
#include <string>
#ifndef READGTCOPY_H
#define READGTCOPY_H

struct Stop {
     std::string stop_id;
     std::string stop_name;
     std::string stop_lat = "0.0";
     std::string stop_lon = "0.0";
     ///double stop_lat;
     ////double stop_lon;
	 
	 Stop() = default;


};

void readgtfs(std::list<Stop>& stops, std::string filename);
/////////void readgtfs(std::list<std::string>& myli);

#endif