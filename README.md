# Public Transport Bus

This repository compares gtfs data to osm data and is still under development. So far There are various files which can be run to obtain stops and details. Some of the  gtfs code and osm code are in separate files. While others are integrated. 
 

As of now the code works best with the specific stops.txt file (which was amended for the addition of osm data for comparison reasons, Isle of Man gtfs where difficult to obtain) and isle-os-man-latest.osm.pbf files included in the repository, thus changes have to be made for flexibility.


## Installing

Dependencies: see Dockerfile or .github/workflows/build-and-run.yml

Note: the -lprotobuf-lite tag is related to OSM-binary which libosmpbfreader is dependent on. But if prompted to install protobuf may have to do so

Note: Need git submodules in order to run the code. Instructions: 

```
git submodule add https://github.com/openstreetmap/OSM-binary OSM-binary
git submodule add https://github.com/osmcode/libosmium libosmium
git submodule add https://github.com/hove-io/libosmpbfreader libosmpbfreader
git submodule add https://github.com/mapbox/protozero protozero

git submodule update --init --recursive
```

## Running

To obtain matched stops, osm bus stop tagged stops and gtfs stops:
1. In libosmpbfreader folder enter: make
2. In OSM-binary folder: mkdir build && cd build  then: cmake ..   then: make  finally enter : sudo make install
3. In publictransportbus folder enter: g++ -O3 -std=c++0x -Wall -Wextra -o getdata listgtfs2.cpp gettingosmdatacopy.cc -lprotobuf-lite -losmpbf -lz
4. ./getdata isle-of-man-latest.osm.pbf stops.txt
5.For running the program to generate csv files contaning matches and non-matches based on distance: g++ -O3 -std=c++0x -Wall -Wextra -o getcsvdata getting_osm_gtfs_data_csv.cc listgtfs2.cpp vincentyinversecopyy.cpp  -lprotobuf-lite -losmpbf -lz     ./getcsvdata isle-of-man-latest.osm.pbf stops.txt
6. For running the program to obtain stops and to match based on distance away from each other using the vincenty inverse algorithm. That is (only those < 30m were matched) :
7. g++ -O3 -std=c++0x -Wall -Wextra -o getda gettingosmdata6.cc listgtfs2.cpp vincentyinversecopyy.cpp  -lprotobuf-lite -losmpbf -lz
8. ./getda isle-of-man-latest.osm.pbf stops.txt
9. This is for matching gtfs to osm stops by name:
    sudo g++ -O2 -o nammatch119 matchingbynam.cc listgtfs22.cpp -losmpbf -lprotobuf -lz
   ./nammatch119 oregon-latest.osm.pbf oregon-stops.txt

10. These commands are for use with a standard quadtree:
     sudo g++ -O2 -o quad1 quadtree3.cpp listgtfs22.cpp -losmpbf -lprotobuf -lz
     ./quad1 oregon-latest.osm.pbf oregon-stops.txt

11. These set of commands are to be used with the modified quadtree which stores both gtfs and osm data and attempts to match by proximity. There is allowance to also use a third set of coordinates:
     sudo g++ -O2 -o quad2 quadtree3Modified.cpp listgtfs22copy.cpp -losmpbf -lprotobuf -lz
     ./quad2 oregon-latest.osm.pbf oregon-stops.txt


Here is a link to a screenshot of what the above command should output: https://drive.google.com/file/d/1y1KiLo2QLio9MkR2HgkYL2tvdRI4r3vi/view?usp=drivesdk

In the gtfs file 'readingcsvcolumn.cpp' the gtfs ids are displayed and a specific gtfs id is found from those gtfs ids. In the osm file, libosmpbfreader/gettingdata.cc the data is sorted according to highway=bus_stop and can also be sorted according to 'name' tags as well as 'ref' tags.
The program requests the reader to enter a ref value for which is gives the number of nodes for that value as well as it's latitude and longitude.

Here are the following steps for other code files: for 'readingcolumn.csv.cpp' enter: sudo clang++ readingcsvcolumn.cpp gtfs-realtime.pb.cc -o gtfsplain3.out pkg-config --cflags --libs protobuf ./gtfsplain3.out

for 'gettingdata.cc' enter: g++ -O3 -std=c++0x -Wall -Wextra -o getting_data gettingdata.cc -lprotobuf-lite -losmpbf -lz ./getting_data isle-of-man-latest.osm.pbf stops.txt

## Downloading your own data

1. Download a PBF file from here (smaller is easier): http://download.geofabrik.de/
   Save to a convenient place like this folder (just don't check it into git!)
   For example, `oregon-latest.osm.pbf` from http://download.geofabrik.de/north-america/us/oregon.html
2. Download a GTFS ZIP file for the same region from your preferred site, like http://www.oregon-gtfs.com/
   For example, http://developer.trimet.org/schedule/gtfs.zip has stops.txt in it; extract to `oregon-stops.txt`
3. Use these file names when running the programs instead of the default osm.pbf and stops.txt

## Running via Docker

```
docker build . -t publictransportbus

# To run the CSV program against the default files (isle-of-man-latest.osm.pbf and stops.txt)
docker run -it publictransportbus

# To get an interactive bash prompt (with the current folder mounted as /context):
docker run -v .:/context -it publictransportbus /bin/bash

# And then running:
./getcsvdata /context/oregon-latest.osm.pbf /context/oregon-stops.txt

# Results will be in stop*30.csv; copy them to /context to save locally
```
