/*
  Machine.h
*/

#ifndef Machine_h
#define Machine_h

class Machine { 
  int seconds;
  int pump_speed;
  bool solenoid_is_open;
 public:
  Machine();
  bool solenoidIsOpen(); 
  bool openSolenoid();
  bool closeSolenoid();
  int pumpSpeed();
  int pumpTach();
  int setPumpSpeed(int);
  int stopPump();
  int shotArmPosition(); 
  int shotArmPercentage();
  int currentShotDuration();
  void updateSeconds();
  void resetSeconds();
 private:
  bool operateSolenoid(bool open);
};

#endif
