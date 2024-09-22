#include <iostream>
#include <list>
#include <string>
#ifndef READGTCOPY_H
#define READGTCOPY_H

struct Stop {
  std::string stop_id;
  std::string stop_name;
  std::string stop_lat = "0.0";
  std::string stop_lon = "0.0";
  /// double stop_lat;
  ////double stop_lon;

  Stop(const std::string &id, const std::string &name,
       const std::string &lat = "0.0", const std::string &lon = "0.0")
      : stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}

  Stop() = default;
};

void readgtfs(std::list<Stop> &stops, std::string filename);
/////////void readgtfs(std::list<std::string>& myli);

#endif