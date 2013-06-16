/*
  Machine.h
*/

#ifndef Machine_h
#define Machine_h

#include "Arduino.h"

class Machine { 
  int seconds;
  int solenoid_position;
  int pump_speed;
 public:
  Machine();
  int solenoidPosition(); 
  int openSolenoid();
  int closeSolenoid();
  int pumpSpeed();
  int setPumpSpeed(int);
  int stopPump();
  int shotArmPosition(); 
  int shotArmPercentage();
  int currentShotDuration();
  void updateSeconds();
  void resetSeconds();
 private:
  int operateSolenoid(int position);
};

#endif
