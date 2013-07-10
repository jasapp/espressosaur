#include "EspressoMachine.h"

// a linker error, adding this line fixes it
// from the avr-c++ micro how-to
extern "C" void atexit(void) {}

EspressoMachine& EspressoMachine::getInstance() {
  static EspressoMachine instance; 
  return instance; 
}

bool EspressoMachine::shotInProgress() {
  return shot_in_progress;
}

void EspressoMachine::startShot() {
  shot_in_progress = true; 
}

void EspressoMachine::stopShot() {
  shot_in_progress = false;
}

void EspressoMachine::manageLcd() {
  if (shotInProgress()) {
    lcd.lcdShot(machine.pumpSpeed(), 0, machine.currentShotDuration());
  } else {
    lcd.writeMode(shot_control.controlName());
  }
}

void EspressoMachine::updatePumpSpeed() {
  int arm = machine.shotArmPercentage();
  int seconds = machine.currentShotDuration();
  int new_pump_speed = shot_control.pumpSpeed(arm, seconds);

  machine.setPumpSpeed(new_pump_speed); 
}
