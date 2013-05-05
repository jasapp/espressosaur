#define solenoid_open 3
#define solenoid_close 4

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

void setupMachine() {
  pinMode(solenoid_open, OUTPUT);
  pinMode(solenoid_close, OUTPUT);
}