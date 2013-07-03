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
  ShotControl() : 
      preinfuse_gap(25), 
      solenoid_open_at(220), 
      solenoid_close_at(180),
      pump_start_at(255) { } ;
 public: 
  int startShot(int arm);
  int stopShot(int arm); 
  virtual int solenoidOpen(int arm);
  virtual int solenoidClose(int arm);
  const virtual char* controlName() = 0; 
  virtual int pumpSpeed(int arm, int seconds) = 0; 
};

#endif
