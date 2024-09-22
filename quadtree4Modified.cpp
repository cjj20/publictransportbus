// quadtree
// command 1:
// sudo g++ -O2 -o quad1 quadtree3Modified.cpp listgtfs22copy.cpp -losmpbf
// -lprotobuf -lz
// command 2:
//./quad1 oregon-latest.osm.pbf oregon-stops.txt

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
//#include "readgt.h"
#include "libosmpbfreader/osmpbfreader.h"
#include "readgtcopy.h"
#include <fstream>
#include <iomanip>
#include <list>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <variant>
//#include <map>

using namespace osmpbfreader;

class QuadTree; // forward declaration

const int NODE_CAPACITY = 4;

std::list<Stop> stops;

struct osmcoordntag {
  std::map<std::string, std::string> Tagg;

  double lati;
  double loni;

  // osmcoordntag(double lati = std::numeric_limits<double>::quiet_NaN(), double
  // loni  = std::numeric_limits<double>::quiet_NaN(), const
  // std::map<std::string, std::string> &tags = {}) : lati(lati), loni(loni),
  // Tagg(tags) {}

  osmcoordntag(double lati, double loni,
               const std::map<std::string, std::string> &tags = {})
      : lati(lati), loni(loni), Tagg(tags) {}

  osmcoordntag() = default;
};

using VariantTypes = std::variant<osmcoordntag, Stop>; // variant stuff

struct nameMatch {

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
      vlopsecondd.push_back({lat, lon, tags});
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

struct Stopq {
  double lat;
  double lon;
  int id;
  std::string code;
  std::string name;
  Stop gtfsstop1;
  osmcoordntag osmcoordntaggs;
  std::vector<VariantTypes> data22;

  // variant  included
  Stopq(double lat, double lon, int id = 0, const std::string &code = "",
        const std::string &name = "",
        const std::vector<VariantTypes> &data22 = {}, Stop gtfsstop1 = {},
        osmcoordntag osmcoordntaggs = {})
      : lat(lat), lon(lon), id(id), code(code), name(name), data22(data22),
        gtfsstop1(gtfsstop1), osmcoordntaggs(osmcoordntaggs) {}
  // Stopq(double lat, double lon, int id = 0, const std::string& code = "",
  // const std::string& name = "", const std::vector<VariantTypes>& data22) =
  // {}) : lat(lat), lon(lon), id(id), code(code), name(name), data22(data22),
  // gtfsstop1(), osmcoordntaggs() {}

  Stopq() = default;
  //
  // double calculateDistance(double targetLat, double targetLon) {
  //   return haversDist(targetLat, targetLon, stopqq);
  //}

  // double calculateDistance(double targetLat, double targetLon ) const {
  //    return QuadTree::haversDist(targetLat, targetLon, *this);

  //}
};

struct Point {
  float x, y;
  Point(float x = 0, float y = 0) : x(x), y(y){};
};

// Axis-aligned bounding box with half dimension and center

struct AABB {

  Stopq center;
  double centerX1;
  double centerY1;
  double halfDimension;
  double halfDimensionX; //
  double halfDimensionY;

  AABB(double x, double y, double dx, double dy)
      : centerX1(x), centerY1(y), halfDimensionX(dx), halfDimensionY(dy) {}

