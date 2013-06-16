#include "Arduino.h"
#include "DirectShotControl.h"

char* DirectShotControl::controlName() {
  return "Direct";
}

int DirectShotControl::pumpSpeed(int arm, int seconds) {
  return arm;
}
