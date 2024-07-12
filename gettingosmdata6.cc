/*
Copyright (c) 2001-2022, Hove
This is an example file, do whatever you want with it! (for example if you are in Paris, invite us for a beer)

This shows the simplest way to use the osm.pbf reader. It just counts the number of objects in the file.

To build this file :
g++ -O2 -o counter gettingosmdata.cc -losmpbf -lprotobuf

To run it:
./getdata isle-of-man-latest.osm.pbf
*/

#include "libosmpbfreader/osmpbfreader.h"
#include <iostream>
#include <string>
#include <list>
#include <typeinfo>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "readgt.h"
#include "vinceinv.h"

using namespace osmpbfreader;

    
   
    


//std::string xtt;

// std::list<std::uint64_t> oslis;
std::list<double> oslp;
std::list<double> oslpp;
//std::vector<osmcoord> vlop;
//std::tuple<uint64_t, double, double> oslis;





struct osmcoord {
     double lati;
     double loni;


};
//std::vector<osmcoord> vlop;


// We need to define a visitor with three methods that will be called while the file is read
struct Counter {
    // Three integers count how many times each object type occurs
    int nodes;
    int ways;
    int relations;

     std::vector<osmcoord> vlop;


    Counter() : nodes(0), ways(0), relations(0) {}

    // This method is called every time a Node is read
   // void node_callback(uint64_t /*osmid*/, double /*lon*/, double /*lat*/, const Tags &tags){
    void node_callback(uint64_t osmid, double lon, double lat, const Tags &tags){
        
        //gettng input 
        
        //std::string xt = "4502"; //works but not global variable

        //std::cout << "osmid" << osmid << "longitude"<< lon << "latitude" << lat <<std::endl; //this line works
        //std::cout << "tags" << &tags <<std::endl;

         
          auto highw = tags.find("highway");
          //auto nam = tags.find("name");
         /// auto refff = tags.find("ref"); //ref instead pf gtfs:stop_id used because no gtfs:stop_id tag for isle of man osm data
          //auto longg = tags.find("lon");
          //auto latt = tags.find("lat");
          //auto gtfsstopidd = tags.find("gtfs:stop_id"); //this is for osm data which contains that tag
          //if(highw != tags.end() && highw->second=="bus_stop" && nam != tags.end() &&nam->second=="Bungalow Footbridge") { //works name value taken from           //a node
          //if(highw != tags.end() && highw->second=="bus_stop" && refff != tags.end() && refff->second=="17401") { //works value taken from a node
          //if(highw != tags.end() && highw->second=="bus_stop" && refff != tags.end() && refff->second==xtt) { //xtt is any value user gives, works
          if(highw != tags.end() && highw->second=="bus_stop") {
          
            ++nodes;
          // osliss = std::make_tuple(osmid, lat, lon);
          // oslis.push_back(osmid);
          oslp.push_back(lat);
          oslpp.push_back(lon);
          vlop.push_back({lat, lon});
            
            std::cout << "osmid" << osmid <<  "tags" << &tags <<  "lon" << lon << "lat" << lat << std::endl; //this line works
            
        }




        
    }

    // This method is called every time a Way is read
    // refs is a vector that contains the reference to the nodes that compose the way
    void way_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const std::vector<uint64_t> &/*refs*/){
   // void way_callback(uint64_t osmid, const Tags &tags, const std::vector<uint64_t> &/*refs*/){

        ++ways;
        
         
           /// std::cout << "osmid" << osmid << std::endl; //this line works
       
       /// std::cout << "tags" << &tags << std::endl; //tags end up same term and seems to be in hex, it may be working

    }

    // This method is called every time a Relation is read
    // refs is a vector of pair corresponding of the relation type (Node, Way, Relation) and the reference to the object
    void relation_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const References & /*refs*/){
        ++relations;
    }
};

