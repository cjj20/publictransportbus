// quadtree
// command 1:
// sudo g++ -O2 -o quad1 quadtree3.cpp listgtfs22.cpp -losmpbf -lprotobuf -lz
// command 2:
//./quad1 oregon-latest.osm.pbf oregon-stops.txt
// define node struct, build tree given N points, point search, range search
// (find all points within specified bounding rectangle

// calculate root point if it is the first division
// iterate i from l to nDatapoints
// compare x, y coordinates of each point with root coordinates:
//  if root(x) > point (x) and root(y) >= point(y)
//      assign the data to UL quarter
// if root(x) < point (x) and root(y) <= point(y)
//      assign the data point to UR quarter
// if root(x) >= point(x) and root(y) < point(y)
//   assign the data point to LL quarter
// if root(x) <= point(x) and root(y) > point(y)
//    assign the data point to LR quarter

// output four quadrants with assigned data points

#include "libosmpbfreader/osmpbfreader.h"
#include "readgt.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

using namespace osmpbfreader;

// TODO include header files for reading gtfs and reading osm
const int NODE_CAPACITY = 4;

std::list<Stop> stops;

/* TODO
struct osmStop {

        name
        std::map<std::string, std::string> Tagg; //tags
        lat
        lon
};
*/
struct osmcoordntag {
  std::map<std::string, std::string> Tagg; // TODO change type
  // TODO remove or modify if needed std::map<std::string, std::string,
  // std::less<>> Tagg;
  double lati;
  double loni;

  osmcoordntag(double lati, double loni,
               const std::map<std::string, std::string> &tags)
      : lati(lati), loni(loni), Tagg(tags) {}
};

struct nameMatch { // TODO osm stuff
  // Three integers count how many times each object type occurs
  int nodes;
  int ways;
  int relations;

  std::vector<Tags> tagss;
  std::vector<osmcoordntag> vlopsecondd;

  // std::vector<tagz> taglisty;

  nameMatch() : nodes(0), ways(0), relations(0) {}

  // This method is called every time a Node is read
  void node_callback(uint64_t /*osmid*/, double lon, double lat,
                     const Tags &tags) {
    //++nodes;
    // std::string xtt; //= gtfs.stop_name;
    auto nam = tags.find("name");
    auto highw = tags.find("highway");
    //// if(highw != tags.end() && highw->second == "bus_stop" && nam !=
    ///tags.end() && nam->second==xtt) {
    if (highw != tags.end() && highw->second == "bus_stop") {
      ++nodes;
      tagss.push_back(tags);
      vlopsecondd.push_back(
          {lat, lon, tags}); // TODO may need to remove or change this line
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
  double lat; // float lat;
  double lon; // float lon;
  int id;
  std::string code;
  std::string name;

  Stopq(double lat, double lon, int id = 0, const std::string &code = "",
        const std::string &name = "")
      : lat(lat), lon(lon), id(id), code(code), name(name) {}
  Stopq() = default;
};

struct XY {
  float x;
  float y;

  XY() = default;
  XY(float _x, float _y) : x(_x), y(_y) {}
};

struct Point {
  float x, y;
  Point(float x = 0, float y = 0) : x(x), y(y){};
};

// Axis-aligned bounding box with half dimension and center

struct AABB {
  ////XY centre;
  Stopq center; // to be changed
  double centerX1;
  double centerY1;
  double halfDimension;
  double halfDimensionX; //
  double halfDimensionY;

  AABB(double x, double y, double dx, double dy)
      : centerX1(x), centerY1(y), halfDimensionX(dx), halfDimensionY(dy) {}

  bool containsPoint(const Stopq &stop) const {

    if (stop.lon > (centerX1 + halfDimensionX))
      return false;

    if (stop.lon < (centerX1 - halfDimensionX))
      return false;

    if (stop.lat > (centerY1 + halfDimensionY))
      return false;

    if (stop.lat < (centerY1 - halfDimensionY))
      return false;

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

  std::vector<Stopq> queryRange(
      const AABB &range) const { // stop should be used here in addition to AABB

    // prepare array/list of results
    auto pointsInRange = std::vector<Stopq>();
    std::vector<Stopq> pointsInRange2;

    // Automatically abort if the range does not intersect this quad
    if (!boundary.intersects(range)) {
      return pointsInRange; // empty list
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

  void print(int indent = 0) {
    if (indent == 0)
      std::cout << "==== Printing QuadTree ====" << std::endl;

    for (int i = 0; i < indent; i++)
      std::cout << "*";

    std::cout << "* " << boundary.centerX1 << boundary.centerY1
              << boundary.halfDimensionX << boundary.halfDimensionY
              << std::endl;

    // list points

    for (auto point : points) {

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
  std::vector<Stopq> points;

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

template <typename nameMatch>
void readd_osm_pbf(const std::string &filename, nameMatch &nameMatchin) {
  Parser<nameMatch> p(filename, nameMatchin);
  p.parse();
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " file_to_read.osm.pbf stops.txt"
              << std::endl;
    /// readgtfs(stops, argv[2]);

    return 1;
  }

  /*Stop stopp(4.0, 8.0);
      double four = 4;
      double eight = 8;
  //auto boundary = AABB( Stop(4.0, 8.0), 4.0); //edit
      auto boundary = AABB(4.0, 8.0, 4.0, 4.0); //edit
      AABB boundary1(-512, -512, 512, 512);
  auto tree = QuadTree(boundary);
      QuadTree tree1(boundary1);
      */
  std::vector<Stopq> points = {Stopq(2.0, 7.0), Stopq(3.0, 11.0),
                               Stopq(2.0, 6.0), Stopq(1.0, 4.0),
                               Stopq(6.0, 5.0), Stopq(7.0, 10.0)};

  double minLat = std::numeric_limits<double>::max();
  double maxLat = std::numeric_limits<double>::lowest();
  double minLon = std::numeric_limits<double>::max();
  double maxLon = std::numeric_limits<double>::lowest();

  for (const auto &point : points) {
    minLat = std::min(minLat, point.lat);
    maxLat = std::max(maxLat, point.lat);
    minLon = std::min(minLon, point.lon);
    maxLon = std::max(maxLon, point.lon);
  }

  // calculating centre and half-dimensions
  double halfDimensionX = (maxLon - minLon) / 2;
  double halfDimensionY = (maxLat - minLat) / 2;
  double centerX = halfDimensionX + minLon;
  double centerY = halfDimensionY + minLat;

  AABB boundary(centerX, centerY, halfDimensionX, halfDimensionY);
  QuadTree quadTree(boundary);

  std::cout << "inserting random points into tree !!" << std::endl;

  for (const auto &point : points) {
    quadTree.insert(point);
  }

  // Search by proximity
  auto results = quadTree.searchByProximity(2.0, 2.0, 500000.0);
  std::cout << "Found " << results.size() << " points within the search radius."
            << std::endl;

  for (const auto &stopres : results) {
    std::cout << "Stop Name: " << stopres.name << ", ID: " << stopres.id
              << ", Code: " << stopres.code << ", Latitude: " << stopres.lat
              << ", Longitude: "
              << stopres.lon
              //<< ", Latitude: " << stop.lat
              << std::endl;
  }

  readgtfs(stops, argv[2]); // gtfs
  nameMatch namematchin;    // osm

  // osm
  readd_osm_pbf(argv[1], namematchin);
  std::cout << "We read for namematchin" << namematchin.nodes << " nodes, "
            << namematchin.ways << " ways and " << namematchin.relations
            << " relations" << std::endl;

  return 0;
}