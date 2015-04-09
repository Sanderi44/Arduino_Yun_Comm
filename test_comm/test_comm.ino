#include <Arduino_Yun_Comm.h>


bool START_FLAG = false;
Arduino_Yun_Comm *com;

void setup() {
  // put your setup code here, to run once:
  com = new Arduino_Yun_Comm();
  Serial.begin(9600);
  Serial.flush();
  Serial1.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(START_FLAG);
  String command = com->cmd();
  if (command != "NONE"){
    //Serial.println(command);
  }
  START_FLAG = aamsCommandHandler(START_FLAG, command);
  delay(50);
}


bool aamsCommandHandler(bool START_FLAG, String command){
  if (command != "NONE"){
    Serial.print("ARDUINO_DOES: ");
    Serial.println(command);
  }
  if (command.startsWith("CONNECT")){
    com->sendCmd("connect");
    Serial.println("ARDUINO SEND: CONNECT");
  } else if (command.startsWith("READY")){
    com->sendCmd("start");
    Serial.println("ARDUINO SEND: START");
  } else if (command.startsWith("START")){
    Serial.println("ARDUINO SET: START_FLAG - TRUE");
    START_FLAG = true;
  } else if (command.startsWith("STOP")){
    Serial.println("ARDUINO SET: START_FLAG - FALSE");
    START_FLAG = false;
  } else if (command.startsWith("DISCONNECT")){
    Serial.println("ARDUINO SET: START_FLAG - FALSE, DISCONNECT");
    START_FLAG = false;
  }
  return START_FLAG;
}
