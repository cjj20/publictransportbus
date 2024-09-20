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
	
	osmcoordntag(double lati, double loni, const std::map<std::string, std::string> &tags = {}) : lati(lati), loni(loni), Tagg(tags) {}
	
	osmcoordntag() = default;

};

using VariantTypes = std::variant<osmcoordntag, Stop>;  //variant stuff

struct OSMBusStopFilter {
    
    int nodes;
    int ways;
    int relations;


	std::vector<osmcoordntag> vlopsecondd;
	
    OSMBusStopFilter() : nodes(0), ways(0), relations(0) {}

    // This method is called every time a Node is read
    void node_callback(uint64_t /*osmid*/, double lon, double lat, const Tags &tags){
        
        auto highw = tags.find("highway");
       
        if (highw != tags.end() && highw->second == "bus_stop") {
			++nodes;
			vlopsecondd.push_back({lat, lon, tags});
		   
		}
    }

    void way_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const std::vector<uint64_t> &/*refs*/){
        ++ways;
    }

    void relation_callback(uint64_t /*osmid*/, const Tags &/*tags*/, const References & /*refs*/){
        ++relations;
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

    std::tuple<double, double> getLatLonTuple() const {

        if (!gtfsstop1.stop_lat.empty() && !gtfsstop1.stop_lon.empty()) {
            return std::make_tuple(
                std::stod(gtfsstop1.stop_lat),
                std::stod(gtfsstop1.stop_lon));
        }
        else if (!std::isnan(osmcoordntaggs.lati) && !std::isnan(osmcoordntaggs.loni)) {
            return std::make_tuple(
                osmcoordntaggs.lati,
                osmcoordntaggs.loni);
        }
        
        return std::make_tuple(lat, lon);
    }

};


struct Point
{
    float x, y;
    Point(float x = 0, float y = 0):x(x), y(y){};
};


// Axis-aligned bounding box with half dimension and center
struct AABB {
   
	double centerX1;
	double centerY1;

    double halfDimensionX;
    double halfDimensionY;


    AABB(double x, double y, double dx, double dy)
        : centerX1(x)
        , centerY1(y)
        , halfDimensionX(dx)
        , halfDimensionY(dy)
        {}
	
    bool containsPoint(double x, double y) const {
     
		if (std::isnan(x) && std::isnan(y))
            return false;
        
        if (x > (centerX1 + halfDimensionX))
            return false;
        
        if (x < (centerX1 - halfDimensionX))
           return false;
		
        if (y > (centerY1 + halfDimensionY))
            return false;

        if (y < (centerY1 - halfDimensionY))
            return false;
		
		return true;
    }
		
    bool intersects(const AABB& other) const {

        if (containsPoint(other.centerX1 - other.halfDimensionX, other.centerY1 - other.halfDimensionY))
            return true;

        if (containsPoint(other.centerX1 + other.halfDimensionX, other.centerY1 - other.halfDimensionY))
            return true;

        if (containsPoint(other.centerX1 - other.halfDimensionX, other.centerY1 + other.halfDimensionY))
            return true;

        if (containsPoint(other.centerX1 + other.halfDimensionX, other.centerY1 + other.halfDimensionY))
            return true;

        if (other.containsPoint(centerX1 - halfDimensionX, centerY1 - halfDimensionY))
            return true;
        
        if (other.containsPoint(centerX1 + halfDimensionX, centerY1 - halfDimensionY))
            return true;
        
        if (other.containsPoint(centerX1 - halfDimensionX, centerY1 + halfDimensionY))
            return true;

        if (other.containsPoint(centerX1 + halfDimensionX, centerY1 + halfDimensionY))
            return true;

        return false;
    }

};
  
class QuadTree {
public:
   
	QuadTree ( const AABB& boundary)
        : boundary(boundary)
        , points()
        , northWest(nullptr)
        , northEast(nullptr)
        , southWest(nullptr)
        , southEast(nullptr)
        {}
    
    bool insert(const Stopq& stop) {
      
        // ignore objects that do not belong in this quad tree, if outside of bounds
        if (!boundary.containsPoint(stop.lat, stop.lon))
            return false;

         // If there is space in this quad tree and if doesn't have subdivisions, add the object here
        if (points.size() < NODE_CAPACITY) {
            points.push_back(stop);
            return true;
        }

         // Otherwise, subdivide and then add the point to whichever node will accept it
        if (northWest == nullptr)
            subdivide();

        // We have to add the points/data contained in this quad array to the new quads if we only want
        // the last node to hold the data
        if(northWest->insert(stop)) return true;
        if(northEast->insert(stop)) return true;
        if(southWest->insert(stop)) return true;
        if(southEast->insert(stop)) return true;

        // Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
        std::cerr << "Failed to insert point ("
            << stop.lat << ", " << stop.lon <<
            ") into any of the QuadTree leafs after subdivision." << std::endl;

        return false;
    }

