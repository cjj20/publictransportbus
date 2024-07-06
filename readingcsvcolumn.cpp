#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>  
using namespace std;
int main(int argc, char** argv)
{
   ifstream fileRead("stops.txt");
   string line;
   int rowCounter=0;
   int rowIdx=0; 

   
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
      string columnvalue;
      while(getline(ss,columnvalue,',')){       //for every value in that stream (ie: every cell on that row)
         data[rowIdx].push_back(columnvalue);
      }
      rowIdx++;   //increment row number
   }
//}
   fileRead.close();


   

   int colNum=0;  //want to access stop_id which is at column 0

   for(int row=0; row<rowCounter; row++)
   {
      cout << data[row][colNum] << "\t";  //print every value in column 0
      if(data[row][colNum] == "istb.191665") { //this is one of the stop_ids taken from a random country/state's stops.txt file
        cout << "found value" << data[row][colNum] << endl;
      }
   }
   cout << endl;


   return 0;
}