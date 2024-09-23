
/*


To build this file :
g++ -O2 -o counter matchingnynam.cc listgtfs22.cpp  vincentyinversecopy.cpp -losmpbf -lprotobuf -lz

To run it:
./counter path_to_your_data.osm.pbf stops.txt
*/

#include "libosmpbfreader/osmpbfreader.h"
#include "readgt.h"
#include "vinceinv.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

using namespace osmpbfreader;



struct osmcoordntag {
  std::map<std::string, std::string> Tagg; 
  
  double lati;
  double loni;

  osmcoordntag(double lati, double loni,
               const std::map<std::string, std::string> &tags)
      : lati(lati), loni(loni), Tagg(tags) {}
};


std::list<Stop> stops;


// We need to define a visitor with three methods that will be called while the
// file is read
struct nameMatch {
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;

  std::vector<Tags> tagss;
  std::vector<osmcoordntag> vlopsecondd;

  

  nameMatch() : nodes(0), ways(0), relations(0) {}

  // This method is called every time a Node is read
  void node_callback(uint64_t /*osmid*/, double lon, double lat,
                     const Tags &tags) {
    
    auto nam = tags.find("name");
    auto highw = tags.find("highway");
    
    if (highw != tags.end() && highw->second == "bus_stop") {
      ++nodes;
      tagss.push_back(tags);
      vlopsecondd.push_back(
          {lat, lon, tags}); 
      
    }
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

  const std::vector<Tags> &getTagsList() const { return tagss; }
};

struct Counter {
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;

  Counter() : nodes(0), ways(0), relations(0) {}

  // This method is called every time a Node is read
  void node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/,
                     const Tags & /*tags*/) {
    ++nodes;
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

////nameMatch namematching;

template <typename nameMatch>
void readd_osm_pbf(const std::string &filename, nameMatch &nameMatchin) {
  Parser<nameMatch> p(filename, nameMatchin);
  p.parse();
}

template <typename Visitor>
void rd_osm_pbf(const std::string &filename, Visitor &visitor) {
  Parser<Visitor> p(filename, visitor);
  p.parse();
}

nameMatch namematching;



  void gettingDistNodes(const nameMatch& namematchin, const std::list<Stop>& stops) {
	  
	 
	  int distanceless30 = 0;
  
  std::cout << "files being compared may take some time, check output files in the meantime..." << std::endl;
  std::ofstream myfile2;
  myfile2.open("stopmorethan30.csv");

  std::ofstream myfile;
  myfile.open("stoplessthan30.csv");
  
  
  
  

  for (const auto &coord : namematchin.vlopsecondd) {
    for (const auto &stop : stops) {
     
      std::string stlow = stop.stop_lon;
      std::string stlaw = stop.stop_lat;

      
      double s = vincentydistt(stlow, stlaw, coord.loni, coord.lati);
      

      if (s < 30) {
        
        myfile << "osm lat"
               << ","
               << "osm lon"
               << ","
               << "gtfs lat"
               << ","
               << "gtfs lon"
			   << "\n";
        myfile << coord.lati << "," << coord.loni << "," << stop.stop_lat << ","
               << stop.stop_lon << "\n";
        
        distanceless30++;
        
      }  else {
        
        myfile2 << "osm lat"
                << ","
                << "osm lon"
                << ","
                << "gtfs lat"
                << ","
                << "gtfs lon"
                << "\n";
        myfile2 << coord.lati << "," << coord.loni << "," << stop.stop_lat
                << "," << stop.stop_lon  << "\n";
        
      }
    }

	
  }
   
  
  
  // myfile.close();
  //myfile2.close();

  std::cout << "Number of stops less than 30m away: " << distanceless30
            << std::endl;
	  
  }





int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt"
              << std::endl;
    
    return 1;
  }

  std::vector<nameMatch> namematchhh;
  nameMatch namematchin;

  readgtfs(stops, argv[2]);
  int countingNameMatches = 0;
  int countingNonNameMatches = 0;

  

  int chose;
  std::cout << "Choose whether to compare by name or by distance. Enter 1 for "
               "name and 2 for distance:  ";
  std::cin >> chose;

  switch (chose) {
  case 1: {
    
    readd_osm_pbf(argv[1], namematchin);
    std::cout << "We read for namematchin" << namematchin.nodes << " nodes, "
              << namematchin.ways << " ways and " << namematchin.relations
              << " relations" << std::endl;
    break;
  }
  case 2: {
	  /*
    Counter countt;
    rd_osm_pbf(argv[1], countt);
    std::cout << "We read for rd_osm_pbf" << countt.nodes << " nodes, "
              << countt.ways << " ways and " << countt.relations << " relations"
              << std::endl;
	*/
	readd_osm_pbf(argv[1], namematchin);
	gettingDistNodes(namematchin, stops);

    break;
  }
  default:
    // doesn't match any
    std::cout << "Error! Invalid option";
    break;
  }

  
  

  for (const auto &stop : stops) {
    
    for (const auto &osmm : namematchin.vlopsecondd) {

      auto nameTag = osmm.Tagg.find("name"); 
                                           
      auto nameTagThree = osmm.Tagg.find("name");
      
      std::string namess =
          (nameTagThree != osmm.Tagg.end()) ? nameTagThree->second : "";
      std::string namess2 = "A.Avenue & 10th Street"; //  use as test
      
      if (!namess.empty() && namess == stop.stop_name) {

        std::cout << stop.stop_name << " and " << namess << std::endl;
        countingNameMatches++; 
      } else {

        countingNonNameMatches++; 
      }
    }
  }

  std::cout << "Number of name matches: " << countingNameMatches << std::endl;
  std::cout << "Number of non-name matches: " << countingNonNameMatches
            << std::endl;
			
			
	

  	
  return 0;
}
