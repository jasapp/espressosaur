/*
  ShotControl.h
*/
#ifndef ShotControl_h
#define ShotControl_h

#include "Arduino.h"

class ShotControl {
  int preinfuse_gap;
  int solenoid_open_at;
  int solenoid_close_at;
  int pump_start_at;
 public: 
  ShotControl();
  int startShot(int);
  int stopShot(int); 
  int solenoidOpen(int);
  int solenoidClose(int);
  virtual char* controlName() = 0; 
  virtual int pumpSpeed(int) = 0; 
};

#endif
