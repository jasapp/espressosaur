#include "Arduino.h"
#include "Machine.h"

#define shot_arm 0
#define solenoid_open 3
#define solenoid_close 4
#define pump_output 8
#define shot_element 12
#define shot_thermistor_one 13
#define shot_thermistor_two 13
#define shot_thermistor_three 13
#define steam_element_one 13
#define steam_element_two 14

Machine::Machine() {
  pinMode(shot_arm, INPUT); 
  pinMode(solenoid_open, OUTPUT);
  pinMode(solenoid_close, OUTPUT);
  pinMode(shot_element, OUTPUT);
  pinMode(pump_output, OUTPUT);

  stopPump();
  closeSolenoid();
}

int Machine::operateSolenoid(int position) {
  digitalWrite(solenoid_open, !position);
  digitalWrite(solenoid_close, position);
  solenoid_position = position;
  return solenoid_position;
}

int Machine::solenoidPosition() {
  return solenoid_position;
}

int Machine::openSolenoid() {
  return operateSolenoid(0);
}

int Machine::closeSolenoid() {
  return operateSolenoid(1);
}

int Machine::pumpSpeed() {
  return pump_speed;
}

// speed is between 0 and 1023 where 0 is off
int Machine::setPumpSpeed(int speed) {
  pump_speed = speed; 
  analogWrite(pump_output, speed / 4);
  return speed;
}

int Machine::stopPump() {
  setPumpSpeed(0);
  return 0;
}

// returns a value between 0 and 1023 where 0 is closed
int Machine::shotArmPosition() {
  int position = analogRead(shot_arm);     // sometimes we see negative values 
  return (position >= 0 ? position : 0);   // so we remove them
}

int Machine::shotArmPercentage() {
  return map(shotArmPosition(), 220, 1023, 0, 100);
}
