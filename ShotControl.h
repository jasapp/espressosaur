/*
  ShotControl.h
*/
#ifndef ShotControl_h
#define ShotControl_h

#include "Arduino.h"

class ShotControl {
 public: 
  ShotControl();
  int startShot(int);
  int stopShot(int); 
  int solenoidOpen(int);
  int solenoidClose(int);
  char* controlName(); 
  int pumpSpeed(int); 
};

#endif
