/*


To build this file :


sudo g++ -O2 -o getda gettingosmdatacopy.cc listgtfs22.cpp -lprotobuf-lite -losmpbf -lz

To run it:
./getdata isle-of-man-latest.osm.pbf
*/

#include "libosmpbfreader/osmpbfreader.h"
#include "readgt.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <typeinfo>
#include <vector>

using namespace osmpbfreader;

std::list<double> oslp;
std::list<double> oslpp;


struct osmcoord {
  double lati;
  double loni;
};

struct Counter {
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;

  std::vector<osmcoord> vlop;

  Counter() : nodes(0), ways(0), relations(0) {}

  
  void node_callback(uint64_t osmid, double lon, double lat, const Tags &tags) {

    auto highw = tags.find("highway");
    
    if (highw != tags.end() && highw->second == "bus_stop") {

      ++nodes;
      
      oslp.push_back(lat);
      oslpp.push_back(lon);
      vlop.push_back({lat, lon});

      std::cout << "osmid" << osmid << "tags" << &tags << "lon" << lon << "lat"
                << lat << std::endl; 
    }
  }

  
  void way_callback(uint64_t /*osmid*/, const Tags & /*tags*/,
                    const std::vector<uint64_t> & /*refs*/) {
    

    ++ways;

    
  }

  // This method is called every time a Relation is read
  // refs is a vector of pair corresponding of the relation type (Node, Way,
  // Relation) and the reference to the object
  void relation_callback(uint64_t /*osmid*/, const Tags & /*tags*/,
                         const References & /*refs*/) {
    ++relations;
  }
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt"
              << std::endl;
    return 1;
  }

  std::list<Stop> stops;
  readgtfs(stops, argv[2]);

  

  Counter counter;
  read_osm_pbf(argv[1], counter);
  
  std::cout << "number of nodes that fit" << counter.nodes
            << std::endl;                                
  std::cout << "ways info" << counter.ways << std::endl; 
  

  for (const auto &coordinn : oslpp) {
    std::cout << coordinn << std::endl;
  }

  std::vector<std::string> coord_prints;
  std::vector<std::string> stop_prints;

  int numgtfsstops = 0;
  int numosmstops = 0;

  for (const auto &coord : counter.vlop) {
    std::cout << "Lat: " << coord.lati << ", Lon: " << coord.loni << std::endl;
    numosmstops++;
    
    std::string coord_print = std::to_string(coord.lati);
    std::cout << coord_print << std::endl;
    coord_prints.push_back(coord_print);
  }

  for (const auto &stop : stops) {
    std::cout << "stop id" << stop.stop_id << "stop lat" << stop.stop_lat
              << "stop lon" << stop.stop_lon << std::endl;
    numgtfsstops++;

    
    std::string stop_print = stop.stop_lat;
    std::cout << stop_print << std::endl;
    stop_prints.push_back(stop_print);
  }

  

  int possiblematches = 0;
  int possiblenonmatches = numosmstops + numgtfsstops;

 

  for (const auto &coord : counter.vlop) {
    for (const auto &stop : stops) {

      std::string latiStr = std::to_string(coord.lati);
      std::string loniStr = std::to_string(coord.loni);

      if ((latiStr.find(stop.stop_lat) != std::string::npos) ||
          (loniStr.find(stop.stop_lon) != std::string::npos)) {

        
        possiblematches++;
        
        std::cout << "possible matches osm - gtfs matches" << std::endl;
        std::cout << "osm lat:" << coord.lati
                  << "  is matching/starts with gtfs lat val" << stop.stop_lat
                  << "osm lon is: " << coord.loni << std::endl;
        std::cout << "gtfs details: gtfs stop lon is:" << stop.stop_lon
                  << "  gtfs stop lat  is: " << stop.stop_lat << std::endl;
      } else if ((stop.stop_lat.find(latiStr) != std::string::npos) ||
                 (stop.stop_lon.find(loniStr) != std::string::npos)) {
        

        std::cout << "possible matches osm - gtfs matches" << std::endl;
        std::cout << "gtfs lat:" << stop.stop_lat
                  << "  is matching/starts with osm lat val" << coord.lati
                  << "osm lon is: " << coord.loni << std::endl;
        std::cout << "gtfs details: gtfs stop lon is:" << stop.stop_lon
                  << "  gtfs stop lat  is: " << stop.stop_lat << std::endl;
      }

      
    }
  }

  possiblenonmatches = possiblenonmatches - possiblematches;

  std::cout << "Number of possible matches found based on latitude: "
            << possiblematches << std::endl;
  std::cout << "Number of possible non-matches found based on latitude: "
            << possiblenonmatches << std::endl;

  std::cout << "Number of gtfs stops available:  " << numgtfsstops << std::endl;
  std::cout << "Number of osm stops that are bus stops: " << numosmstops
            << std::endl;

  

  return 0;
}
