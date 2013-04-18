/*
 * Espresso Machine Control
 * Pulling shots with an arduino
 *
 * Kshanti Coffee - www.kshanticoffee.com
 * Author: Jeff Sapp <jeff@kshanticoffee.com>
 * Date: April 17, 2013
 */

// https://github.com/rocketscream/Low-Power.git
#include <LowPower.h>

// https://github.com/dreamcat4/CmdMessenger.git
#include <CmdMessenger.h>

#define shot_arm 0
#define analog_comparator_v 5
#define solenoid 13
#define shot_heating_element 1
#define pump_output 11

const int shot_solenoid_threshold = 5; 
const int shot_pump_threshold = 20;

int solenoid_open_at;
int pump_start_at;

int shot_in_progress = 0;
int shot_counter = 0;
int solenoid_position = 0; 
int pump_speed = 0;
int timer_counter = 0;

void sendShotData() {

}

void operateSolenoid(int position) {
  digitalWrite(solenoid, position);
  solenoid_position = position;
}

int solenoidPosition() {
  return solenoid_position;
}

void openSolenoid() {
  operateSolenoid(0);
}

void closeSolenoid() {
  operateSolenoid(1);
}

int pumpRunning() {
  return pump_speed;
}

// speed is between 0 and 1023 where 0 is off
void setPumpSpeed(int speed) {
  pump_speed = speed; 
  analogWrite(pump_output, speed / 4);
}

void stopPump() {
  setPumpSpeed(0);
}

// returns a value between 0 and 1023 where 0 is closed
int shotArmPosition() {
  return analogRead(shot_arm);
}

int shotInProgress() {
  return shot_in_progress;
}

void stopShot() {
  shot_counter++;
  shot_in_progress = 0;
  stopPump();
  closeSolenoid();
}

void startShot() {
  shot_in_progress = 1; 
  openSolenoid();
}

// can be replaced by other functions
void manageShotBoiler() {
  int val = shotArmPosition();
  if (val > shot_solenoid_threshold) {

    if (!shotInProgress()) {
      startShot();
    }

    if (val > shot_pump_threshold) {
      if (pump_speed != val) {
	setPumpSpeed(val);
      }
    } else {
      stopPump();
    }
  } else if (shotInProgress()) {
    stopShot();
  }
}

void manageSteamBoiler() {
 
}

int calculateInterval(int hz) {
  return 65536 - 16 / 256 / hz;
}

void setupTimer() {
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  timer_counter = calculateInterval(2);  
  TCNT1 = timer_counter;                   // preload timer
  TCCR1B |= (1 << CS12);                   // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);                  // enable timer overflow interrupt

  interrupts();
}

void timerOff() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  interrupts();
}

void shotHandleInterrupt() {
  ACSR = B01011011;
}

void cancelShotHandleInterrupt() {
  ACSR = B11011011;
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = timer_counter;
  int arm = shotArmPosition();

  // it's possible for the solenoid to be open without the pump running
  // this gives us preinfusion at line pressure
  setPumpSpeed(arm);
  openSolenoid();

  // if the arm has moved to the off position
  if (arm < analog_comparator_v) {
    stopShot();
    shotHandleInterrupt();
    timerOff();
  }
}

ISR(ANALOG_COMP_vect) {
  cancelShotHandleInterrupt();  // the handle has been moved
  setupTimer();                 // set the timer so we can watch the handle closer
}

void setupHandle() {
  solenoid_open_at = analogRead(analog_comparator_v);
  pump_start_at = solenoid_open_at + 20; 
}

void setup() {
  pinMode(shot_arm, INPUT); 
  pinMode(analog_comparator_v, INPUT);
  pinMode(solenoid, OUTPUT);
  pinMode(shot_heating_element, OUTPUT);
  pinMode(pump_output, OUTPUT);
  Serial.begin(9600);

  setupHandle();
  stopPump();
  closeSolenoid();
  shotHandleInterrupt();
}

void loop() {

}
