/*
  ShotControl.h
*/
#ifndef ShotControl_h
#define ShotControl_h

#include "Arduino.h"

class ShotControl {
 protected:
  int preinfuse_gap;
  int solenoid_open_at;
  int solenoid_close_at;
  int pump_start_at;
 public: 
  ShotControl();
  int startShot(int);
  int stopShot(int); 
  virtual int solenoidOpen(int) = 0; 
  virtual int solenoidClose(int) = 0; 
  virtual char* controlName() = 0; 
  virtual int pumpSpeed(int, int) = 0; 
};

#endif
