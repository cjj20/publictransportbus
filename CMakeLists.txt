cmake_minimum_required(VERSION 3.10)

# Project name
project(OsmGtfsMather)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectory for libosmpbfreader
if(EXISTS "${CMAKE_SOURCE_DIR}/libosmpbfreader/CMakeLists.txt")
    # Set CMake module path to find custom modules
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/libosmpbfreader/cmake/modules")

    add_subdirectory(libosmpbfreader)
else()
    message(FATAL_ERROR "libosmpbfreader directory not found did you forget to add git submodule?")
endif()


# Find osmpbf package
find_package(osmpbf REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/libosmpbfreader/include)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/libosmpbfreader/build)

# Executable for getdata
add_executable(getdata listgtfs2.cpp gettingosmdatacopy.cc)
target_link_libraries(getdata protobuf-lite osmpbf z)

# Executable for getcsvdata
add_executable(getcsvdata getting_osm_gtfs_data_csv.cc listgtfs2.cpp vincentyinversecopyy.cpp)
target_link_libraries(getcsvdata protobuf-lite osmpbf z)

# Executable for getda
add_executable(getda gettingosmdata6.cc listgtfs2.cpp vincentyinversecopyy.cpp)
target_link_libraries(getda protobuf-lite osmpbf z)

# Executable for QTreeMatch
add_executable(q3match quadtree3Modified.cpp listgtfs22copy.cpp)
target_link_libraries(q3match protobuf-lite osmpbf z)

# Install targets
install(TARGETS getdata getcsvdata getda q3match
        RUNTIME DESTINATION bin)