    std::vector<Stopq> queryRange(const AABB& range) const {
          
        auto pointsInRange = std::vector<Stopq>();
        // std::vector<Stopq> pointsInRange2;

        // Automatically abort if the range does not intersect this quad
        if(!boundary.intersects(range)) {
            return pointsInRange;
        }

        // Check objects at this quad level
        for (auto point : points) {
            if (range.containsPoint(point.lon, point.lat))
                pointsInRange.push_back(point);
        }
        
        // exit if there are no children
        if(northWest == nullptr)
            return pointsInRange;

        // Otherwise, add the points from the children

        if (northWest->boundary.intersects(range)) {
            for (auto point : northWest->queryRange(range)){
                pointsInRange.push_back(point);
            }
        }

        if (northEast->boundary.intersects(range)) {
            for (auto point : northEast->queryRange(range)){
                pointsInRange.push_back(point);
            }
        }

        if (southWest->boundary.intersects(range)) {
            for (auto point : southWest->queryRange(range)){
                pointsInRange.push_back(point);
            }
        }

        if (southEast->boundary.intersects(range)) {
            for (auto point : southEast->queryRange(range)){
                pointsInRange.push_back(point);
            }
        }

        // TODO: Find push_all analogue or collections append

        return pointsInRange;

    }
    
    // TODO: Convert all points to EPSG:3857 before searching
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
		
    void print(int indent = 0) {

        if( indent == 0)
            std::cout << "==== Printing QuadTree ====" << std::endl;
        
        for ( int i = 0; i < indent; i++)
            std::cout << "*";
	
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
          
		double Quarter_halfDimensionX = boundary.halfDimensionX / 2;
		double Quarter_halfDimensionY = boundary.halfDimensionY / 2;
        double x2 = boundary.centerX1; //change center.x
        double y2 = boundary.centerY1; //change center.y

        northWest = std::make_unique<QuadTree>(AABB(x2 - (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2), Quarter_halfDimensionX, Quarter_halfDimensionY));
        southWest = std::make_unique<QuadTree>(AABB(x2 - (boundary.halfDimensionX / 2), y2 - (boundary.halfDimensionY / 2) , Quarter_halfDimensionX, Quarter_halfDimensionY ));
        northEast = std::make_unique<QuadTree>(AABB(x2 + (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2) , Quarter_halfDimensionX, Quarter_halfDimensionY ));
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

// TODO: Rewrite with AABB.extend(double x, double y)
AABB calculateStopsBB(const std::vector<Stop>& points) {

    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();

    for (const auto& point : points) {
        if (!point.stop_lat.empty() && !point.stop_lon.empty()) {
            double gtfsLat = std::stod(point.stop_lat);
            double gtfsLon = std::stod(point.stop_lon);

            minLat = std::min(minLat, gtfsLat);
            maxLat = std::max(maxLat, gtfsLat);
            minLon = std::min(minLon, gtfsLon);
            maxLon = std::max(maxLon, gtfsLon);
        }
    }

    double halfDimensionX = (maxLon - minLon) / 2.0;
    double halfDimensionY = (maxLat - minLat) / 2.0;
    double centerX = halfDimensionX + minLon;
    double centerY = halfDimensionY + minLat;

    return AABB(centerX, centerY, halfDimensionX, halfDimensionY);
};

// TODO: Rewrite with AABB.extend(double x, double y)
AABB calculateOsmStopsBB(const std::vector<osmcoordntag>& points) {

    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();

    for (const auto& point : points) {
        if (!std::isnan(point.lati) && !std::isnan(point.loni)) {
            minLat = std::min(minLat, point.lati);
            maxLat = std::max(maxLat, point.lati);
            minLon = std::min(minLon, point.loni);
            maxLon = std::max(maxLon, point.loni);
        }
    }

    double halfDimensionX = (maxLon - minLon) / 2.0;
    double halfDimensionY = (maxLat - minLat) / 2.0;
    double centerX = halfDimensionX + minLon;
    double centerY = halfDimensionY + minLat;

    return AABB(centerX, centerY, halfDimensionX, halfDimensionY);
};


int main (int argc, char** argv){
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt" << std::endl;
        return 1;
    }

    //gtfs reading
    readgtfs(stops, argv[2]);

	//osm reading
    OSMBusStopFilter osm_stops;
    Parser<OSMBusStopFilter> p(argv[1], osm_stops);
    p.parse();

    // QTree boundary for the whole world
    AABB boundary1(0.0, 0.0, 180.0, 90.0);
	QuadTree tree1(boundary1);
	
	// Index GTFS stops
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

		tree1.insert(stopqgts);
    }

    std::ofstream outputFile("proximityresults.txt");

    // Add osmcoordtags to points
    for (const auto& osmcoordstopq : osm_stops.vlopsecondd) {
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

        // TODO: Add constant for radius
		std::vector<Stopq> matches = tree1.searchByProximity(osmcoordstopq.lati, osmcoordstopq.loni, 500.0);
            
        for (const auto& match : matches) {
            // Furter filter by tags / name whatewer
            outputFile << "stop_gtfs_id\t" << match.gtfsstop1.stop_id << "\t";
            outputFile << "stop_osm_id\t" << osmcoordstopq.Tagg.at("id") << "\t";

            outputFile << "stop_gtfs_lat\t" << match.gtfsstop1.stop_lat << "\t";
            outputFile << "stop_gtfs_lon\t" << match.gtfsstop1.stop_lon << "\t";

            outputFile << "stop_osm_lat\t" << osmcoordstopq.lati << "\t";
            outputFile << "stop_osm_lon\t" << osmcoordstopq.loni << "\t";

            outputFile << "stop_gtfs_name\t" << match.gtfsstop1.stop_name << "\t";
            outputFile << "stop_osm_name\t" << osmcoordstopq.Tagg.at("name") << "\t";
            outputFile << "stop_osm_ref\t" << osmcoordstopq.Tagg.at("ref") << "\t";

            outputFile << std::endl;
        }

    }
	
    outputFile.close();
 
    return 0;
}