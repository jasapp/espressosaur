/*
  DirectShotControl.h
*/
#ifndef DirectShotControl_h
#define DirectShotControl_h

#include "Arduino.h"
#include "ShotControl.h"

class DirectShotControl: public ShotControl {
 public: 
  char* controlName(); 
  int pumpSpeed(int); 
};

#endif
