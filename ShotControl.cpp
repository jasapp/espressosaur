#include "Arduino.h"
#include "ShotControl.h"

ShotControl::ShotControl() {
  preinfuse_gap = 25;
  solenoid_open_at = 220;
  solenoid_close_at = 180;
  pump_start_at = 255;
}

int ShotControl::startShot(int arm) {
  return (arm > pump_start_at);
}

int ShotControl::solenoidOpen(int arm) {
  return (arm >= solenoid_open_at);
}

int ShotControl::solenoidClose(int arm) {
  return (arm < solenoid_close_at);
}

int ShotControl::stopShot(int arm) { 
  return (arm < solenoid_close_at); 
}

char* ShotControl::controlName() {
  return "Direct"; 
}

// int ShotControl::pumpSpeed(int arm) {
//   return (arm > pump_start_at) ? (arm - pump_start_at) : 0;
// }
