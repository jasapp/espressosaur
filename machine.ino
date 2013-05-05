#define shot_arm 0
#define solenoid_open 3
#define solenoid_close 4
#define data_led 7
#define pump_output 8
#define shot_heating_element 12

int solenoid_position = 0; 
int pump_speed = 0;

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

int pumpSpeed() {
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
  int position = analogRead(shot_arm);     // sometimes we see negative values 
  return (position >= 0 ? position : 0);   // so we remove them
}

int shotArmPercentage() {
  return map(shotArmPosition(), solenoid_open_at, 1023, 0, 100);
}

void setupMachine() {
  pinMode(shot_arm, INPUT); 
  pinMode(solenoid_open, OUTPUT);
  pinMode(solenoid_close, OUTPUT);
  pinMode(shot_heating_element, OUTPUT);
  pinMode(pump_output, OUTPUT);
  pinMode(data_led, OUTPUT);
}

