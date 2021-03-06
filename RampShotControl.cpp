#include "Arduino.h"
#include "ShotControl.h"
#include "RampShotControl.h"

RampShotControl::RampShotControl() {
  end = 0; 
}

char* RampShotControl::controlName() {
  return "Ramp";
}

int RampShotControl::pumpSpeed(int arm, int seconds) {

  max = 100; 
  preinfuse_duration = 5; 
  ramp_duration = 10;
  soak_duration = 10; 
  down_duration = 5;

  float seconds_f = (float)seconds;
  int total_duration = (ramp_duration + soak_duration + down_duration + preinfuse_duration); 

  end = (seconds >= total_duration);

  if (seconds <= preinfuse_duration) {
    return 0;
  }
    
  if (seconds <= ramp_duration + preinfuse_duration) {
    return (int)(((seconds_f - preinfuse_duration) / ramp_duration) * max);
  }

  if (seconds <= preinfuse_duration + ramp_duration + soak_duration) {
    return max; 
  }

  if (seconds <= total_duration) {
    float seconds_remaining = total_duration - seconds_f;
    return (int)((seconds_remaining / down_duration) * max);
  }
}

int RampShotControl::solenoidOpen(int arm) {
  return (end == 0) && ShotControl::solenoidOpen(arm);
}

int RampShotControl::solenoidClose(int arm) {
  return (end == 1) || ShotControl::solenoidClose(arm);
}
