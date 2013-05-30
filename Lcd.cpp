#include "Arduino.h"
#include "Lcd.h"
#include <SoftwareSerial.h>

#define lcd_length 16
#define IDLE_STATE 0
#define SHOT_STATE 1

Lcd::Lcd() : serial(0,2) {
  current_state = -1;
  last_bars = 0; 

  serial.begin(9600);
  addChars();
  setContrast(200);
  setBrightness(255);
  cursorOff();
  blockCursorOff();
  setBackground(255,255,255); // white
  // setBackground(205,0,100); // dark pink
  // setBackground(255,0,255); // purple
  // setBackground(255,255,255); // blueish green
}

void Lcd::createChar(char *custom, int location) {
  serial.write(0xFE);
  serial.write(0x4E);
  serial.write((uint8_t)location);

  for (int i=0; i<8; i++) {
    serial.write(custom[i]);
  }
}

void Lcd::addChars() {
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

void Lcd::writeLcdCommand(char *cmds, int count, int delay_cmd) {
  serial.write(0xFE);
  for (int i=0; i < count; i++) 
    serial.write(cmds[i]);

  if (delay_cmd) {
    delay(10); 
  }
}

// delay after the command
void Lcd::writeLcdCommand(char *cmds, int count) {
  writeLcdCommand(cmds, count, 1);
}

void Lcd::writeLcdCommand(char cmd) {
  writeLcdCommand(&cmd, 1); 
}

void Lcd::clearScreen() {
  writeLcdCommand(0x58);
}

void Lcd::goHome() {
  writeLcdCommand(0x48);
}

void Lcd::moveForward() {
  writeLcdCommand(0x4D);
}

void Lcd::moveBackward() {
  writeLcdCommand(0x4C);
}

void Lcd::cursorOff() {
  writeLcdCommand(0x4B);
}

void Lcd::blockCursorOff() {
  writeLcdCommand(0x4B);
}

int Lcd::checkColor(int color) {
  if (color < 0) 
    return 0;

  if (color > 255) 
    return 255; 

  return color;
}

void Lcd::setBackground(int red, int green, int blue) {
  char cmds[] = { 0xD0, checkColor(red), checkColor(green), checkColor(blue) };
  writeLcdCommand(cmds, 4);
}

void Lcd::setContrast(int contrast) {
  char cmds[] = { 0x50, contrast };
  writeLcdCommand(cmds, 2); 
}

void Lcd::setBrightness(int brightness) {
  char cmds[] = { 0x99, brightness };
  writeLcdCommand(cmds, 2); 
}

void Lcd::setCursor(int column, int row) {
  char cmds[] = { 0x47, column, row };
  writeLcdCommand(cmds, 3); 
}

void Lcd::lcdMessage(char *message) {
  serial.write(message);
}

void Lcd::clearLcd() {
  clearScreen();
  goHome();
}

int Lcd::percentageToBars(int percentage) {
  double a = (lcd_length / 100.0) * percentage;
  int bars = 0; 
  for (int i=0; i<a; i++) {
    bars += 1; 
  }
  return bars; 
}

void Lcd::percentageBar(int bars) {
  for (int i=0; i<lcd_length; i++) {
    if (i < bars) {
      serial.write(4);
    } else {
      serial.write(" ");
    }
  }
}

// just playing around here, clean this up sometime
void Lcd::fadeBackground(int percentage) {
  int green = map(percentage, 0, 100, 255, 0);
  setBackground(255, green, 255);
}

// don't use the blinking block cursor, in the movie it doesn't blink
void Lcd::shallWePlayAGame() {
  char quote[] = "Shall we play a game?";
}

int Lcd::spacesToScoot(int num, int *spaces) {
  int start = 0;
  if (num > 9)
    start += 1; 
  if (num > 99)
    start += 1; 

  return spaces[start];
}

void Lcd::writeGrams(int grams) {
  char gram_str[3];
  int spaces[] = { 2, 1, 1 };

  sprintf(gram_str, "%dg", grams);
  setCursor(spacesToScoot(grams, spaces), 2);
  lcdMessage(gram_str); 
}

void Lcd::writeSeconds(int seconds) {
  char second_str[3];
  int spaces[] = { 15, 14, 13 };
  
  sprintf(second_str, "%ds", seconds);
  setCursor(spacesToScoot(seconds, spaces), 2);
  lcdMessage(second_str);
}

void Lcd::writeMode(char *mode) {
  if (current_state != IDLE_STATE) {
    clearLcd();
    lcdMessage(mode);
  }
  current_state = IDLE_STATE; 
}

void Lcd::lcdShot(int percentage, int grams, int seconds) {
  int bars = percentageToBars(percentage);
  if (current_state != SHOT_STATE || bars != last_bars) { 
    goHome();
    percentageBar(bars);
    fadeBackground(percentage);
    last_bars = bars;
  }

  writeSeconds(seconds);
  current_state = SHOT_STATE;
}

void Lcd::lcdShotSummary(int shot, int grams, int seconds) {

}
