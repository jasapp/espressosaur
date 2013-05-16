/*
  Lcd.h
*/
#ifndef Lcd_h
#define Lcd_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class Lcd {
  int current_state; 
  int last_bars;
  SoftwareSerial serial; 
 public: 
  Lcd();
  void clearLcd();
  void setupLcd();
  void lcdIdle(); 
  void lcdShot(int, int, int); 
  void lcdShotSummary(int, int, int); 
  void lcdMessage(char*); 
 private:
  void createChar(char*, int); 
  void addChars();
  void writeLcdCommand(char*, int, int); 
  void writeLcdCommand(char*, int); 
  void writeLcdCommand(char);
  void clearScreen(); 
  void goHome();
  void moveForward();
  void moveBackward();
  void cursorOff();
  void blockCursorOff(); 
  int  checkColor(int); 
  void setBackground(int, int, int); 
  void setContrast(int); 
  void setBrightness(int); 
  void setCursor(int, int); 
  int  percentageToBars(int); 
  void percentageBar(int); 
  void fadeBackground(int); 
  int  spacesToScoot(int, int*); 
  void writeGrams(int); 
  void writeSeconds(int); 
  void shallWePlayAGame();
};

#endif
