// https://github.com/dreamcat4/CmdMessenger.git
#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>

char field_separator = ',';
char command_separator = ';';

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised
  kSHOT_START    = 004,
  kSHOT_DATA     = 005,
  kSHOT_END      = 006,
  kSEND_CMDS_END, // Mustnt delete this line
};

// Commands we send from the PC and want to recieve on the Arduino.
messengerCallbackFunction messengerCallbacks[] = 
{
  status,
  NULL
};

void status() {
  cmdMessenger.sendCmd(kACK, "Status");
}

// ------------------ C A L L B A C K S -----------------------
void arduino_ready() {
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void shotStartCmd() {
  cmdMessenger.sendCmd(kSHOT_START,"Data to follow");
}

void shotDataCmd(int seconds, int percentage) {
  // just make the buffer small and encode the whole thing right now
  char data_buf[32];
  char base64_buf[256] = { '\0' };
  sprintf(data_buf, "s:%d,p:%d", seconds, percentage);
  base64_encode(base64_buf, data_buf, 32);
  // cmdMessenger.sendCmd(kSHOT_DATA, base64_buf);
  cmdMessenger.sendCmd(kSHOT_DATA, data_buf);
}

void shotEndCmd() {
  cmdMessenger.sendCmd(kSHOT_END,"Shot ended");
}

void unknownCmd() {
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks) {
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i]) {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setupCmds() {
  // Listen on serial connection for messages from the pc
  Serial.begin(9600); 
  cmdMessenger.print_LF_CR();
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);
  arduino_ready();
}

void manageCmds() {
  cmdMessenger.feedinSerialData();
}
