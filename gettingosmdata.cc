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

using namespace osmpbfreader;

std::string xtt;

// We need to define a visitor with three methods that will be called while the
// file is read
struct Counter {
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;

  Counter() : nodes(0), ways(0), relations(0) {}

  // This method is called every time a Node is read
  // void node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/,
  // const Tags &tags){
  void node_callback(uint64_t osmid, double lon, double lat, const Tags &tags) {

    // gettng input

    // std::string xt = "4502"; //works but not global variable

    // std::cout << "osmid" << osmid << "longitude"<< lon << "latitude" << lat
    // <<std::endl; //this line works std::cout << "tags" << &tags <<std::endl;

    auto highw = tags.find("highway");
    // auto nam = tags.find("name");
    auto refff =
        tags.find("ref"); // ref instead pf gtfs:stop_id used because no
                          // gtfs:stop_id tag for isle of man osm data
    // auto longg = tags.find("lon");
    // auto latt = tags.find("lat");
    // auto gtfsstopidd = tags.find("gtfs:stop_id"); //this is for osm data
    // which contains that tag if(highw != tags.end() &&
    // highw->second=="bus_stop" && nam != tags.end() &&nam->second=="Bungalow
    // Footbridge") { //works name value taken from           //a node if(highw
    // != tags.end() && highw->second=="bus_stop" && refff != tags.end() &&
    // refff->second=="17401") { //works value taken from a node
    if (highw != tags.end() && highw->second == "bus_stop" &&
        refff != tags.end() &&
        refff->second == xtt) { // xtt is any value user gives, works
      // if(highw != tags.end() && highw->second=="bus_stop") {

      ++nodes;
      /// ways.push_back(refs);

      std::cout << "osmid" << osmid << "tags" << &tags << "lon" << lon << "lat"
                << lat << std::endl; // this line works
    }
  }

  // This method is called every time a Way is read
  // refs is a vector that contains the reference to the nodes that compose the
  // way
  void way_callback(uint64_t /*osmid*/, const Tags & /*tags*/,
                    const std::vector<uint64_t> & /*refs*/) {
    // void way_callback(uint64_t osmid, const Tags &tags, const
    // std::vector<uint64_t> &/*refs*/){

    ++ways;

    /// std::cout << "osmid" << osmid << std::endl; //this line works

    /// std::cout << "tags" << &tags << std::endl; //tags end up same term and
    /// seems to be in hex, it may be working
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
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf" << std::endl;
    return 1;
  }

  std::cout << "Enter a number: ";
  getline(std::cin, xtt);

  Counter counter;
  read_osm_pbf(argv[1], counter);
  // std::cout << "We read " << nodes << " counter.nodes " << counter.ways << "
  // ways and " << counter.relations << " relations" << std::endl;
  std::cout << "number of nodes that fit" << counter.nodes
            << std::endl;                                // this line works
  std::cout << "ways info" << counter.ways << std::endl; // this line works

  // print the value we got from the user
  // std::cout << x << std::endl;

  return 0;
}