  bool containsPoint(const Stopq &stop) const {

    // test data
    if (!std::isnan(stop.lon) && !std::isnan(stop.lat)) {

      if (stop.lon > (centerX1 + halfDimensionX) ||
          stop.lon < (centerX1 -
                      halfDimensionX)) // should be minus here to return true
        return false;

      // if( stop.lon < (centerX1 - halfDimensionX))
      //    return false;

      if (stop.lat > (centerY1 + halfDimensionY) ||
          stop.lat < (centerY1 - halfDimensionY))
        return false;

      // if (stop.lat < (centerY1 - halfDimensionY))
      //     return false;

      return true;
    }

    /*
         if (!std::isnan(stop.lon) && !std::isnan(stop.lat)) {

 if(stop.lon > (centerX1 + halfDimensionX)) //should be minus here to return
 true return false;


 if( stop.lon < (centerX1 - halfDimensionX))
    return false;


 if  (stop.lat > (centerY1 + halfDimensionY))
     return false;


 if (stop.lat < (centerY1 - halfDimensionY))
     return false;



         return true;
         }
         */

    //  gtfs
    if (!std::isnan(std::stod(stop.gtfsstop1.stop_lon)) &&
        !std::isnan(std::stod(stop.gtfsstop1.stop_lat))) {

      if (std::stod(stop.gtfsstop1.stop_lon) > (centerX1 + halfDimensionX) ||
          std::stod(stop.gtfsstop1.stop_lon) < (centerX1 - halfDimensionX))
        return false;

      // if( std::stod(stop.gtfsstop1.stop_lon) < (centerX1 - halfDimensionX))
      //   return false;

      if (std::stod(stop.gtfsstop1.stop_lat) > (centerY1 + halfDimensionY) ||
          std::stod(stop.gtfsstop1.stop_lat) < (centerY1 - halfDimensionY))
        return false;

      // if (std::stod(stop.gtfsstop1.stop_lat) < (centerY1 - halfDimensionY))
      //    return false;

      return true;
    }

    /*
    if (!std::isnan(std::stod(stop.gtfsstop1.stop_lon)) &&
!std::isnan(std::stod(stop.gtfsstop1.stop_lat))) {

if(std::stod(stop.gtfsstop1.stop_lon) > (centerX1 + halfDimensionX)) //should be
minus here to return true return false;



if( std::stod(stop.gtfsstop1.stop_lon) < (centerX1 - halfDimensionX))
return false;



if  (std::stod(stop.gtfsstop1.stop_lat) > (centerY1 + halfDimensionY))
return false;


if (std::stod(stop.gtfsstop1.stop_lat) < (centerY1 - halfDimensionY))
return false;




    return true;
    }
    */

    // osm
    if (!std::isnan(stop.osmcoordntaggs.loni) &&
        !std::isnan(stop.osmcoordntaggs.lati)) {

      if (stop.osmcoordntaggs.loni > (centerX1 + halfDimensionX) ||
          stop.osmcoordntaggs.loni < (centerX1 - halfDimensionX))
        return false;

      // if(stop.osmcoordntaggs.loni < (centerX1 - halfDimensionX))
      //   return false;

      if (stop.osmcoordntaggs.lati > (centerY1 + halfDimensionY) ||
          stop.osmcoordntaggs.lati < (centerY1 - halfDimensionY))
        return false;

      // if (stop.osmcoordntaggs.lati < (centerY1 - halfDimensionY))
      //     return false;

      return true;
    }

    /*
    if (!std::isnan(stop.osmcoordntaggs.loni) &&
!std::isnan(stop.osmcoordntaggs.lati)) {

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
    */

    return true;
  }

  bool intersects(const AABB &other) const {
    // this right > that left                                          this left
    // <s that right
    if (centerX1 + halfDimensionX > other.centerX1 - other.halfDimensionX ||
        centerX1 - halfDimensionX < other.centerX1 + other.halfDimensionX) {
      // This bottom > that top
      if (centerY1 + halfDimensionY > other.centerY1 - other.halfDimensionY ||
          centerY1 - halfDimensionY < other.centerY1 + other.halfDimensionY) {
        return true;
      }
    }
    return false;
  }
};

class QuadTree {
public:
  std::vector<Stopq> points;

  ////QuadTree ( const AABB& boundary) : boundary(boundary), northWest(nullptr),
  ///northEast(nullptr), southWest(nullptr), southEast(nullptr),
  ///points(std::vector<Stop>()) {}
  QuadTree(const AABB &boundary)
      : boundary(boundary), points(), northWest(nullptr), northEast(nullptr),
        southWest(nullptr), southEast(nullptr) {}

  bool insert(const Stopq &stop) {
    // ignore objects that do not belong in this quad tree, if outside of bounds

    if (!boundary.containsPoint(stop)) {
      return false; // object cannot be added
    }

    // If there is space in this quad tree and if doesn't have subdivisions, add
    // the object here

    if (points.size() < NODE_CAPACITY) {

      points.push_back(stop);
      return true;
    }

    // Otherwise, subdivide and then add the point to whichever node will accept
    // it
    if (northWest == nullptr) {
      subdivide();
    }

    // We have to add the points/data contained in this quad array to the new
    // quads if we only want
    // the last node to hold the data
    if (northWest->insert(stop))
      return true;
    if (northEast->insert(stop))
      return true;
    if (southWest->insert(stop))
      return true;
    if (southEast->insert(stop))
      return true;

    // Otherwise, the point cannot be inserted for some unknown reason (this
    // should never happen)
    return false;
  }

