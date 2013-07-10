/*
 * Espresso Machine Control
 * Pulling shots with an arduino
 *
 * Kshanti Coffee - www.kshanticoffee.com
 * Author: Jeff Sapp <jeff@kshanticoffee.com>
 * Date: April 17, 2013
 */

// https://github.com/dreamcat4/CmdMessenger.git
#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include "Machine.h"
#include "EspressoMachine.h"
#include "Lcd.h"
#include "ShotControl.h"
#include "DirectShotControl.h"
#include "RampShotControl.h"
#include "SerialCommunication.h"

// https://github.com/rocketscream/Low-Power.git
// #include <LowPower.h>

Machine *machine;
Lcd *lcd;
ShotControl *shot_control;

bool shot_in_progress = false;
int shot_counter = 0;
int timer1_counter = 0;
int timer3_counter = 0;
int timer4_counter = 0;

volatile int send_serial = 0;
volatile int shot_started_at = 0; 

void sendShotData() {
  if (shotInProgress() && send_serial) {
    shotDataCmd(millis() - shot_started_at, machine->pumpSpeed());
  }
}

bool shotInProgress() {
  return shot_in_progress;
}

void stopShot() {
  shot_counter++;
  shot_in_progress = false;
  EspressoMachine::getInstance().stopShot();
  machine->stopPump();
  machine->closeSolenoid();
  machine->resetSeconds();
}

void startShot() {
  shot_in_progress = true; 
  EspressoMachine::getInstance().startShot();
  machine->openSolenoid();
}

void setupTimer() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_counter = 64886;
  TCNT1 = timer1_counter;                   // preload timer, 100 Hz
  TCCR1B |= (1 << CS12);                    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);                   // enable timer overflow interrupt

  TCCR3A = 0;
  TCCR3B = 0;
  timer3_counter = 3036;
  TCNT3 = timer3_counter;                   // preload timer, 1 Hz
  TCCR3B |= (1 << CS32);                    // 256 prescaler
  TIMSK3 |= (1 << TOIE3);                   // enable timer overflow interrupt

  TCCR4A = 0;
  TCCR4B = 0;
  timer4_counter = 31250;
  TCNT4 = timer4_counter;                   // preload timer, 2 Hz
  TCCR4B |= (1 << CS42);                    // 256 prescaler
  TIMSK4 |= (1 << TOIE4);                   // enable timer overflow interrupt
  interrupts();
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

void shotHandleInterrupt() {
  ACSR = B01011011;
}

void cancelShotHandleInterrupt() {
  ACSR = B11011011;
}

// don't leave this here
// move the timer into Machine (?)
ISR(TIMER3_OVF_vect) {
  TCNT3 = timer3_counter;
  EspressoMachine::getInstance().machine.updateSeconds();
}

ISR(TIMER4_OVF_vect) {
  TCNT4 = timer4_counter;
  send_serial = 1;
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = timer1_counter;
  EspressoMachine::getInstance().updatePumpSpeed();

  // if the arm has moved to the on position
  if (shot_control->solenoidOpen(arm)) {
    startShot();
  } else {
    // if the arm has moved to the off position
    if (shot_control->solenoidClose(arm)) {
      timerOff();
      stopShot();
      shotHandleInterrupt();
    }
  }
}

ISR(ANALOG_COMP_vect) {
  shot_started_at = millis();
  cancelShotHandleInterrupt();  // the handle has been moved
  setupTimer();                 // set the timer so we can watch the handle closer
}

void setup() {
  machine = new Machine();
  shot_control = new DirectShotControl();
  // EspressoMachine::getInstance().manageLcd();
  setupCmds();
  shotHandleInterrupt(); 
}

void loop() {
  sendShotData();
  EspressoMachine::getInstance().manageLcd();
  manageCmds();
  delay(10);
}
