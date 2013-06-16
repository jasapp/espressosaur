/*
  RampShotControl.h
*/
#ifndef RampShotControl_h
#define RampShotControl_h

#include "Arduino.h"
#include "ShotControl.h"

class RampShotControl: public ShotControl {
  int max;
  int preinfuse_duration;
  int ramp_duration;
  int soak_duration;
  int down_duration;
  int end; 
 public: 
  RampShotControl();
  char* controlName(); 
  int pumpSpeed(int, int); 
  int solenoidOpen(int);
  int solenoidClose(int);
 private:
  int rampUp(int, int); 
  int rampDown(int);
};

#endif
