/*
 * Espresso Machine Control
 * Pulling shots with an arduino
 *
 * Kshanti Coffee - www.kshanticoffee.com
 * Author: Jeff Sapp <jeff@kshanticoffee.com>
 * Date: April 17, 2013
 */

#define shot_arm 0
#define pump_output 11
#define shot_heating_element 9
#define solenoid 7

// const int shot_solenoid_threshold = 5; 
// const int shot_pump_threshold = 20;

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
  if (!solenoidPosition()) {
    operateSolenoid(1);
  }
}

void closeSolenoid() {
  if (solenoidPosition()) {
    operateSolenoid(0);
  }
}

int pumpRunning() {
  return pump_speed;
}

void setPumpSpeed(int speed) {
  if (pumpRunning()) {
    if (speed == 0) {
      Serial.println("Stopping pump");
    } else if (speed != pump_speed) {
      Serial.print("Pump speed: ");
      Serial.print(speed);
      Serial.println("%");
    }
  }

  pump_speed = speed; 
  analogWrite(pump_output, map(speed, 0, 100, 0, 255));
}

void stopPump() {
  setPumpSpeed(0);
}

int shotInProgress() {
  return shot_in_progress;
}

void stopShot() {
  shot_in_progress = 0;
  closeSolenoid();
}

void startShot() {
  shot_in_progress = 1; 
  openSolenoid();
}

int readShotHandle() {
  return map(analogRead(shot_arm), 0, 1023, 0, 100);
}

// can be replaced by other functions
void manageShotBoiler() {
  int val = readShotHandle();
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

void setupTimer() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  timer_counter = 62000;   // preload timer 65536-16MHz/256/50Hz
  timer_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

  interrupts();
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer_counter;
  // just for now...
  digitalWrite(shot_heating_element, digitalRead(shot_heating_element) ^ 1);

  if (shotInProgress())
    sendShotData();
}

void setup() {
  pinMode(shot_arm, INPUT); 
  pinMode(solenoid, OUTPUT);
  pinMode(shot_heating_element, OUTPUT);
  Serial.begin(9600);
  setupTimer();
}

void loop() {
  manageShotBoiler();
  manageSteamBoiler();
  // check shot arm
  // read messages
}
