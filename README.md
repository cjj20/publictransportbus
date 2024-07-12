Public Transport Bus

This repository compares gtfs data to osm data and is still under development. So far There are various files which can be run to obtain stops and details. Some of the  gtfs code and osm code are in separate files. While others are integrated. 
 

As of now the code works best with the specific stops.txt file (which was amended for the addition of osm data for comparison reasons, Isle of Man gtfs where difficult to obtain) and isle-os-man-latest.osm.pbf files included in the repository, thus changes have to be made for flexibility.

To obtain matched stops, osm bus stop tagged stops and gtfs stops:  
1. sudo apt-get install libosmpbf-dev
2. In libosmpbfreader folder enter: make
3. In OSM-binary folder: mkdir build && cd build  then: cmake ..   then: make  finally enter : sudo make install
4. In publictransportbus folder enter: g++ -O3 -std=c++0x -Wall -Wextra -o getdata listgtfs2.cpp gettingosmdatacopy.cc -lprotobuf-lite -losmpbf -lz
5. ./getdata isle-of-man-latest.osm.pbf
6. For running the program to obtain stops and to match based on distance away from each other using the vincenty inverse algorithm. That is (only those < 30m were matched) :
7. g++ -O3 -std=c++0x -Wall -Wextra -o getda gettingosmdata6.cc listgtfs2.cpp vincentyinversecopyy.cpp  -lprotobuf-lite -losmpbf -lz
8. ./getda isle-of-man-latest.osm.pbf


Here is a link to a screenshot of what the above command should output: https://drive.google.com/file/d/1y1KiLo2QLio9MkR2HgkYL2tvdRI4r3vi/view?usp=drivesdk

   
Note: the -lprotobuf-lite tag is related to OSM-binary which libosmpbfreader is dependent on. But if prompted to install protobuf may have to do so

Note: Need git submodules in order to run the code. Instructions: 
git submodule add  https://github.com/openstreetmap/OSM-binary OSM-binary
git submodule update --init --recursive

git submodule add https://github.com/osmcode/libosmium libosmium
git submodule update --init --recursive

git submodule add https://github.com/hove-io/libosmpbfreader libosmpbfreader
git submodule update --init --recursive

git submodule add https://github.com/mapbox/protozero protozero
git submodule update --init --recursive

In the gtfs file 'readingcsvcolumn.cpp' the gtfs ids are displayed and a specific gtfs id is found from those gtfs ids. In the osm file, libosmpbfreader/gettingdata.cc the data is sorted according to highway=bus_stop and can also be sorted according to 'name' tags as well as 'ref' tags.
The program requests the reader to enter a ref value for which is gives the number of nodes for that value as well as it's latitude and longitude.

Here are the following steps for other code files: for 'readingcolumn.csv.cpp' enter: sudo clang++ readingcsvcolumn.cpp gtfs-realtime.pb.cc -o gtfsplain3.out pkg-config --cflags --libs protobuf ./gtfsplain3.out

for 'gettingdata.cc' enter: g++ -O3 -std=c++0x -Wall -Wextra -o getting_data gettingdata.cc -lprotobuf-lite -losmpbf -lz ./getting_data isle-of-man-latest.osm.pbf
