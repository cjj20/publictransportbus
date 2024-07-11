Public Transport Bus

This repository attempts to compare gtfs data to osm data and is still under development. So far the gtfs code and osm code are in separate files. In the gtfs file 'readingcsvcolumn.cpp' the gtfs ids are displayed and a specific gtfs id is found from those gtfs ids. In the osm file, libosmpbfreader/gettingdata.cc the data is sorted according to highway=bus_stop and can also be sorted according to 'name' tags as well as 'ref' tags.

The program requests the reader to enter a ref value for which is gives the number of nodes for that value as well as it's latitude and longitude. As of now the code works best with the specific stops.txt file and isle-os-man-latest.osm.pbf files included in the repository, thus changes have to be made for flexibility.

To obtain matched stops, osm stops and gtfs stops: g++ -o getdata listgtfs2.cpp gettingosmdata.cc

Note: Need git submodules in order to run the code. Instructions: 
git submodule add  https://github.com/openstreetmap/OSM-binary OSM-binary
git submodule update --init --recursive

git submodule add https://github.com/osmcode/libosmium libosmium
git submodule update --init --recursive

git submodule add https://github.com/hove-io/libosmpbfreader libosmpbfreader
git submodule update --init --recursive

git submodule add https://github.com/mapbox/protozero protozero
git submodule update --init --recursive


To run the code files these are the following steps: for 'readingcolumn.csv.cpp' enter: sudo clang++ readingcsvcolumn.cpp gtfs-realtime.pb.cc -o gtfsplain3.out pkg-config --cflags --libs protobuf ./gtfsplain3.out

for 'gettingdata.cc' enter: g++ -O3 -std=c++0x -Wall -Wextra -o getting_data gettingdata.cc -lprotobuf-lite -losmpbf -lz ./getting_data isle-of-man-latest.osm.pbf
