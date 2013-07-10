/* 
  EspressoMachine.h
*/

#ifndef EspressoMachine_h
#define EspressoMachine_h

#include "Machine.h"
#include "Lcd.h"
#include "DirectShotControl.h"

class EspressoMachine
{
 public:
  Machine machine;
  Lcd lcd;
  DirectShotControl shot_control;
  void manageLcd();
  bool shotInProgress();
  void startShot();
  void stopShot();
  void updatePumpSpeed();
  static EspressoMachine& getInstance();

 private: 
  bool shot_in_progress;
  int shot_counter; 
  EspressoMachine() : machine(), lcd(), shot_control(), shot_in_progress(false), shot_counter(0) {};
  EspressoMachine(EspressoMachine const& copy); // not implemented
  EspressoMachine& operator= (EspressoMachine const& copy); // not implemented
};

#endif
