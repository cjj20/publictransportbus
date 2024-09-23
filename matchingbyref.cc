/*
Copyright (c) 2001-2022, Hove
This is an example file, do whatever you want with it! (for example if you are
in Paris, invite us for a beer)

This shows the simplest way to use the osm.pbf reader. It just counts the number
of objects in the file.

To build this file :
g++ -O2 -o counter gettingosmdata.cc -losmpbf -lprotobuf

To run it:
./getdata isle-of-man-latest.osm.pbf
*/

#include "libosmpbfreader/osmpbfreader.h"
#include <iostream>
#include <string>
#include "readgt.h"
#include <fstream>
#include <iomanip>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

using namespace osmpbfreader;





struct Counter {
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;
  const std::list<Stop> &stops;

  Counter(const std::list<Stop> &stops_ref) : nodes(0), ways(0), relations(0), stops(stops_ref) {}

  // This method is called every time a Node is read
  // void node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/,
  // const Tags &tags){
  void node_callback(uint64_t osmid, double lon, double lat, const Tags &tags) {

    auto highw = tags.find("highway");
    
    auto refff = tags.find("ref"); 
       
    if (highw != tags.end() && highw->second == "bus_stop" &&
        refff != tags.end()) { 
		
		for (const auto &stop : stops) {
			if(refff->second == stop.stop_id) {
				
				++nodes;
				std::cout << "Matched node! OSM ID: " << osmid 
                              << ", osm lon: " << lon 
                              << ", osm lat: " << lat 
							  << " gtfs id " << stop.stop_id 
							  << " osm ref " <<  refff->second  
							  << " gtfs lat" << stop.stop_lat
							  << "gtfs lon " << stop.stop_lon << std::endl;
				
					 
			} /*else { 
				
				std::cout << "non-matching nodes" << refff->second << "ref,  osm lon " << lat << " osm lon " << lon << std::endl;
				std::cout << "non-matching gtfs" << stop.stop_id << "gtfs stop id,  gtfs stop lat" << stop.stop_lat << " gtfs stop lon " << stop.stop_lon << std::endl;
				
			} */
			
		}
		
		 
		
  // This method is called every time a Way is read
  // refs is a vector that contains the reference to the nodes that compose the
  // way
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
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf file_to_read.gtfs" << std::endl;
    return 1;
  }
  
  std::list<Stop> stops;
  readgtfs(stops, argv[2]);

  

  Counter counter(stops);
  read_osm_pbf(argv[1], counter);
  
  std::cout << "number of nodes that fit" << counter.nodes
            << std::endl;                                
  std::cout << "ways info" << counter.ways << std::endl; 

  
  std::cout << "Number of matched nodes based on ref: " << counter.nodes << std::endl;
  
  
  

  return 0;
}
