#include "gtfs-realtime.pb.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
  // ifstream file;

  // file.open("stops.txt");
  ifstream file1("stops.txt");
  string output;
  getline(file1, output);
  while (getline(file1, output)) {
    cout << output << endl;
  }
  file1.close();
  return 0;
}