int main(int argc, char** argv) {
     if(argc != 2) {
         std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf" << std::endl;
         return 1;
     }

     


     std::list<Stop> stops;   
     readgtfs(stops);    
     

    // std::cout << "Enter a number: ";
   ///  getline(std::cin, xtt);


     
     Counter counter;
     read_osm_pbf(argv[1], counter);
     //std::cout << "We read " << nodes << " counter.nodes " << counter.ways << " ways and " << counter.relations << " relations" << std::endl;
     std::cout << "number of nodes that fit" << counter.nodes << std::endl; //this line works
     std::cout << "ways info" << counter.ways << std::endl; //this line works
    // for (const auto& strr : oslis) {
    //    std::cout << strr << std::endl;
   // }


   //  for (const auto& coordin : oslp) {
   //     std::cout << coordin <<std::endl;
   // }
     
     for (const auto& coordinn : oslpp) {
       std::cout << coordinn << std::endl;
    }


    std::vector<std::string> coord_prints;
    std::vector<std::string> stop_prints;

    int numgtfsstops = 0;
    int numosmstops = 0;
    
   
     for (const auto& coord : counter.vlop) {
        std::cout << "Lat: " << coord.lati << ", Lon: " << coord.loni << std::endl;
        numosmstops++;
      ///std::string coord_print = "Lat: " + std::to_string(coord.lati) + ", Lon: " + std::to_string(coord.loni);
       std::string coord_print = std::to_string(coord.lati);
        std::cout << coord_print << std::endl;
        coord_prints.push_back(coord_print);
    }

    for (const auto& stop : stops) {
        std::cout << "stop id" << stop.stop_id << "stop lat" <<  stop.stop_lat  << "stop lon" << stop.stop_lon << std::endl;
         numgtfsstops++;


    ///   std::string stop_print = "stop id: " + stop.stop_id + ", stop lat: " + stop.stop_lat + ", stop lon: " + stop.stop_lon;
        std::string stop_print = stop.stop_lat;
        std::cout << stop_print << std::endl;
        stop_prints.push_back(stop_print);

   }

    //double testt = 54.1517;
   // std::vector<double> doubleTestt;
   // doubleTestt.push_back(54.1517);
    
    // std::vector<std::string> doubleT;
    //doubleT.push_back("54.1517");
   //std::string doubless = "54.1517";


  
    int possiblematches = 0;
    int possiblenonmatches = numosmstops + numgtfsstops;
    
    
   
 


   // string stopla = std::to_string(coord.lati);
    //double stoplo = std::stod(stop.stop_lon);

   for (const auto& coord : counter.vlop) {
     for (const auto& stop : stops) {
     
    std::string latiStr = std::to_string(coord.lati);
    std::string loniStr = std::to_string(coord.loni);
    

  


  

    if ((latiStr.find(stop.stop_lat) != std::string::npos) || (loniStr.find(stop.stop_lon) != std::string::npos) ) {
    
   /// if (loniStr.find(stop.stop_lon) == 0 ) {
          possiblematches++;
  ///  if (latiStr.find(stop.stop_lat) != std::string::npos && loniStr.find(stop.stop_lon) != std::string::npos) {
       //// if(loniStr.find(stop.stop_lon) == 0) {
           std::cout << "possible matches osm - gtfs matches" << std::endl;
            std::cout << "osm lat:" << coord.lati << "  is matching/starts with gtfs lat val"  << stop.stop_lat << "osm lon is: " << coord.loni << std::endl;
          std::cout << "gtfs details: gtfs stop lon is:" <<  stop.stop_lon << "  gtfs stop lat  is: " << stop.stop_lat << std::endl;
        }   
        else if ((stop.stop_lat.find(latiStr) != std::string::npos) || (stop.stop_lon.find(loniStr) != std::string::npos) ) {
      ///  else if ((stop.stop_lat.find(latiStr) == 0) || (stop.stop_lon.find(loniStr) == 0)  ){
           
                 
             std::cout << "possible matches osm - gtfs matches" << std::endl;
            std::cout << "gtfs lat:" << stop.stop_lat << "  is matching/starts with osm lat val"  << coord.lati << "osm lon is: " << coord.loni << std::endl;
          std::cout << "gtfs details: gtfs stop lon is:" <<  stop.stop_lon << "  gtfs stop lat  is: " << stop.stop_lat << std::endl;

     
     }

     //// } //lonistr if
  
      }

  }



     possiblenonmatches = possiblenonmatches - possiblematches;
   
     
  
     std::cout << "Number of possible matches found based on latitude: " << possiblematches << std::endl;
     std::cout << "Number of possible non-matches found based on latitude: " << possiblenonmatches << std::endl;

      

     
     std::cout << "Number of gtfs stops available:  " << numgtfsstops << std::endl;
     std::cout << "Number of osm stops that are bus stops: " << numosmstops << std::endl;




    /// for (const auto& val : coord_prints) {
      //  if(val == doubless) {
       //     std::cout << "got one" << std::endl;
     //   }
     // if (val.find(doubless) == 0) {
     //       std::cout << val << " starts with " << doubless <<  std::endl;
      //  }

    //}
 
     int distanceless30 = 0;
    // int distancemore30 = 0;

    for (const auto& coord : counter.vlop) {
     for (const auto& stop : stops) {
     //  double stop_loo = stringToDouble(stop.stop_lon);
      //  double stop_laa = stringToDouble(stop.stop_lat);
       std::string stlow = stop.stop_lon;
       std::string stlaw = stop.stop_lat;

     //  double s = vincentydistt(stop.stop_lon, stop.stop_lat, coord.loni, coord.lati);
       double s = vincentydistt(stlow, stlaw, coord.loni, coord.lati);
     //   double s =  vincentydistt(stop_loo, stop_laa, coord.loni, coord.lati);


   //      std::stringstream ss(stop.stop_lon);

   // int num;
    //double doub;
   // std::string word;
   // ss >> doub;
   //  std::cout << "double: " << double << std::endl;
       
         if (s < 30) {
               distanceless30++;
               std::cout << "for less than 30m, distance away" << s  << std::endl;
               std::cout << "osm lat: " << coord.lati << "osm lon: " << coord.loni << std::endl;
               std::cout << "gtfs lat: " << stop.stop_lat << "gtfs lon: " << stop.stop_lon << std::endl;
          } //else{
              // int distancemore30++;
              //std::cout << "greater than 30 distance" << s << std::endl;
             //  std::cout << "osm lat:" << coord.lati << "osm lon:" << coord.loni << std::endl;
             //  std::cout << "gtfs lat: " << stop.stop_lat << "gtfs lon: " << stop.stop_lon << std::endl;

          //}

      } 
}
   


    std::cout<< "Number of stops less than 30m away: " << distanceless30 << std::endl;

    //////std::cout << std::get<0>(osliss) << std::endl;

    // print the value we got from the user
    //std::cout << x << std::endl;
    
     return 0;
}
