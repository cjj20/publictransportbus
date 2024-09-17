//quadtree
//command 1:
 //sudo g++ -O2 -o quad1 quadtree3Modified.cpp listgtfs22copy.cpp -losmpbf -lprotobuf -lz
//command 2: 
//./quad1 oregon-latest.osm.pbf oregon-stops.txt



#include <iostream>
#include <vector>
#include <memory> 
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
//#include "readgt.h"
#include "readgtcopy.h"
#include "libosmpbfreader/osmpbfreader.h"
#include <list>
#include <typeinfo>
#include <fstream>
#include <sstream> 
#include <iomanip> 
#include <stdexcept>
#include <variant>
//#include <map>

using namespace osmpbfreader;

const int NODE_CAPACITY = 4;

std::list<Stop> stops;


struct osmcoordntag {
	std::map<std::string, std::string> Tagg; 
	
	double lati;
	double loni;
	
	//osmcoordntag(double lati = std::numeric_limits<double>::quiet_NaN(), double loni  = std::numeric_limits<double>::quiet_NaN(), const std::map<std::string, std::string> &tags = {}) : lati(lati), loni(loni), Tagg(tags) {}
	
	osmcoordntag(double lati, double loni, const std::map<std::string, std::string> &tags = {}) : lati(lati), loni(loni), Tagg(tags) {}
	
	osmcoordntag() = default;

};

using VariantTypes = std::variant<osmcoordntag, Stop>;  //variant stuff

struct nameMatch { 
    
    int nodes;
    int ways;
    int relations;


    std::vector<Tags> tagss;
	std::vector<osmcoordntag> vlopsecondd;
	
    nameMatch() : nodes(0), ways(0), relations(0) {}

