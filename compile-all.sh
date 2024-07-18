#!/bin/bash
set -e # stop on error

cd libosmpbfreader
ls -al
echo "first make"
make

echo "cmake"
mkdir build
cd build
cmake ..

echo "second make"
make

# Needs sudo when run as non-root
if [ `id -u` -ne 0 ]
  then sudo make install
  else make install
fi


cd ../..
echo "getdata"
g++ -O3 -std=c++0x -Wall -Wextra -o getdata listgtfs2.cpp gettingosmdatacopy.cc -lprotobuf-lite -losmpbf -lz
echo "getcsvdata"
g++ -O3 -std=c++0x -Wall -Wextra -o getcsvdata getting_osm_gtfs_data_csv.cc listgtfs2.cpp vincentyinversecopyy.cpp -lprotobuf-lite -losmpbf -lz
echo "getda"
g++ -O3 -std=c++0x -Wall -Wextra -o getda gettingosmdata6.cc listgtfs2.cpp vincentyinversecopyy.cpp -lprotobuf-lite -losmpbf -lz
#ls && clang++ readingcsvcolumn.cpp gtfs-realtime.pb.cc -o gtfsplain3.out pkg-config --cflags --libs protobuf ./gtfsplain3.out
#ls && g++ -O3 -std=c++0x -Wall -Wextra -o getting_data gettingdata.cc -lprotobuf-lite -losmpbf -lz ./getting_data isle-of-man-latest.osm.pbf
ls -alth
echo "done"