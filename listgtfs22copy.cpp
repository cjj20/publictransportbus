//#include "functionsExample.h"
#include "readgtcopy.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <iomanip>
#include <list> 
using namespace std;



///struct Stop {
 ///    string stop_id;
///     string stop_name;
///     string stop_lat;
///     string stop_lon;


///};
//auto readd = readgtfs();
//list<pair<int, string>> mylist = readd;
//list<pair<int, string>> mylit = readgtfs();
//list<string> mylit = readgtfs();

void readgtfs(list<Stop>& stops, std::string filename) {
    ifstream fileRead(filename);
   string line;
   int rowCounter=0;
   //int rowIdx=0; 
   //int row;
     int stopIdNum = -1;
     //int stopNamNum;
     int stopLatNum = -1;
     int stopLonNum = -1;
	 
	 int stopNamNum = -1;
   
   while(getline(fileRead,line)){
      rowCounter++;
   }

   //table, a row is data[row_number].
   
   vector<string> data[rowCounter];
   //vector<vector<string>> data[rowCounter];

   fileRead.clear(); //remove failbit (ie: continue using fileRead)
   fileRead.seekg(fileRead.beg); //rewind stream to start

   while(getline(fileRead,line)) 
   {
      stringstream ss(line);  //copy line to stringstream
      vector<string> columns;
      string columnvalue;
	  bool bInsideQuotes = false;
	  
	  for(char c : line) {
		if(c == '"') { 
            bInsideQuotes = !bInsideQuotes;
		} else if(c == ',' && !bInsideQuotes) {
            columns.push_back(columnvalue);
			columnvalue.clear();


		} else {
             columnvalue += c;         

		}			  
		  
		  
		  
	  }
	  
	  columns.push_back(columnvalue);
	  /*
      while(getline(ss,columnvalue,',')){       //for every value in that stream (ie: every cell on that row)
        ///// data[rowIdx].push_back(columnvalue);
	
	          
              columns.push_back(columnvalue);
      }
    /////  rowIdx++;   //increment row number

      */

    
    


    //int row;
     ////int stopIdNum;
     //int stopNamNum;
     /////int stopLatNum;
    ///// int stopLonNum;
    Stop stop;
     vector<string>& headers = columns; //[0];
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
    //////stop.stop_id = columns[0];
   /////// stop.stop_lat = columns[3];
   /////// stop.stop_lon = columns[2];









    stops.push_back(stop);


   }
//}
   fileRead.close();

   

  ///// for(int row=0; row<rowCounter; row++)
  ///// {
   /////  stop.stop_id = data[row][stopIdNum];
   /////  stop.stop_lat = data[row][stopLatNum];
  /////   stop.stop_lon = data[row][stopLonNum];
  ///////    cout << data[row][stopIdNum] << data[row][stopNamNum] << data[row][stopLonNum] <<  data[row][stopLatNum] << "\t";  
     // if(data[row][colNum] == "istb.191665") { //this is one of the stop_ids taken from a random country/state's stops.txt file
      //  cout << "found value" << data[row][colNum] << endl;
      //}
  ///// }
  // cout << endl;
  
  


}