  std::vector<Stopq> queryRange(const AABB &range) const {

    auto pointsInRange = std::vector<Stopq>();
    std::vector<Stopq> pointsInRange2;

    // Automatically abort if the range does not intersect this quad
    if (!boundary.intersects(range)) {
      return pointsInRange;
    }

    // Check objects at this quad level
    for (int p = 0; p < points.size(); p++) {
      if (range.containsPoint(points[p]))
        pointsInRange.push_back(points[p]);
    }

    // exit if there are no children
    if (northWest == nullptr)
      return pointsInRange;

    // Otherwise, add the points from the children
    auto points = northWest->queryRange(range);
    for (auto point : points) {
      pointsInRange.push_back(point);
    }
    points = northEast->queryRange(range);
    for (auto point : points) {
      pointsInRange.push_back(point);
    }
    points = southWest->queryRange(range);
    for (auto point : points) {
      pointsInRange.push_back(point);
    }
    points = southEast->queryRange(range);
    for (auto point : points) {
      pointsInRange.push_back(point);
    }

    return pointsInRange;
  }

  std::vector<Stopq> searchByProxim(double lat, double lon, double radius) {
    double earth_radius = 6371000.0;

    double x1 = lon - (180.0 / M_PI *
                       (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
    double x2 = lon + (180.0 / M_PI *
                       (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
    double y1 = lat + (radius / earth_radius * 180.0 / M_PI);
    double y2 = lat - (radius / earth_radius * 180.0 / M_PI);

    double xAbs = std::abs(x1 - x2) / 2.0;
    double yAbs = std::abs(y1 - y2) / 2.0;
    AABB area(lat, lon, xAbs, yAbs);

    return queryRange(area);
  }

  /*
  std::vector<Stopq> searchByProximity(double lat, double lon, double radius) {
     Stopq stopqq;
             std::vector<Stopq> stopqresults;
             double earth_radius = 6371000.0;

                  if (!std::isnan(stopqq.osmcoordntaggs.lati) &&
  !std::isnan(stopqq.osmcoordntaggs.loni)) { double olat =
  stopqq.osmcoordntaggs.lati; double olon = stopqq.osmcoordntaggs.loni; auto
  osmres = searchByProxim(olat, olon, radius);
                          //saving vector into vector
                          stopqresults.insert(stopqresults.begin(),
  osmres.begin(), osmres.end());

      }

                  if (!stopqq.gtfsstop1.stop_lat.empty() &&
  !stopqq.gtfsstop1.stop_lon.empty()) {

          double glat = std::stod(stopqq.gtfsstop1.stop_lat);
          double glon = std::stod(stopqq.gtfsstop1.stop_lon);
                          auto gtfsres = searchByProxim(glat, glon, radius);
                          stopqresults.insert(stopqresults.begin(),
  gtfsres.begin(), gtfsres.end());

      }


                  if (!std::isnan(stopqq.lat) && !std::isnan(stopqq.lon)){

                          double tlat = stopqq.lat;
                          double tlon = stopqq.lon;

                          auto otherres = searchByProxim(tlat, tlon, radius);
                          stopqresults.insert(stopqresults.begin(),
  otherres.begin(), otherres.end());





                  //return  queryRange(area);
                  }

                  return stopqresults;




  }
          */
  // function overloading
  /*
  std::vector<Stopq> searchByProximity(const std::string& latstring, const
std::string& lonstring, double radius) { double earth_radius = 6371000.0;

          double lat = std::stod(latstring);
          double lon = std::stod(lonstring);


          return searchByProximity(lat, lon, radius);

}
  */
  static double deg2rad(double deg) { // inline double?
    return M_PI * deg / 180.0;
  }

  static double haversDistance(double lat1, double lon1, double lat2,
                               double lon2) {

    double latRad1 = deg2rad(lat1);
    double latRad2 = deg2rad(lat2);
    double lonRad1 = deg2rad(lon1);
    double lonRad2 = deg2rad(lon2);

    double difflat = latRad2 - latRad1;
    double difflon = lonRad2 - lonRad1;

    double earthRadiusM = 6371000.0;
    double u = sin((latRad2 - latRad1) / 2);
    double v = sin((lonRad2 - lonRad1) / 2);
    return 2.0 * earthRadiusM *
           asin(sqrt(u * u + cos(latRad1) * cos(latRad2) * v *
                                 v)); // distance in metres
  }

  static double roundToDecimalPlaces(double value, int decimalPlaces) {
    if (std::abs(value) > 1e6) { //
      double scale = std::pow(10.0, decimalPlaces);
      return std::round(value * scale) / scale;
    }
    return value; //
  }

  static double getOsmCoordinates(double targetLat, double targetLon,
                                  const Stopq &stopqq) {

    double lat;
    double lon;

    if (!std::isnan(stopqq.osmcoordntaggs.lati) &&
        !std::isnan(stopqq.osmcoordntaggs.loni)) {
      lat = (stopqq.osmcoordntaggs.lati < 1e-6)
                ? stopqq.osmcoordntaggs.lati
                : roundToDecimalPlaces(stopqq.osmcoordntaggs.lati, 6);
      lon = (stopqq.osmcoordntaggs.loni < 1e-6)
                ? stopqq.osmcoordntaggs.loni
                : roundToDecimalPlaces(stopqq.osmcoordntaggs.loni, 6);
      // haversDistance(targetLat, targetLon, lat, lon);
    }
    return haversDistance(targetLat, targetLon, lat, lon);
  }

  static double getGtfsCoordinates(double targetLat, double targetLon,
                                   const Stopq &stopqq) {
    double lat;
    double lon;

    if (!stopqq.gtfsstop1.stop_lat.empty() &&
        !stopqq.gtfsstop1.stop_lon.empty()) {
      try {
        double tempLat = std::stod(stopqq.gtfsstop1.stop_lat);
        double tempLon = std::stod(stopqq.gtfsstop1.stop_lon);
        lat = (tempLat < 1e-6) ? tempLat : roundToDecimalPlaces(tempLat, 6);
        lon = (tempLon < 1e-6) ? tempLon : roundToDecimalPlaces(tempLon, 6);
      } catch (const std::invalid_argument &e) {
        // Handle conversion error by setting lat and lon to 0.0 (already
        // initialized)
        lat = 0.0;
        lon = 0.0;
      }
      // haversDistance(targetLat, targetLon, lat, lon);
    }

    // Call the primary haversine function with stop coordinates
    return haversDistance(targetLat, targetLon, lat, lon);
  }

  static double haversDist2(double targetLat, double targetLon,
                            const Stopq &stopqq) {

    std::vector<Stopq> stophavers;

    double lat;
    double lon;

    if ((!stopqq.gtfsstop1.stop_lat.empty() &&
         !stopqq.gtfsstop1.stop_lon.empty()) ||
        (!std::isnan(stopqq.osmcoordntaggs.lati) &&
         !std::isnan(stopqq.osmcoordntaggs.loni)) ||
        (!std::isnan(stopqq.lat) && !std::isnan(stopqq.lon))) {

      // Get latitude and longitude from the given conditions
      double gtfscoors = getGtfsCoordinates(targetLat, targetLon, stopqq);
      double osmcoors = getOsmCoordinates(targetLat, targetLon, stopqq);
      /*
      else {
// lat = roundToDecimalPlaces(stopqq.lat, 6);
// lon = roundToDecimalPlaces(stopqq.lon, 6);
           lat = (stopqq.lat  < 1e-6) ? stopqq.lat :
roundToDecimalPlaces(stopqq.lat, 6); lon = (stopqq.lon  < 1e-6) ? stopqq.lon :
roundToDecimalPlaces(stopqq.lon, 6);


}
      */
    }

    // QuadTree qt;
    // return qt.haversDistance(targetLat, targetLon, lat, lon);

    // Call the primary haversine function with stop coordinates
    return haversDistance(targetLat, targetLon, lat, lon);
  }

  static double haversDist(double targetLat, double targetLon,
                           const Stopq &stopqq) {

    double lat;
    double lon;

    if ((!stopqq.gtfsstop1.stop_lat.empty() &&
         !stopqq.gtfsstop1.stop_lon.empty()) ||
        (!std::isnan(stopqq.osmcoordntaggs.lati) &&
         !std::isnan(stopqq.osmcoordntaggs.loni)) ||
        (!std::isnan(stopqq.lat) && !std::isnan(stopqq.lon))) {

      // Get latitude and longitude from the given conditions
      if (!stopqq.gtfsstop1.stop_lat.empty() &&
          !stopqq.gtfsstop1.stop_lon.empty()) {
        try {
          // lat = roundToDecimalPlaces(std::stod(stopqq.gtfsstop1.stop_lat),
          // 6); lon =
          // roundToDecimalPlaces(std::stod(stopqq.gtfsstop1.stop_lon), 6);
          double tempLat = std::stod(stopqq.gtfsstop1.stop_lat);
          double tempLon = std::stod(stopqq.gtfsstop1.stop_lon);
          lat = (tempLat < 1e-6) ? tempLat : roundToDecimalPlaces(tempLat, 6);
          lon = (tempLon < 1e-6) ? tempLon : roundToDecimalPlaces(tempLon, 6);

        } catch (const std::invalid_argument &e) {
          lat = 0.0;
          lon = 0.0;
        }
      }
      // else
      if (!std::isnan(stopqq.osmcoordntaggs.lati) &&
          !std::isnan(stopqq.osmcoordntaggs.loni)) {
        // lat = roundToDecimalPlaces(stopqq.osmcoordntaggs.lati, 6);
        // lon = roundToDecimalPlaces(stopqq.osmcoordntaggs.loni, 6
        lat = (stopqq.osmcoordntaggs.lati < 1e-6)
                  ? stopqq.osmcoordntaggs.lati
                  : roundToDecimalPlaces(stopqq.osmcoordntaggs.lati, 6);
        lon = (stopqq.osmcoordntaggs.loni < 1e-6)
                  ? stopqq.osmcoordntaggs.loni
                  : roundToDecimalPlaces(stopqq.osmcoordntaggs.loni, 6);

      } else {
        // lat = roundToDecimalPlaces(stopqq.lat, 6);
        // lon = roundToDecimalPlaces(stopqq.lon, 6);
        lat = (stopqq.lat < 1e-6) ? stopqq.lat
                                  : roundToDecimalPlaces(stopqq.lat, 6);
        lon = (stopqq.lon < 1e-6) ? stopqq.lon
                                  : roundToDecimalPlaces(stopqq.lon, 6);
      }
    }

    // QuadTree qt;
    // return qt.haversDistance(targetLat, targetLon, lat, lon);

    // Call the primary haversine function with stop coordinates
    return haversDistance(targetLat, targetLon, lat, lon);
  }

  double calculateDistance(double targetLat, double targetLon,
                           const Stopq &stopqq) {
    return haversDist(targetLat, targetLon, stopqq);
  }

  double calculateDistance2(double targetLat, double targetLon,
                            const Stopq &stopqq) {
    return haversDist2(targetLat, targetLon, stopqq);
  }

  double calculateDistance3(double targetLat, double targetLon,
                            const Stopq &stopqq) {
    return getGtfsCoordinates(targetLat, targetLon, stopqq);
  }

  double calculateDistance4(double targetLat, double targetLon,
                            const Stopq &stopqq) {
    return getOsmCoordinates(targetLat, targetLon, stopqq);
  }

  // double calculateDistance(double targetLat, double targetLon) {
  // Call the wrapper haversine function
  //    return haversDist(targetLat, targetLon, stopqq);
  //}

  /// std::vector<Stopq> searchByProxim2(const std::vector<Stopq>& points,
  /// double lat, double lon, double radius) {
  std::vector<Stopq> searchByProxim2(double lat, double lon, double radius) {

    ////
    std::vector<Stopq> results2;
    std::cout << "Number of points to search: " << points.size() << std::endl;
    std::cout << "Searching for points within radius: " << radius << " from ("
              << lat << ", " << lon << ")" << std::endl;

    /*
    ABB range = createBoundingBox(lat, lon, radius);

// Get candidate points within the bounding box
auto candidates = queryRange(range);

// Filter candidates using the Haversine distance
for (const auto& point : candidates) {
double distance = haversine(lat, lon, point.lat, point.lon);  */
    std::vector<Stopq> candidates1;
    AABB range = createBoundingBox(lat, lon, radius);
    auto candidates = queryRange(range);
    std::vector<Stopq> candidates2;
    // QuadTree& quadtree2;
    candidates2 = queryRange(range);

    for (const auto &allpoints : points) {
      // for(const auto& allpoints : candidates2) {
      //
      /// double distanceFrom = allpoints.calculateDistance(lat, lon);

      /// double distanceFrom = calculateDistance(lat, lon, allpoints);

      /*
      double distanceFrom = calculateDistance3(lat, lon, allpoints);
      std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
<< allpoints.lat << ", " << allpoints.lon << "): " << distanceFrom << std::endl;
      std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
<< allpoints.gtfsstop1.stop_lat << ", " << allpoints.gtfsstop1.stop_lon << "): "
<< distanceFrom << std::endl; std::cout << "Distance from (" << lat << ", " <<
lon << ") to point ("
<< allpoints.osmcoordntaggs.lati << ", " << allpoints.osmcoordntaggs.loni << "):
" << distanceFrom << std::endl;
      */

      double distanceFrom = calculateDistance3(lat, lon, allpoints);
      // std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
      //  << allpoints.lat << ", " << allpoints.lon << "): " << distanceFrom <<
      //  std::endl;
      std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
                << allpoints.gtfsstop1.stop_lat << ", "
                << allpoints.gtfsstop1.stop_lon << "): " << distanceFrom
                << std::endl;
      // std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
      // << allpoints.osmcoordntaggs.lati << ", " <<
      // allpoints.osmcoordntaggs.loni << "): " << distanceFrom << std::endl;
      if (distanceFrom <= radius) {

        results2.push_back(allpoints);
      }
    }

    for (const auto &allpoints : points) {
      // for(const auto& allpoints : candidates2) {
      //
      /// double distanceFrom = allpoints.calculateDistance(lat, lon);

      /// double distanceFrom = calculateDistance(lat, lon, allpoints);
      double distanceFrom = calculateDistance4(lat, lon, allpoints);
      // std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
      //  << allpoints.lat << ", " << allpoints.lon << "): " << distanceFrom <<
      //  std::endl;
      // std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
      //  << allpoints.gtfsstop1.stop_lat << ", " <<
      //  allpoints.gtfsstop1.stop_lon << "): " << distanceFrom << std::endl;
      std::cout << "Distance from (" << lat << ", " << lon << ") to point ("
                << allpoints.osmcoordntaggs.lati << ", "
                << allpoints.osmcoordntaggs.loni << "): " << distanceFrom
                << std::endl;

      if (distanceFrom <= radius) {

        results2.push_back(allpoints);
      }
    }

    std::cout << "Total number of points contained within given radius: "
              << results2.size() << std::endl;
    return results2;
  }

  AABB createBoundingBox(double lat, double lon, double radius) const {
    double halfWidth =
        radius / 111320; // Approximate conversion from meters to degrees
    double halfHeight =
        radius / 110540; // Approximate conversion from meters to degrees
    return AABB(lon - halfWidth, lat - halfHeight, lon + halfWidth,
                lat + halfHeight);
  }

  std::vector<Stopq> searchByProximity(double lat, double lon, double radius) {
    double earth_radius = 6371000.0;

    double x1 = lon - (180.0 / M_PI *
                       (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
    double x2 = lon + (180.0 / M_PI *
                       (radius / earth_radius / std::cos(lat * M_PI / 180.0)));
    double y1 = lat + (radius / earth_radius * 180.0 / M_PI);
    double y2 = lat - (radius / earth_radius * 180.0 / M_PI);

    double xAbs = std::abs(x1 - x2) / 2.0;
    double yAbs = std::abs(y1 - y2) / 2.0;
    AABB area(lat, lon, xAbs, yAbs);

    return queryRange(area);
  }

  std::vector<Stopq> searchByProximity(const Stopq &stopqq, double radius) {
    double earth_radius = 6371000.0;

    double lat;
    double lon;

    if (!stopqq.gtfsstop1.stop_lat.empty() &&
        !stopqq.gtfsstop1.stop_lon.empty()) {

      lat = std::stod(stopqq.gtfsstop1.stop_lat);
      lon = std::stod(stopqq.gtfsstop1.stop_lon);

    } else if (!std::isnan(stopqq.osmcoordntaggs.lati) &&
               !std::isnan(stopqq.osmcoordntaggs.loni)) {
      lat = stopqq.osmcoordntaggs.lati;
      lon = stopqq.osmcoordntaggs.loni;
    } else {

      lat = stopqq.lat;
      lon = stopqq.lon;
    }

    return searchByProximity(lat, lon, radius);
  }

  void print(int indent = 0) { // check
    if (indent == 0)
      std::cout << "==== Printing QuadTree ====" << std::endl;

    for (int i = 0; i < indent; i++)
      std::cout << "*";
    ////std::cout << "* " << boundary << std::endl;
    std::cout << "* " << boundary.centerX1 << boundary.centerY1
              << boundary.halfDimensionX << boundary.halfDimensionY
              << std::endl;

    // list points
    ////for( auto point : points ){
    for (auto point : points) {
      ////std::cout << "| " << point << std::endl;
      std::cout << "| " << point.lat << point.lon << std::endl;
    }

    if (northWest == nullptr)
      return;

    northWest->print(indent + 1);
    northEast->print(indent + 1);
    southWest->print(indent + 1);
    southEast->print(indent + 1);
  }

private:
  // bounding box representing the boundaries of this quadtree
  AABB boundary;

  // points in this tree
  // std::vector<Stopq> points;

  // children

  std::unique_ptr<QuadTree> northWest;
  std::unique_ptr<QuadTree> northEast;
  std::unique_ptr<QuadTree> southWest;
  std::unique_ptr<QuadTree> southEast;

  void subdivide() {

    double Quarter_halfDimension = boundary.halfDimension / 4;
    double Quarter_halfDimensionX = boundary.halfDimensionX / 2;
    double Quarter_halfDimensionY = boundary.halfDimensionY / 2;
    double x2 = boundary.centerX1; // change center.x
    double y2 = boundary.centerY1; // change center.y
    //// northWest = new QuadTree(AABB({x - (boundary.halfDimension / 2), y +
    ///(boundary.halfDimensionY / 2)}, Quarter_halfDimension));
    northWest = std::make_unique<QuadTree>(AABB(
        x2 - (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2),
        Quarter_halfDimensionX, Quarter_halfDimensionY));
    ////southWest = new QuadTree(AABB({x - (boundary.halfDimension / 2), y -
    ///(boundary.halfDimension / 2) }, Quarter_halfDimension ));
    southWest = std::make_unique<QuadTree>(AABB(
        x2 - (boundary.halfDimensionX / 2), y2 - (boundary.halfDimensionY / 2),
        Quarter_halfDimensionX, Quarter_halfDimensionY));
    //// northEast = new QuadTree(AABB({x + (boundary.halfDimension / 2), y +
    ///(boundary.halfDimension / 2) }, Quarter_halfDimension ));
    northEast = std::make_unique<QuadTree>(AABB(
        x2 + (boundary.halfDimensionX / 2), y2 + (boundary.halfDimensionY / 2),
        Quarter_halfDimensionX, Quarter_halfDimensionY));
    ////   southEast = new QuadTree(AABB({x + (boundary.halfDimension / 2), y -
    ///(boundary.halfDimension / 2) }, Quarter_halfDimension ));
    southEast = std::make_unique<QuadTree>(AABB(
        x2 + (boundary.halfDimensionX / 2), y2 - (boundary.halfDimensionY / 2),
        Quarter_halfDimensionX, Quarter_halfDimensionY));

    for (const auto &pointt : points) {
      northWest->insert(pointt);
      northEast->insert(pointt);
      southWest->insert(pointt);
      southEast->insert(pointt);
    }

    points.clear();
  }
};

template <typename nameMatch> // osm
void readd_osm_pbf(const std::string &filename, nameMatch &nameMatchin) {
  Parser<nameMatch> p(filename, nameMatchin);
  p.parse();
}

int main(int argc, char **argv) {
  if (argc != 3) { // osm and gtfs args
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt"
              << std::endl;

    return 1;
  }

  // AABB boundary1(-512, -512, 512, 512);
  // auto tree = QuadTree(boundary);
  // QuadTree tree1(boundary1);
  /*
std::vector<Stopq> points = {
  Stopq(2.0, 7.0),
  Stopq(3.0, 11.0),
  Stopq(2.0, 6.0),
  Stopq(1.0, 4.0),
  Stopq(6.0, 5.0),
  Stopq(7.0, 10.0)
};
  */
  std::vector<Stopq> pointsgtfs;

  double minLat = std::numeric_limits<double>::max();
  double maxLat = std::numeric_limits<double>::lowest();
  double minLon = std::numeric_limits<double>::max();
  double maxLon = std::numeric_limits<double>::lowest();

  // calculating centre and half-dimensions
  double halfDimensionX = (maxLon - minLon) / 2;
  double halfDimensionY = (maxLat - minLat) / 2;
  double centerX = halfDimensionX + minLon;
  double centerY = halfDimensionY + minLat;

  AABB boundary(centerX, centerY, halfDimensionX, halfDimensionY);
  QuadTree quadTree(boundary);

  for (const auto &point : quadTree.points) {

    if (!std::isnan(point.lat)) {

      minLat = std::min(minLat, point.lat);
      maxLat = std::max(maxLat, point.lat);
    }

    if (!std::isnan(point.lon)) {

      minLon = std::min(minLon, point.lon);
      maxLon = std::max(maxLon, point.lon);
    }

    // conversion from std::string to double
    if (!std::isnan(std::stod(point.gtfsstop1.stop_lat))) {

      minLat = std::min(minLat, std::stod(point.gtfsstop1.stop_lat));
      maxLat = std::max(maxLat, std::stod(point.gtfsstop1.stop_lat));
    }

    if (!std::isnan(std::stod(point.gtfsstop1.stop_lon))) {

      minLon = std::min(minLon, std::stod(point.gtfsstop1.stop_lon));
      maxLon = std::max(maxLon, std::stod(point.gtfsstop1.stop_lon));
    }

    // osm
    if (!std::isnan(point.osmcoordntaggs.lati)) {

      minLat = std::min(minLat, point.osmcoordntaggs.lati);
      maxLat = std::max(maxLat, point.osmcoordntaggs.lati);
    }

    if (!std::isnan(point.osmcoordntaggs.loni)) {

      minLon = std::min(minLon, point.osmcoordntaggs.loni);
      maxLon = std::max(maxLon, point.osmcoordntaggs.loni);
    }
  }

  /*
        //calculating centre and half-dimensions
        double halfDimensionX = (maxLon - minLon) / 2;
        double halfDimensionY = (maxLat - minLat) / 2;
        double centerX = halfDimensionX + minLon;
        double centerY = halfDimensionY + minLat;


        AABB boundary(centerX, centerY, halfDimensionX, halfDimensionY);
        QuadTree quadTree(boundary);
  */
  // AABB aabb1(centerX, centerY, halfDimensionX, halfDimensionY);

  // tree.print();

  std::cout << "inserting random points into tree !!" << std::endl;

  // TODO reinsert
  /// for (const auto& point : points) {
  ///   quadTree.insert(point);
  ///}

  readgtfs(stops, argv[2]); // gtfs reading
  nameMatch namematchin;    // osm

  // osm reading
  readd_osm_pbf(argv[1], namematchin);

  // Add (gtfs) stops to points
  for (const auto &gtfsstopstopq : stops) {

    Stopq stopqgts(0.0,           // Default latitude
                   0.0,           // Default longitude
                   0,             // Default ID
                   "",            // Default code
                   "",            // Default name
                   {},            // Default data22
                   gtfsstopstopq, // GTFS Stop data
                   {}             // Default osmcoordntag
    );
    quadTree.points.push_back(stopqgts);
    /// quadTree.insert(stopqgts);
  }

  // Add osmcoordtags to points
  for (const auto &osmcoordstopq : namematchin.vlopsecondd) {
    // Stopq stopqos;
    Stopq stopqos(0.0,          // Default latitude
                  0.0,          // Default longitude
                  0,            // Default ID
                  "",           // Default code
                  "",           // Default name
                  {},           // Default data22
                  {},           // Default Stop
                  osmcoordstopq // OSM coord data
    );
    quadTree.points.push_back(stopqos);

    /// quadTree.insert(stopqos);
  }
  /*
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
          ///quadTree.insert(stopqgts);


}

  */

  std::ofstream outputFile2("allpoints.txt");
  for (const auto &point : quadTree.points) {
    quadTree.insert(point);
    outputFile2 << "Stop Name: " << point.name << ", ID: " << point.id
                << ", Code: " << point.code << ", Latitude: " << point.lat
                << ", Longitude: " << point.lon
                << "  GTFS Stop - Lat: " << point.gtfsstop1.stop_lat
                << ", GTFS Stop Lon: " << point.gtfsstop1.stop_lon
                << ", GTFS Stop Name: " << point.gtfsstop1.stop_name
                << "  OSM Coord - Lat: " << point.osmcoordntaggs.lati
                << ", OSM Coord Lon: "
                << point.osmcoordntaggs.loni
                //<< ", OSM Name: " << point.osmcoordntaggs.Tagg
                << std::endl;
  }
  outputFile2.close();

  std::cout << "Number of points: " << quadTree.points.size() << '\n';

  /*
  std::cout << "Get points within bounds " << std::endl;
  auto pointInRange = tree.queryRange(AABB {Stop{ 4.0, 6.0}, 2.0 );
  for ( auto point : pointInRange ){
      std::cout << point  << std::endl;
  } */

  // Search by proximity

  Stop stop11(std::string("2"), std::string("A Ave & Chandler"),
              std::string("45.420609"), std::string("-122.675671"));
  Stopq stopInstance2(0.0, 0.0, 0, " ", " ", {}, stop11, {});

  double stopLatGt = std::stod(stop11.stop_lat); // Convert string to double
  double stopLonGt = std::stod(stop11.stop_lon); // Convert string to double

  auto results = quadTree.searchByProxim2(stopLatGt, stopLonGt, 30.0);
  // auto results = quadTree.searchByProximity(2.0, 2.0, 500000.0);

  std::cout << "Found " << results.size() << " points within the search radius."
            << std::endl;

  std::ofstream outputFile("proximityresults.txt");

  for (const auto &stopres : results) {
    //// std::cout << "Stop Name: " << stopres.name
    outputFile << "Stop Name: " << stopres.name << ", ID: " << stopres.id
               << ", Code: " << stopres.code << ", Latitude: " << stopres.lat
               << ", Longitude: " << stopres.lon
               << "  GTFS Stop - Lat: " << stopres.gtfsstop1.stop_lat
               << ", GTFS Stop Lon: " << stopres.gtfsstop1.stop_lon
               << ", GTFS STop Name: " << stopres.gtfsstop1.stop_name
               << "  OSM Coord - Lat: " << stopres.osmcoordntaggs.lati
               << ", OSM Coord Lon: "
               << stopres.osmcoordntaggs.loni
               //<< ", OSM Name: " << stopres.osmcoordntaggs.Tagg
               << std::endl;
  }

  outputFile.close();

  std::cout << "We read for namematchin" << namematchin.nodes << " nodes, "
            << namematchin.ways << " ways and " << namematchin.relations
            << " relations" << std::endl;

  return 0;
}