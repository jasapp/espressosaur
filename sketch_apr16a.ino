/*
 * Espresso Machine Control
 * Pulling shots with an arduino
 *
 * Kshanti Coffee - www.kshanticoffee.com
 * Author: Jeff Sapp <jeff@kshanticoffee.com>
 * Date: April 17, 2013
 */

#include <SoftwareSerial.h>

// https://github.com/rocketscream/Low-Power.git
// #include <LowPower.h>

// https://github.com/dreamcat4/CmdMessenger.git
#include <CmdMessenger.h>

#define shot_arm 0
#define solenoid_open 3
#define solenoid_close 4
#define shot_heating_element 12
#define pump_output 8
#define preinfuse_gap 25
#define data_led 7
#define solenoid_open_at 220
#define solenoid_close_at 180

int pump_start_at;
int shot_in_progress = 0;
int shot_counter = 0;
int solenoid_position = 0; 
int pump_speed = 0;
int timer1_counter = 0;
int timer3_counter = 0;
int timer4_counter = 0;

int second_mod = 0; 
volatile int second_counter = 0;
volatile int send_serial = 0;
int data_led_state = 0;

SoftwareSerial lcd = SoftwareSerial(0,2); 

void sendShotData() {
  if (shotInProgress() && send_serial) {
    Serial.println("Data.");
    data_led_state = !data_led_state;
    digitalWrite(data_led, data_led_state);
  }
}

void operateSolenoid(int position) {
  digitalWrite(solenoid_open, !position);
  digitalWrite(solenoid_close, position);
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

int shotArmPercentage() {
  return map(shotArmPosition(), solenoid_open_at, 1023, 0, 100);
}

int shotInProgress() {
  return shot_in_progress;
}

void stopShot() {
  shot_counter++;
  second_counter = 0;
  shot_in_progress = 0;
  stopPump();
  closeSolenoid();
}

void startShot() {
  shot_in_progress = 1; 
  openSolenoid();
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

ISR(TIMER3_OVF_vect) {
  TCNT3 = timer3_counter;
  second_counter += 1;
}

ISR(TIMER4_OVF_vect) {
  TCNT4 = timer4_counter;
  send_serial = 1;
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = timer1_counter;
  int arm = shotArmPosition();

  // it's possible for the solenoid to be open without the pump running
  // this gives us preinfusion at line pressure
  (arm > pump_start_at) ? setPumpSpeed(arm - pump_start_at) : stopPump();

  // if the arm has moved to the on position
  if (arm >= solenoid_open_at) {
    startShot();
  } else {
    // if the arm has moved to the off position
    if (arm < solenoid_close_at) {

      stopShot();
      shotHandleInterrupt();
      timerOff();
    }
  }
}

ISR(ANALOG_COMP_vect) {
  cancelShotHandleInterrupt();  // the handle has been moved
  setupTimer();                 // set the timer so we can watch the handle closer
}

void setupHandle() {
  pump_start_at = solenoid_open_at + preinfuse_gap; 
}

void manageLcd() {
  if (shotInProgress()) {
    lcdShot(shotArmPercentage(),0,second_counter);
  } else {
    lcdIdle(); 
  }
}

void setup() {
  pinMode(shot_arm, INPUT); 
  pinMode(solenoid_open, OUTPUT);
  pinMode(solenoid_close, OUTPUT);
  pinMode(shot_heating_element, OUTPUT);
  pinMode(pump_output, OUTPUT);
  pinMode(data_led, OUTPUT);

  Serial.begin(9600);
  lcd.begin(9600);

  setupLcd();
  manageLcd();

  setupHandle();
  stopPump();
  closeSolenoid();
  shotHandleInterrupt(); 
}

void loop() {
  /* int arm = analogRead(shot_arm); */
  /* Serial.print("Arm: "); */
  /* Serial.println(arm); */
  /* if(shotInProgress()) { */
  /*   Serial.println("Shot in progress!"); */
  /* } */

  sendShotData();
  manageLcd();
  delay(50);
}
