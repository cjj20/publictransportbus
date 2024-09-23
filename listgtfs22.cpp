//#include "functionsExample.h"
#include "readgt.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;



void readgtfs(list<Stop> &stops, std::string filename) {
  ifstream fileRead(filename);
  string line;
  int rowCounter = 0;
  
  int stopIdNum = -1;
 
  int stopLatNum = -1;
  int stopLonNum = -1;

  int stopNamNum = -1;

  while (getline(fileRead, line)) {
    rowCounter++;
  }

  

  vector<string> data[rowCounter];
  

  fileRead.clear();             
  fileRead.seekg(fileRead.beg); 

  while (getline(fileRead, line)) {
    stringstream ss(line); 
    vector<string> columns;
    string columnvalue;
    while (getline(
        ss, columnvalue,
        ',')) { 
      columns.push_back(columnvalue);
    }
    
    Stop stop;
    vector<string> &headers = columns; 
    auto it = std::find(headers.begin(), headers.end(), "stop_id");
    if (it != headers.end()) {
      stopIdNum = std::distance(headers.begin(), it);
    }

    it = std::find(headers.begin(), headers.end(), "stop_name");
    if (it != headers.end()) {
      stopNamNum = std::distance(headers.begin(), it);
    }

    it = std::find(headers.begin(), headers.end(), "stop_lat");
    if (it != headers.end()) {
      stopLatNum = std::distance(headers.begin(), it);
    }
    it = std::find(headers.begin(), headers.end(), "stop_lon");
    if (it != headers.end()) {
      stopLonNum = std::distance(headers.begin(), it);
    }

    stop.stop_id = columns[stopIdNum];
    stop.stop_lat = columns[stopLatNum];
    stop.stop_lon = columns[stopLonNum];
    stop.stop_name = columns[stopNamNum];
    

    stops.push_back(stop);
  }
 
  fileRead.close();


}