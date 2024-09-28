# Public Transport Bus

This repository compares gtfs data to osm data and is still under development. So far There are various files which can be run to obtain stops and details. Some of the  gtfs code and osm code are in separate files. While others are integrated. 
 

The code enables the user to match by coordinates, ref/ID, name, distance (both haversine and vincenty inverse algorithms utilised), also there is the availability of matching by distance using a quadtree. Matches as well as non-matches can be displayed some are saved to csv files. More information on the components and how they were deployed can be found in the public gist for this project. 

#### Gist containing detailed summary of how the project works:
https://gist.github.com/cjj20/3cf2a9fcb8333d2d11ab5b2710dc9b3b


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
1. In libosmpbfreader folder enter: ``make``
2. In OSM-binary folder: ``mkdir build && cd build``  then: ``cmake ..``   then: ``make`` finally enter : ``sudo make install``
3. In publictransportbus folder enter: 
    ```
    sudo g++ -O2 -o getda gettingosmdatacopy.cc listgtfs22.cpp -lprotobuf-lite -losmpbf -lz
    ./getdata isle-of-man-latest.osm.pbf stops.txt
    ```
4. For running the program to generate csv files contaning matches and non-matches based on distance: 
   ```
   sudo g++ -O2 -o getcsvdata getting_osm_gtfs_data_csv.cc listgtfs22.cpp vincentyinversecopyy.cpp -losmpbf -lprotobuf -lz
   ./getcsvdata isle-of-man-latest.osm.pbf stops.txt
   ```
5. For running the program to obtain stops and to match based on distance away from each other using the vincenty inverse algorithm. That is (only those < 30m were matched) :
   ```
    sudo g++ -O2 -o getda2 gettingosmdata6.cc listgtfs22.cpp vincentyinversecopyy.cpp -losmpbf -lprotobuf -lz
   ./getda2 isle-of-man-latest.osm.pbf stops.txt
   ```
6. This is for matching gtfs to osm stops by name:
    ```
    sudo g++ -O2 -o nammatch10 matchingbynam.cc listgtfs22.cpp vincentyinversecopyy.cpp -losmpbf -lprotobuf -lz
    ./nammatch10 oregon-latest.osm.pbf oregon-stops.txt
    ```
7. These commands are for use with a standard quadtree:
   ```
   sudo g++ -O2 -o quad1 quadtree3.cpp listgtfs22.cpp -losmpbf -lprotobuf -lz
   ./quad1 oregon-latest.osm.pbf oregon-stops.txt
   ```
8. These set of commands are to be used with the modified quadtree which stores both gtfs and osm data and attempts to match by proximity. There is allowance to also use a third set of coordinates:
     ```
     sudo g++ -O2 -o quadmodif29 quadtree4Modified.cpp listgtfs22copy.cpp -losmpbf -lprotobuf -lz
     ./quadmodif29 oregon-latest.osm.pbf oregon-stops.txt
     ```
9.  This command is for matching stops by ref: 
     ```
     sudo g++ -O2 -o getda matchingbyref.cc listgtfs22copy.cpp -lprotobuf-lite -losmpbf -lz
     ./getda oregon-latest.osm.pbf oregon-stops.txt
	 
	 sudo g++ -O2 -o getda matchingbyref.cc listgtfs22.cpp -lprotobuf-lite -losmpbf -lz
     ./getda oregon-latest.osm.pbf oregon-stops.txt
	 
     ```

10. Commands for the Hidden Markov Model Forward Algorithm: 
     ```
     sudo g++ -o forwardhmm mainhmm.cpp hmm.cpp
     ./forwardhmm
     ```
11. Cmake is also available and was used to compact commands.

What the above command should output:

<img width="500dp" src=".github/comparison - distance for coordinates.jpg">

In the gtfs file 'readingcsvcolumn.cpp' the gtfs ids are displayed and a specific gtfs id is found from those gtfs ids. In the osm file, libosmpbfreader/gettingdata.cc the data is sorted according to highway=bus_stop and can also be sorted according to 'name' tags as well as 'ref' tags.
The program requests the reader to enter a ref value for which is gives the number of nodes for that value as well as it's latitude and longitude.

Here are the following steps for other code files: 
- for 'readingcolumn.csv.cpp' enter: ``sudo clang++ readingcsvcolumn.cpp gtfs-realtime.pb.cc -o gtfsplain3.out pkg-config --cflags --libs protobuf`` then ``./gtfsplain3.out``

- for 'gettingdata.cc' enter: ``g++ -O3 -std=c++0x -Wall -Wextra -o getting_data gettingdata.cc -lprotobuf-lite -losmpbf -lz`` then ``./getting_data isle-of-man-latest.osm.pbf stops.txt``

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
