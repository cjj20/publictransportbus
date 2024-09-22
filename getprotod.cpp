
#include <iostream>
//#include "google/protobuf/io/zero_copy_streamimpl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"
#include "gtfs-realtime.pb.h"
#include <fstream>
#include <string>
//#include <windows.h>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/wire_format_lite.h"
#include <sstream>
using namespace std;
using transit_realtime::FeedEntity;
using transit_realtime::FeedMessage;
using transit_realtime::TripDescriptor;
using transit_realtime::TripUpdate_StopTimeEvent;
using transit_realtime::TripUpdate_StopTimeUpdate;
using transit_realtime::TripUpdate_StopTimeUpdate_ScheduleRelationship;

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  // google::protobuf::Message* msg;

  transit_realtime::FeedMessage msg;
  transit_realtime::FeedMessage msg2;
  transit_realtime::FeedMessage msg3;

  ifstream file1("Alerts.pb", ios::in | ios::binary); // open

  float fileContent[20];
  for (int i = 0; i < 20; i++)
    file1.read((char *)&fileContent[i], sizeof(float));

  // ifstream file1("TripUpdates",ios::in | ios::binary); //open
  transit_realtime::FeedHeader headr;
  transit_realtime::FeedEntity ent;
  transit_realtime::Alert alr;
  transit_realtime::VehiclePosition vehpos;
  // FeedEntity enn;

  // FeedMessage msg;
  // FeedMessage() &msg;
  // msg::has_header() //FeedMessage::has_header()//
  // FeedMessage.entity

  if (!msg.has_header()) {
    cout << "header isn't detected maybe" << endl;
    cout << msg.DebugString() << "debug string for msg " << endl;
    // cout << msg << endl;
  }
  // msg.header = NULL;

  // for header in msg.Header
  // transit_realtime::FeedMessage() mss;
  if (file1) {
    cout << "file found" << endl;
  }

  if (!file1) {
    cout << "file not found" << endl;
  }

  // if (!file1.is_open()) return false;

  // cout << msg.header << endl;

  msg.ParseFromIstream(&file1); // parse file
  // headr.ParseFromIstream(&file1);// trying to parse header
  ent.ParseFromIstream(&file1);    // trying to parse feed content
  alr.ParseFromIstream(&file1);    // parsing alerts stuff
  vehpos.ParseFromIstream(&file1); // vehicleposition parsing

  // file1.close(); //may come after cout stuff, may not be necessary

  // cout << msg.getTripUpdates() << endl; //doublecheck this

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
    cout << msg.entity_size() << "parsing message string"
         << endl; // or std::cerr << "Failed to parse protobuf message" <<
                  // std::endl;
    cout << msg.DebugString() << endl;
    // return false; //seems unnecessary
  }

  cout << msg3.ParseFromString(binn) << "trying to parse from string" << endl;

  if (alr.ParseFromIstream(&file1)) {
    cout << "parsing alerts" << endl;
    cout << ent.has_alert() << endl;
    // cout <<  ent.alert()   << endl;
    // cout <<  ent.stop()   << endl;
    // cout <<  transit_realtime::FeedMessage::_internal_entity_size()   <<
    // endl;
    cout << ent.has_alert() << endl;
    // cout <<  ent.alert()   << endl;
    // cout <<  ent.alert   << endl;
    // cout <<  transit_realtime.Alert._internal_has_effect()   << endl;
    // cout <<  transit_realtime.Alert.has_effect()   << endl;
    // cout <<  transit_realtime.Alert.has_cause()   << endl;
    // cout <<  transit_realtime.Alert.SeverityLevel.has_severity_level()   <<
    // endl; cout <<  transit_realtime.Alert.has_severity_level()   << endl;
    // cout <<  ent::_internal_has_alert()   << endl;
    //  cout <<  ent.alert   << endl;
    // cout <<  _internal_has_effect()   << endl;
    cout << alr.has_effect() << endl;
    cout << alr.has_cause() << endl;
    // cout <<  alr.SeverityLevel.has_severity_level()   << endl;
    cout << alr.has_severity_level() << endl;
    // cout <<  ent::has_alert()   << endl;
    // cout <<  ent::_internal_has_alert()   << endl;
    // cout <<  ent::internal_has_trip_update()   << endl;
    cout << msg.entity_size() << endl;
  }

  if (!alr.ParseFromIstream(&file1)) {
    cout << "can't parse alert info" << endl;
  }

  if (vehpos.ParseFromIstream(&file1)) {
    cout << "parsing vehicle positions stuff" << endl;
  }

  if (!vehpos.ParseFromIstream(&file1)) {
    cout << "can't parse vehiclepositions" << endl;
  }

  if (ent.ParseFromIstream(&file1)) {
    cout << "feed enity can be parsed" << endl;
  }

  if (!ent.ParseFromIstream(&file1)) {
    cout << "can't parse feedentity info" << endl;
  }

  cout << "Parsed GTFS Realtime message:" << endl;
  cout << msg.DebugString() << "trying to debug string msg" << endl;

  file1.close();
  // google::protobuf::ShutdownProtobufLibrary();
  // return 0;

  // can use if function after ParseFromIstream
  // if(person.IsInitialized()) {
  // cout << "Name" << person.name() << endl;
  //   if(person.has_email()) {
  //  cout << "email" << person.email() << endl;
  //}
  //}

  return 0;
}