    // This method is called every time a Node is read
    void node_callback(uint64_t /*osmid*/, double lon, double lat, const Tags &tags){
        
        auto nam = tags.find("name");
        auto highw = tags.find("highway");
       
        if(highw != tags.end() && highw->second == "bus_stop") {
			++nodes;
			tagss.push_back(tags);
			vlopsecondd.push_back({lat, lon, tags}); 
		   
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

struct Stopq {
    double lat; 
    double lon; 
	int id;
    std::string code;
    std::string name;
    Stop gtfsstop1;  
	osmcoordntag osmcoordntaggs; 
	std::vector<VariantTypes> data22; 
	
	//variant  included
	Stopq(double lat, double lon, int id = 0, const std::string& code = "", const std::string& name = "", const std::vector<VariantTypes>& data22 = {}, Stop gtfsstop1 = {}, osmcoordntag osmcoordntaggs = {}) : lat(lat), lon(lon), id(id), code(code), name(name), data22(data22), gtfsstop1(gtfsstop1), osmcoordntaggs(osmcoordntaggs) {} 
    //Stopq(double lat, double lon, int id = 0, const std::string& code = "", const std::string& name = "", const std::vector<VariantTypes>& data22) = {}) : lat(lat), lon(lon), id(id), code(code), name(name), data22(data22), gtfsstop1(), osmcoordntaggs() {} 

	Stopq() = default;
	


};


struct Point
{
    float x, y;
    Point(float x = 0, float y = 0):x(x), y(y){};
};

// Axis-aligned bounding box with half dimension and center

struct AABB {
   
    Stopq center; 
	double centerX1;
	double centerY1;
    double halfDimension;
    double halfDimensionX; //  
    double halfDimensionY;     
   
    

    AABB(double x, double y, double dx, double dy) : centerX1(x), centerY1(y), halfDimensionX(dx), halfDimensionY(dy) {}
	
    bool containsPoint(const Stopq& stop) const {
     
		if (!std::isnan(stop.lon) && !std::isnan(stop.lat)) {
        
        if(stop.lon > (centerX1 + halfDimensionX)) //should be minus here to return true
            return false;
       
        
        if( stop.lon < (centerX1 - halfDimensionX))
           return false;
        
		
        if  (stop.lat > (centerY1 + halfDimensionY))
            return false;


        if (stop.lat < (centerY1 - halfDimensionY))
            return false;
        
		
		
		return true;
		}
		
		
		//  gtfs
		if (!std::isnan(std::stod(stop.gtfsstop1.stop_lon)) && !std::isnan(std::stod(stop.gtfsstop1.stop_lat))) {
		
        if(std::stod(stop.gtfsstop1.stop_lon) > (centerX1 + halfDimensionX)) //should be minus here to return true
            return false;
        
       
        
        if( std::stod(stop.gtfsstop1.stop_lon) < (centerX1 - halfDimensionX))
           return false;
        
		
		
        if  (std::stod(stop.gtfsstop1.stop_lat) > (centerY1 + halfDimensionY))
            return false;

      
        if (std::stod(stop.gtfsstop1.stop_lat) < (centerY1 - halfDimensionY))
            return false;
        
		
		
		
		return true;
		}
		
		
		// osm 
		if (!std::isnan(stop.osmcoordntaggs.loni) && !std::isnan(stop.osmcoordntaggs.lati)) {
		
        if(stop.osmcoordntaggs.loni > (centerX1 + halfDimensionX)) 
            return false;
        
		
        
        if(stop.osmcoordntaggs.loni < (centerX1 - halfDimensionX))
           return false;
        
		
		
        if  (stop.osmcoordntaggs.lati > (centerY1 + halfDimensionY))
            return false;

     
	 
        if (stop.osmcoordntaggs.lati < (centerY1 - halfDimensionY))
            return false;
        
		
		
		return true;
		}
		
		
		
		
        return false;

  
    }

     
     
     bool intersects(const AABB& other) const
    {
        //this right > that left                                          this left <s that right
        if(centerX1 + halfDimensionX > other.centerX1 - other.halfDimensionX || centerX1 - halfDimensionX < other.centerX1 + other.halfDimensionX)
        {
        // This bottom > that top
            if(centerY1 + halfDimensionY > other.centerY1 - other.halfDimensionY || centerY1 - halfDimensionY < other.centerY1 + other.halfDimensionY)
            {
                return true;
            }
        }
        return false;
    } 

};
  
class QuadTree {
public:
   
    ////QuadTree ( const AABB& boundary) : boundary(boundary), northWest(nullptr), northEast(nullptr), southWest(nullptr), southEast(nullptr), points(std::vector<Stop>()) {}
	QuadTree ( const AABB& boundary) : boundary(boundary), points(), northWest(nullptr), northEast(nullptr), southWest(nullptr), southEast(nullptr) {}

	
    
    
    bool insert(const Stopq& stop) { 
        //ignore objects that do not belong in this quad tree, if outside of bounds
      
         if(!boundary.containsPoint(stop)) {
            return false; //object cannot be added
         }
       

         // If there is space in this quad tree and if doesn't have subdivisions, add the object here
        
         if(points.size() < NODE_CAPACITY) {
            
             points.push_back(stop);
             return true;
         }

         // Otherwise, subdivide and then add the point to whichever node will accept it
         if(northWest == nullptr) {
             subdivide(); 
         }

         // We have to add the points/data contained in this quad array to the new quads if we only want
        // the last node to hold the data
         if(northWest->insert(stop)) return true;
         if(northEast->insert(stop)) return true;
         if(southWest->insert(stop)) return true;
         if(southEast->insert(stop)) return true;

         // Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
         return false;


          }

          std::vector<Stopq> queryRange(const AABB& range) const { 
          

          
          auto pointsInRange = std::vector<Stopq>();
          std::vector<Stopq> pointsInRange2;
          

           // Automatically abort if the range does not intersect this quad
           if(!boundary.intersects(range)) {
               return pointsInRange; 
            }

            // Check objects at this quad level
        for ( int p = 0; p < points.size(); p++)
        {
            if (range.containsPoint(points[p]))
                pointsInRange.push_back(points[p]);
        }
        
           
          // exit if there are no children
        if(northWest == nullptr)
            return pointsInRange;

        // Otherwise, add the points from the children
        auto points = northWest->queryRange(range);
        for ( auto point : points){
            pointsInRange.push_back(point);
        }
        points = northEast->queryRange(range);
        for(auto point : points){
            pointsInRange.push_back(point);
        }
        points = southWest->queryRange(range);
        for( auto point : points){
            pointsInRange.push_back(point);
        }
        points = southEast->queryRange(range);
        for(auto point : points){
            pointsInRange.push_back(point);
        }

        return pointsInRange;


       

          }          
     

        
        std::vector<Stopq> searchByProximity(double lat, double lon, double radius) {
            double earth_radius = 6371000.0;
       

            double x1 = lon - (180.0 / M_PI * (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
            double x2 = lon + (180.0 / M_PI * (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
            double y1 = lat + (radius / earth_radius * 180.0 / M_PI);
            double y2 = lat - (radius / earth_radius * 180.0 / M_PI);
    
            double xAbs = std::abs(x1 - x2) / 2.0;
            double yAbs = std::abs(y1 - y2) / 2.0;
            AABB area(lat, lon, xAbs, yAbs);

            return  queryRange(area);
        } 
		
		//function overloading
		/*
		std::vector<Stopq> searchByProximity(const std::string& latstring, const std::string& lonstring, double radius) {
            double earth_radius = 6371000.0;
			
			double lat = std::stod(latstring);
			double lon = std::stod(lonstring);
       
            
			return searchByProximity(lat, lon, radius);
			
        } 
		*/
		
		
		std::vector<Stopq> searchByProximity(const Stopq& stopqq, double radius) {
            double earth_radius = 6371000.0;
			
			double lat;
			double lon;
       

            if (!stopqq.gtfsstop1.stop_lat.empty() && !stopqq.gtfsstop1.stop_lon.empty()) {
        
                lat = std::stod(stopqq.gtfsstop1.stop_lat);
                lon = std::stod(stopqq.gtfsstop1.stop_lon);
       
            } else if (!std::isnan(stopqq.osmcoordntaggs.lati) && !std::isnan(stopqq.osmcoordntaggs.loni)) {
                lat = stopqq.osmcoordntaggs.lati;
                lon = stopqq.osmcoordntaggs.loni;
            } else {
				
				lat = stopqq.lat;
				lon = stopqq.lon;
			}



			return searchByProximity(lat, lon, radius);
			
        } 
		
		
		

        void print(int indent = 0) { //check
        if( indent == 0)
        std::cout << "==== Printing QuadTree ====" << std::endl;
        
        for ( int i = 0; i < indent; i++)
            std::cout << "*";
        ////std::cout << "* " << boundary << std::endl;
		std::cout << "* " << boundary.centerX1 << boundary.centerY1 << boundary.halfDimensionX << boundary.halfDimensionY << std::endl;
        
        // list points 
        ////for( auto point : points ){
		for( auto point : points ){
            ////std::cout << "| " << point << std::endl;
			std::cout << "| " << point.lat << point.lon << std::endl;
        }

        if(northWest == nullptr)
            return;    
        
        northWest->print(indent +1);
        northEast->print(indent +1);
        southWest->print(indent +1);
        southEast->print(indent +1);
    

    }
	

private: 
    
    //bounding box representing the boundaries of this quadtree
    AABB boundary;

    //points in this tree
    std::vector<Stopq> points;

    //children
	
	std::unique_ptr<QuadTree> northWest;
    std::unique_ptr<QuadTree> northEast;
    std::unique_ptr<QuadTree> southWest;
    std::unique_ptr<QuadTree> southEast;

  
       void subdivide() {
          
        double Quarter_halfDimension = boundary.halfDimension / 4;
		double Quarter_halfDimensionX = boundary.halfDimensionX / 2;
		double Quarter_halfDimensionY = boundary.halfDimensionY / 2;
        double x2 = boundary.centerX1; //change center.x
        double y2 = boundary.centerY1; //change center.y
       //// northWest = new QuadTree(AABB({x - (boundary.halfDimension / 2), y + (boundary.halfDimensionY / 2)}, Quarter_halfDimension));
        northWest = std::make_unique<QuadTree>(AABB(x2 - (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2), Quarter_halfDimensionX, Quarter_halfDimensionY));
        ////southWest = new QuadTree(AABB({x - (boundary.halfDimension / 2), y - (boundary.halfDimension / 2) }, Quarter_halfDimension ));
        southWest = std::make_unique<QuadTree>(AABB(x2 - (boundary.halfDimensionX / 2), y2 - (boundary.halfDimensionY / 2) , Quarter_halfDimensionX, Quarter_halfDimensionY ));
       //// northEast = new QuadTree(AABB({x + (boundary.halfDimension / 2), y + (boundary.halfDimension / 2) }, Quarter_halfDimension ));
        northEast = std::make_unique<QuadTree>(AABB(x2 + (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2) , Quarter_halfDimensionX, Quarter_halfDimensionY ));
     ////   southEast = new QuadTree(AABB({x + (boundary.halfDimension / 2), y - (boundary.halfDimension / 2) }, Quarter_halfDimension ));
        southEast = std::make_unique<QuadTree>(AABB(x2 + (boundary.halfDimensionX / 2), y2 - (boundary.halfDimensionY / 2) , Quarter_halfDimensionX, Quarter_halfDimensionY ));
       
	   
	   
	    for (const auto& pointt : points) {
            northWest->insert(pointt);
            northEast->insert(pointt);
            southWest->insert(pointt);
            southEast->insert(pointt);
        }

        points.clear();
    }
       

        };


template<typename nameMatch>  //osm
     void readd_osm_pbf(const std::string &filename, nameMatch &nameMatchin)  {
         Parser<nameMatch> p(filename, nameMatchin);
         p.parse();
     }

int main (int argc, char** argv){
	if(argc != 3) { // osm and gtfs args
         std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt" << std::endl;
          
        
         return 1;
     }
	
    
	//AABB boundary1(-512, -512, 512, 512);
    //auto tree = QuadTree(boundary);
	//QuadTree tree1(boundary1);
	
    std::vector<Stopq> points = {
        Stopq(2.0, 7.0),
        Stopq(3.0, 11.0),
        Stopq(2.0, 6.0),
        Stopq(1.0, 4.0),
        Stopq(6.0, 5.0),
        Stopq(7.0, 10.0)
    };
	
	std::vector<Stopq> pointsgtfs;


     double minLat = std::numeric_limits<double>::max();
     double maxLat = std::numeric_limits<double>::lowest();
     double minLon = std::numeric_limits<double>::max();
     double maxLon = std::numeric_limits<double>::lowest();


      for (const auto& point : points) {
		
		if (!std::isnan(point.lat)) {
		
			minLat = std::min(minLat, point.lat);
            maxLat = std::max(maxLat, point.lat);
			
			
		}
		
		if (!std::isnan(point.lon)) {
		
			minLon = std::min(minLon, point.lon);
            maxLon = std::max(maxLon, point.lon);
			
			
		}
		
		//conversion from std::string to double
		if (!std::isnan(std::stod(point.gtfsstop1.stop_lat))) {
		
			minLat = std::min(minLat, std::stod(point.gtfsstop1.stop_lat));
            maxLat = std::max(maxLat, std::stod(point.gtfsstop1.stop_lat));
			
			
		}
		
		if (!std::isnan(std::stod(point.gtfsstop1.stop_lon))) {
		
			minLon = std::min(minLon, std::stod(point.gtfsstop1.stop_lon));
            maxLon = std::max(maxLon, std::stod(point.gtfsstop1.stop_lon));
			
			
		}
		
		//osm
		if (!std::isnan(point.osmcoordntaggs.lati)) {
		
			minLat = std::min(minLat, point.osmcoordntaggs.lati);
            maxLat = std::max(maxLat, point.osmcoordntaggs.lati);
			
			
		}
		
		if (!std::isnan(point.osmcoordntaggs.loni)) {
		
			minLon = std::min(minLon, point.osmcoordntaggs.loni);
            maxLon = std::max(maxLon, point.osmcoordntaggs.loni);
			
			
		}
		
      }


      //calculating centre and half-dimensions
      double halfDimensionX = (maxLon - minLon) / 2;
      double halfDimensionY = (maxLat - minLat) / 2;
      double centerX = halfDimensionX + minLon;
      double centerY = halfDimensionY + minLat;
      
      
      AABB boundary(centerX, centerY, halfDimensionX, halfDimensionY);
      QuadTree quadTree(boundary);	  
	  //AABB aabb1(centerX, centerY, halfDimensionX, halfDimensionY); 
    



    //tree.print();
    
    std::cout << "inserting random points into tree !!" << std::endl;
    
	
	for (const auto& point : points) {
       quadTree.insert(point);
    }
	
	
	readgtfs(stops, argv[2]); //gtfs reading
	nameMatch namematchin; //osm
	
	//osm reading
	readd_osm_pbf(argv[1], namematchin);
	
	
	// Add (gtfs) stops to points
    for (const auto& gtfsstopstopq : stops) {
       
	
		Stopq stopqgts(
            0.0,  // Default latitude
            0.0,  // Default longitude
            0,    // Default ID
            "",   // Default code
            "",   // Default name
            {},   // Default data22
            gtfsstopstopq, // GTFS Stop data
            {}    // Default osmcoordntag
        );
		points.push_back(stopqgts);
		quadTree.insert(stopqgts);
		
		
    }
  

    // Add osmcoordtags to points
    for (const auto& osmcoordstopq : namematchin.vlopsecondd) {
        //Stopq stopqos;
		Stopq stopqos(
            0.0,   // Default latitude
            0.0,   // Default longitude
            0,              // Default ID
            "",             // Default code
            "",             // Default name
            {},             // Default data22
            {},             // Default Stop
            osmcoordstopq         // OSM coord data
        );
		points.push_back(stopqos);
		quadTree.insert(stopqos);
    }
	
	
	std::cout << "Number of points: " << points.size() << '\n';
	
	
    /*
    std::cout << "Get points within bounds " << std::endl;
    auto pointInRange = tree.queryRange(AABB {Stop{ 4.0, 6.0}, 2.0 ); 
    for ( auto point : pointInRange ){
        std::cout << point  << std::endl;
    } */

    // Search by proximity
	
	//Stop stop11;
    //stop11.stop_lat = "45.420609";
    //stop11.stop_lon = "-122.675671";

    // Initialize Stopq object with latitude and longitude directly
    //Stopq stopq11(0.0, 0.0, 0, " ", " ", {}, stop11, {});
   // Stopq stopq11(stop11.stop_lat, stop11.stop_lon, 0, " ", " ", {}, stop11, {});
   
   
	
	auto results = quadTree.searchByProximity(2.0, 2.0, 500000.0);
	
    std::cout << "Found " << results.size() << " points within the search radius." << std::endl;
    
	std::ofstream outputFile("proximityresults.txt");
	
	for (const auto& stopres : results) {
       //// std::cout << "Stop Name: " << stopres.name 
		outputFile << "Stop Name: " << stopres.name 
                  << ", ID: " << stopres.id 
                  << ", Code: " << stopres.code
                  << ", Latitude: " << stopres.lat			  
                  << ", Longitude: " << stopres.lon 
				  << "  GTFS Stop - Lat: " << stopres.gtfsstop1.stop_lat
                  << ", GTFS Stop Lon: " << stopres.gtfsstop1.stop_lon
                  << ", GTFS STop Name: " << stopres.gtfsstop1.stop_name
				  << "  OSM Coord - Lat: " << stopres.osmcoordntaggs.lati
                  << ", OSM Coord Lon: " << stopres.osmcoordntaggs.loni
				  //<< ", OSM Name: " << stopres.osmcoordntaggs.Tagg
                  << std::endl;
    }
	
	outputFile.close();
	
	
    std::cout << "We read for namematchin" << namematchin.nodes << " nodes, " << namematchin.ways << " ways and " << namematchin.relations << " relations" << std::endl;
    
	
    return 0;



}