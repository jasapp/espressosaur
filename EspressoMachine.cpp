#include "EspressoMachine.h"

// a linker error, adding this line fixes it
// from the avr-c++ micro how-to
extern "C" void atexit(void) {}

void shotHandleInterrupt() {
  ACSR = B01011011;
}

void timerOff() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR3A = 0;
  TCCR3B = 0;
  TCCR4A = 0;
  TCCR4B = 0;
  interrupts();
}

EspressoMachine& EspressoMachine::getInstance() {
  static EspressoMachine instance; 
  return instance; 
}

void EspressoMachine::setup() {
  manageLcd();
  shotHandleInterrupt(); 
}

bool EspressoMachine::shotInProgress() {
  return shot_in_progress;
}

void EspressoMachine::startShot() {
  shot_in_progress = true; 
  machine.openSolenoid();
}

void EspressoMachine::stopShot() {
  shot_counter++;
  shot_in_progress = false;
  machine.stopPump();
  machine.closeSolenoid();
  machine.resetSeconds();
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

void EspressoMachine::operateSolenoid() {
  int arm = machine.shotArmPosition();

  // if the arm has moved to the on position
  if (shot_control.solenoidOpen(arm)) {
    startShot();
  } else {
    // if the arm has moved to the off position
    if (shot_control.solenoidClose(arm)) {
      timerOff();
      stopShot();
      shotHandleInterrupt();
    }
  }
}




