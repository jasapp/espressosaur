#define lcd_length 16
#define IDLE_STATE 0
#define SHOT_STATE 1

int current_state = -1;

void createChar(char *custom, int location) {
  lcd.write(0xFE);
  lcd.write(0x4E);
  lcd.write((uint8_t)location);

  for (int i=0; i<8; i++) {
    lcd.write(custom[i]);
  }
}

void addChars() {
  char p1[] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
  char p2[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 };
  char p3[] = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C };
  char p4[] = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E };
  char p5[] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

  createChar(p1, 0);
  createChar(p2, 1);
  createChar(p3, 2);
  createChar(p4, 3);
  createChar(p5, 4);
}

void writeLcdCommand(char *cmds, int count, int delay_cmd) {
  lcd.write(0xFE);
  for (int i=0; i < count; i++) 
    lcd.write(cmds[i]);

  if (delay_cmd) {
    delay(10); 
  }
}

// delay after the command
void writeLcdCommand(char *cmds, int count) {
  writeLcdCommand(cmds, count, 1);
}

void writeLcdCommand(char cmd) {
  writeLcdCommand(&cmd, 1); 
}

void clearScreen() {
  writeLcdCommand(0x58);
}

void goHome() {
  writeLcdCommand(0x48);
}

void moveForward() {
  writeLcdCommand(0x4D);
}

void moveBackward() {
  writeLcdCommand(0x4C);
}

void cursorOff() {
  writeLcdCommand(0x4B);
}

void blockCursorOff() {
  writeLcdCommand(0x4B);
}

void setBackground(int red, int green, int blue) {
  char cmds[] = { 0xD0, red, green, blue };
  writeLcdCommand(cmds, 4);
}

void setContrast(int contrast) {
  char cmds[] = { 0x50, contrast };
  writeLcdCommand(cmds, 2); 
}

void setBrightness(int brightness) {
  char cmds[] = { 0x99, brightness };
  writeLcdCommand(cmds, 2); 
}

void setCursor(int column, int row) {
  char cmds[] = { 0x47, column, row };
  writeLcdCommand(cmds, 3); 
}

void lcdMessage(char *message) {
  lcd.write(message);
}

void clearLcd() {
  clearScreen();
  goHome();
}

void setupLcd() {
  addChars();
  setContrast(200);
  setBrightness(255);
  cursorOff();
  blockCursorOff();
  // setBackground(205,0,100); // dark pink
  // setBackground(255,0,255); // purple
  setBackground(255,255,255); // white
 // setBackground(255,255,255); // blueish green
}

int percentageToBars(int percentage) {
  double a = (lcd_length / 100.0) * percentage;
  int bars = 0; 
  for (int i=0; i<a; i++) {
    bars += 1; 
  }
  return bars; 
}

int last_bars = 0;

void percentageBar(int bars) {
  for (int i=0; i<lcd_length; i++) {
    if (i < bars) {
      lcd.write(4);
    } else {
      lcd.write(" ");
    }
  }
}

// just playing around here, clean this up sometime
void fadeBackground(int percentage) {
  // int red = map(percentage, 0, 100, 255, 0);
  //int green = map(percentage, 0, 100, 255, 160);
  int green = map(percentage, 0, 100, 255, 0);
  // setBackground(red, green, 255);
  setBackground(255, green, 255);
}

// don't use the blinking block cursor, in the movie it doesn't blink
void shallWePlayAGame() {
  char quote[] = "Shall we play a game?";
}

int spacesToScoot(int num, int *spaces) {
  int start = 0;
  if (num > 9)
    start += 1; 
  if (num > 99)
    start += 1; 

  return spaces[start];
}

void writeGrams(int grams) {
  char gram_str[3];
  int spaces[] = { 2, 1, 1 };

  sprintf(gram_str, "%dg", grams);
  setCursor(spacesToScoot(grams, spaces), 2);
  lcdMessage(gram_str); 
}

void writeSeconds(int seconds) {
  char second_str[3];
  int spaces[] = { 15, 14, 13 };
  
  sprintf(second_str, "%ds", seconds);
  setCursor(spacesToScoot(seconds, spaces), 2);
  lcdMessage(second_str);
}

void lcdIdle() {
  if (current_state != IDLE_STATE) {
    clearLcd();
    lcdMessage("Idle");
  }
  current_state = IDLE_STATE; 
}

void lcdShot(int percentage, int grams, int seconds) {
  int bars = percentageToBars(percentage);
  if (current_state != SHOT_STATE || bars != last_bars) { 
    goHome();
    percentageBar(bars);
    fadeBackground(percentage);
    writeSeconds(percentage);
    writeGrams(percentage);
    last_bars = bars;
  }

  current_state = SHOT_STATE;
}

void lcdShotSummary(int shot, int grams, int seconds) {

}
