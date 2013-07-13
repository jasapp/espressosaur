#include "Arduino.h"
#include "Machine.h"

static const int shot_arm = 0;
static const int solenoid_open = 3;
static const int solenoid_close = 4;
static const int pump_output = 13;
static const int pump_tach = 12;
static const int shot_element = 11;
static const int shot_thermistor_one = 13;
static const int shot_thermistor_two = 13;
static const int shot_thermistor_three = 13;
static const int steam_element_one = 13;
static const int steam_element_two = 14;

Machine::Machine() {
  pinMode(shot_arm, INPUT); 
  pinMode(solenoid_open, OUTPUT);
  pinMode(solenoid_close, OUTPUT);
  pinMode(shot_element, OUTPUT);
  pinMode(pump_output, OUTPUT);
  pinMode(pump_tach, INPUT);

  resetSeconds();
  stopPump();
  closeSolenoid();
}

// returns true if solenoid is open
bool Machine::operateSolenoid(bool open) {
  digitalWrite(solenoid_open, open);
  digitalWrite(solenoid_close, !open);
  solenoid_is_open = open;
  return solenoid_is_open;
}

bool Machine::solenoidIsOpen() {
  return solenoid_is_open;
}

bool Machine::openSolenoid() {
  return operateSolenoid(true);
}

bool Machine::closeSolenoid() {
  return operateSolenoid(false);
}

int Machine::pumpTach() {
  return analogRead(pump_tach);
}

int Machine::pumpSpeed() {
  return pump_speed;
}

// speed is between 0 and 1023 where 0 is off
int Machine::setPumpSpeed(int speed) {
  pump_speed = speed; 
  analogWrite(pump_output, map(speed, 0, 100, 0, 255));
  return speed;
}

int Machine::stopPump() {
  setPumpSpeed(0);
  return 0;
}

int Machine::currentShotDuration() {
  return seconds;
}

void Machine::updateSeconds() {
  seconds += 1; 
}

void Machine::resetSeconds() {
  seconds = 0; 
}

// returns a value between 0 and 1023 where 0 is closed
int Machine::shotArmPosition() {
  int position = analogRead(shot_arm);     // sometimes we see negative values 
  return (position >= 0 ? position : 0);   // so we remove them
}

int Machine::shotArmPercentage() {
  return map(shotArmPosition(), 220, 1023, 0, 100);
}
