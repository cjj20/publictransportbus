
/*



This shows the simplest way to use the osm.pbf reader. It just counts the number of objects in the file.

To build this file :
g++ -O2 -o counter matchingnynam.cc listgtfs22.cpp -losmpbf -lprotobuf -lz

To run it:
./counter path_to_your_data.osm.pbf stops.txt
*/

#include "libosmpbfreader/osmpbfreader.h"
#include <list>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector> 
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstring>
#include "readgt.h"

using namespace osmpbfreader;


// reuse maybe  std::map<std::string, std::string> Tagg;

struct osmcoordntag {
	std::map<std::string, std::string> Tagg; //TODO change type
	// TODO remove or modify if needed std::map<std::string, std::string, std::less<>> Tagg;
	// const Tagg taggg; //TODO change or replace maybe
	double lati;
	double loni;
	
	osmcoordntag(double lati, double loni, const std::map<std::string, std::string> &tags) : lati(lati), loni(loni), Tagg(tags) {}
};


/*     struct tagz {
       // tags;
    }; */
	
    // std::string xtt;
	//gtfs stops
     std::list<Stop> stops;
	 /*
    //  readgtfs(stops, argv[2]);
    ///  auto xtt = stops.stop_name;
     //// stops::stop_name& xtt = stops.stop_name;
     ////readgtfs(stops, argv[2]);
     /*
     for (const auto& stop :stops) {
     std::cout << "gtfs stops names maybe" << stop.stop_name = xtt <<std::endl;
     std::cout << xtt << std::endl;
      }

*/


// We need to define a visitor with three methods that will be called while the file is read
struct nameMatch {
    // Three integers count how many times each object type occurs
    int nodes;
    int ways;
    int relations;


    std::vector<Tags> tagss;
	std::vector<osmcoordntag> vlopsecondd;
	
	
    // std::vector<tagz> taglisty;

    nameMatch() : nodes(0), ways(0), relations(0) {}

    // This method is called every time a Node is read
    void node_callback(uint64_t /*osmid*/, double lon, double lat, const Tags &tags){
        //++nodes;
       // std::string xtt; //= gtfs.stop_name;
        auto nam = tags.find("name");
        auto highw = tags.find("highway");
       //// if(highw != tags.end() && highw->second == "bus_stop" && nam != tags.end() && nam->second==xtt) {
        if(highw != tags.end() && highw->second == "bus_stop") {
			++nodes;
			tagss.push_back(tags);
			vlopsecondd.push_back({lat, lon, tags}); //TODO may need to remove or change this line
		   /*
			 for (const auto& stop : stops) {
			  auto xttt = stop.stop_name;
			 std::cout << "gtfs stops names maybe" << xttt <<std::endl;
			 ++nodes;
			 }
			*/
		}                
    }

    // This method is called every time a Way is read
    // refs is a vector that contains the reference to the nodes that compose the way
    void way_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const std::vector<uint64_t> &/*refs*/){
        ++ways;
    }

    // This method is called every time a Relation is read
    // refs is a vector of pair corresponding of the relation type (Node, Way, Relation) and the reference to the object
    void relation_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const References & /*refs*/){
        ++relations;
    }



	const std::vector<Tags>& getTagsList() const {
        return tagss;
    }

};

struct Counter {
    // Three integers count how many times each object type occurs
    int nodes;
    int ways;
    int relations;

    Counter() : nodes(0), ways(0), relations(0) {}

    // This method is called every time a Node is read
    void node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/, const Tags &/*tags*/){
        ++nodes;
    }

    // This method is called every time a Way is read
    // refs is a vector that contains the reference to the nodes that compose the way
    void way_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const std::vector<uint64_t> &/*refs*/){
        ++ways;
    }

    // This method is called every time a Relation is read
    // refs is a vector of pair corresponding of the relation type (Node, Way, Relation) and the reference to the object
    void relation_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const References & /*refs*/){
        ++relations;
    }
};


////nameMatch namematching;
     
     template<typename nameMatch>
     void readd_osm_pbf(const std::string &filename, nameMatch &nameMatchin)  {
         Parser<nameMatch> p(filename, nameMatchin);
         p.parse();
     }

    template<typename Visitor>
    void rd_osm_pbf(const std::string & filename, Visitor & visitor){
        Parser<Visitor> p(filename, visitor);
        p.parse();
    }
     // Let's read that file !
     nameMatch namematching;





    




