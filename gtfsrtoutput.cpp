#include "gtfs-realtime.pb.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//
using transit_realtime::FeedEntity;
using transit_realtime::FeedMessage;
using transit_realtime::TripDescriptor;
using transit_realtime::TripUpdate_StopTimeEvent;
using transit_realtime::TripUpdate_StopTimeUpdate;
using transit_realtime::TripUpdate_StopTimeUpdate_ScheduleRelationship;
//
int main() {

  // reinterpret_cast
  // CodedInputStream

  transit_realtime::FeedMessage msg;
  transit_realtime::FeedMessage msg1;
  transit_realtime::FeedMessage msg2;
  transit_realtime::FeedMessage msg3;

  transit_realtime::FeedHeader headr;
  transit_realtime::FeedEntity ent;
  transit_realtime::Alert alr;
  transit_realtime::VehiclePosition vehpos;

  ifstream file1("Alerts.pb", ios::in | ios::binary);
  string output;
  getline(file1, output);
  while (getline(file1, output)) {
    cout << output << endl;
  }
  // file1.close();
  // return 0;

  if (!msg.has_header()) {
    cout << "header isn't detected maybe" << endl;
    cout << msg.DebugString() << "debug string for msg " << endl;
    // cout << msg << endl;
  }

  if (!msg.ParseFromIstream(&file1)) {
    cout << "failed to parse" << endl; // or std::cerr << "Failed to parse
                                       // protobuf message" << std::endl;
    // return false; //seems unnecessary
  }

  if (msg.ParseFromIstream(&file1)) {
    cout << msg.entity_size() << "parsing message info"
         << endl; // or std::cerr << "Failed to parse protobuf message" <<
                  // std::endl;
    cout << msg.DebugString() << endl;
    cout << msg.entity_size() << endl;
    // return false; //seems unnecessary
  }

  cout << msg.DebugString() << "debug string on its own" << endl;
  cout << "debug string message stuff" << msg.DebugString();
  cout << msg.DebugString();

  string binn;
  msg2.SerializeToString(&binn);

  if (msg3.ParseFromString(binn)) {
    cout << msg.entity_size() << "parsing message string entity size"
         << endl; // or std::cerr << "Failed to parse protobuf message" <<
                  // std::endl;
    cout << msg.DebugString() << endl;
    // return false; //seems unnecessary
  }

  cout << msg3.ParseFromString(binn) << "trying to parse from string" << endl;
  cout << msg3.entity_size() << "msg entity size" << endl;
  cout << msg3.DebugString() << "msg debug" << endl;
  cout << alr.has_effect() << "alert has effect stuff" << endl;
  cout << alr.has_cause() << "alert has cause stuff" << endl;
}