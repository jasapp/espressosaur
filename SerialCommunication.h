#ifndef DATA_H
#define DATA_H

void status();
void arduino_ready();
void shotStartCmd();
void shotDataCmd(int seconds, int percentage); 
void shotEndCmd(); 
void unknownCmd();
void setupCmds();
void manageCmds();

#endif