int main(int argc, char** argv) {
     if(argc != 3) {
         std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt" << std::endl;
          ///readgtfs(stops, argv[2]);
        
         return 1;
     }

     std::vector<nameMatch> namematchhh;
     nameMatch namematchin;

     readgtfs(stops, argv[2]);
     int countingNameMatches = 0;
	 int countingNonNameMatches = 0;
	 
  /*    auto xtt = stops.stop_name;
    
     for (const auto& stop :stops) {
     
      auto xttt = stop.stop_name;
     std::cout << "gtfs stops names maybe" << xtt << std::endl;
     
      }


*/




/////namematching.node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/, const Tags &/*tags*/);


   /*  nameMatch namematchin;
     readd_osm_pbf(argv[1], namematchin);
     std::cout << "We read for namematchin" << namematchin.nodes << " nodes, " << namematchin.ways << " ways and " << namematchin.relations << " relations" << std::endl;
      
     

     Counter counter;
     read_osm_pbf(argv[1], counter);
     std::cout << "We read " << counter.nodes << " nodes, " << counter.ways << " ways and " << counter.relations << " relations" << std::endl;
     
     
     Counter countt;
     rd_osm_pbf(argv[1], countt);
     std::cout << "We read for rd_osm_pbf" << countt.nodes << " nodes, " << countt.ways << " ways and " << countt.relations << " relations" << std::endl;

*/


      int chose;
      std::cout << "Choose whether to compare by name or by distance. Enter 1 for name and 2 for distance:  ";
      std::cin >> chose;



      switch (chose) {
        case 1: {
           //TODo reuse  nameMatch namematchin;
            readd_osm_pbf(argv[1], namematchin);
            std::cout << "We read for namematchin" << namematchin.nodes << " nodes, " << namematchin.ways << " ways and " << namematchin.relations << " relations" << std::endl;
            break;
             }
        case 2: {
            Counter countt;
            rd_osm_pbf(argv[1], countt);
            std::cout << "We read for rd_osm_pbf" << countt.nodes << " nodes, " << countt.ways << " ways and " << countt.relations << " relations" << std::endl;
            break;
             }
        default:
            // doesn't match any 
            std::cout << "Error! Invalid option";
            break;
    } 


     //gtfs stops
  /*
   std::list<Stop> stops;
     readgtfs(stops, argv[2]);
     ///stops.stop_name = xtt;
     for (const auto& stop :stops) {
     std::cout << "gtfs stops names maybe" << stop.stop_name <<std::endl;
      }

*/

    
	
	
	
    for (const auto& stop : stops) {
	/* TODO REINSTATE std::cout << "gtfs stuff here" << std::endl;
     std::cout << "gtfs stops names maybe" << stop.stop_name << std::endl; */
      // for(const auto& osmm :  namematchhh) {
		 // TODO reuse  for(const auto& osmm : namematching.vlopsecondd) {
	     for(const auto& osmm : namematchin.vlopsecondd) {
			 
         // TODO fix this   for(const auto& osmm :  namematching.getTagsList()) {
          //const char* namess = osmm.tags().get_value_by_key("name"); // osmm.tags()["name"]
		  //ToDO REINSTATE std::cout << "Inside the for loop of vlopsecondd" << std::endl;
		  //TODO REINSTATE std::cout << "test0" << std::endl;
           auto nameTag = osmm.Tagg.find("name"); //TODO replace
		 //TODO reuse  auto nameTagTwo = Tagg["name"]; //Tagg["name"];
		   auto nameTagThree = osmm.Tagg.find("name");
		//TODO REINSTATE   std::cout << "test1" << std::endl;
		   
          // const char* namess = (nameTag != osmm.end()) ? nameTag->second.c_str() : nullptr; // osmm.tags()["name"]
		//TODO   const char* namess = (nameTag != osmm.Tagg.end()) ? nameTag->second.c_str() : nullptr;
           std::string namess = (nameTagThree != osmm.Tagg.end()) ? nameTagThree->second : "";
		   std::string namess2 = "A.Avenue & 10th Street";  //TODO! use as test
		 //TODO REINSTATE  std::cout << "test2" << std::endl;
		   //std::cout << nameTagThree->second << "second" << std::endl;
		/* TODO REINSTATE  if (nameTagThree != osmm.Tagg.end()) {
                // Output the value of nameTagThree
				std::cout << "Key 'name' " << nameTagThree->first << std::endl;
                std::cout << "Key 'name' found with value: " << nameTagThree->second << std::endl;
				
          *  } */
		   
		   //use std::string::npos
		   //if(namess.find(stop.stop_name) != std::string::npos {
		  //std::cout << "nameTag key: " << nameTagThree->first << " namess values: " << namess << std::endl;
		  /// TODO reuse ?  if(namess && !std::strcmp(namess, stop.stop_name.c_str())) {
		  if (!namess.empty() && namess == stop.stop_name) {
              
              std::cout << stop.stop_name << " and " << namess << std::endl;
			  countingNameMatches++;  ///may need to remove 
           } else {
		   
			// TODO REINSTATE   std::cout << namess << " no matching for osm name" << std::endl;
			// TODO REINSTATE   std::cout << stop.stop_name << " no matching " << std::endl;
			   countingNonNameMatches++; //may need to remove
		   }
     }
     
    }


//operator and get value by key are a part of other class etc
/*
  public:

     const char* get_value_by_key(const char* key, const char* default_value = nullptr) const noexcept {
            assert(key);
            const auto result = find_key(key);
            return result == cend() ? default_value : result->value();
        }
 
        const char* operator[](const char* key) const noexcept {
            return get_value_by_key(key);
        }
*/
    
    std::cout << "Number of name matches: " << countingNameMatches << std::endl;
    std::cout << "Number of non-name matches: " << countingNonNameMatches << std::endl;
     return 0;
}
