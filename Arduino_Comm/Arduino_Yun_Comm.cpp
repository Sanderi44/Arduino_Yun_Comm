#include "Arduino.h"
#include "Arduino_Yun_Comm.h"

Arduino_Yun_Comm::Arduino_Yun_Comm()
{
	Serial1.begin(9600);
	_START_FLAG = false;
	_ROLLOVERS = 0;
	_READY_TO_ROLL = false;
	_START_TIME = 0;
	_STARTUP_TIME_SYNC = true;
}

bool Arduino_Yun_Comm::connection(void)
{
	if (_START_FLAG){
		return true;
	} else {
		return false;
	}
}


bool Arduino_Yun_Comm::isCmd(void)
{
	if (Serial1.available() > 0){
		return true;
	} else {
		return false;
	}
}

String Arduino_Yun_Comm::rcvCmd(void)
{
	String COMMAND = "";
	while (Serial1.available() > 0){
		char c = Serial1.read();
		COMMAND += c;
	}
	return COMMAND;
}

String Arduino_Yun_Comm::doCmd(String command)
{
	if (command.startsWith("READY")){
		_START_FLAG = true;
	} else if (command.startsWith("DISCONNECT")){
		_START_FLAG = false;
	} 
	return command;
}

String Arduino_Yun_Comm::cmd(void)
{
	if (isCmd()){
		String COMMAND = rcvCmd();
		return doCmd(COMMAND);
	} else {
		return "NONE";
	}
}

bool Arduino_Yun_Comm::commandHandler(bool START_FLAG, String command){
  if (command != "NONE"){
    Serial.print("ARDUINO_DOES: ");
    Serial.println(command);
  }
  if (command.startsWith("CONNECT")){
    sendCmd("connect");
    Serial.println("ARDUINO SEND: CONNECT");
    START_FLAG = false;
  } else if (command.startsWith("READY")){
    sendCmd("start");
    Serial.println("ARDUINO SEND: START");
  } else if (command.startsWith("START")){
    Serial.println("ARDUINO SET: START_FLAG - TRUE");
    if (command.startsWith("STARTF")){
    	timeSyncStartup(false);
    }
    if (_STARTUP_TIME_SYNC){
    	sendCmd("timesync");
    	Serial.println("ARDUINO SEND: TIMESYNC");
    } else {
    	START_FLAG = true;
    }
  } else if (command.startsWith("STOP")){
    Serial.println("ARDUINO SET: START_FLAG - FALSE");
    START_FLAG = false;
  } else if (command.startsWith("DISCONNECT")){
    Serial.println("ARDUINO SET: START_FLAG - FALSE, DISCONNECT");
    START_FLAG = false;
    sendCmd("connect");
  } else if (command.startsWith("SYNC")){
  	sendCmd("timesync");
    Serial.println("ARDUINO SEND: TIMESYNC");
  } else if (command.startsWith("TIME")){
  	Serial.println("ARDUINO SET: TIME SYNC");
  	String timestr = timeString();
  	sendCmd("time:" + timestr);
  	START_FLAG = true;
  }
  return START_FLAG;
}


void Arduino_Yun_Comm::sendCmd(String command)
{
	//command += "\n";
	Serial1.println(command);
}

void Arduino_Yun_Comm::sendDeviceCmd(String command)
{
	String cmd = "devicespecific_";
	cmd += command;
	cmd +="\n";
	sendCmd(cmd);
}

void Arduino_Yun_COmm::sendData(String data){
	data += "\n";
	Serial1.print(data);
}

void Arduino_Yun_Comm::softReset(){
	Serial1.println("reset");
}

int Arduino_Yun_Comm::timeRollovers(unsigned long now){
	const unsigned long halfwayMillis = 2147483647;

	if (now > halfwayMillis){
		_READY_TO_ROLL = true; 
	}

	if (_READY_TO_ROLL && now < halfwayMillis){
		_ROLLOVERS++;
		_READY_TO_ROLL = false;
	}
}

unsigned long Arduino_Yun_Comm::getTime(){
	return millis();
}

String Arduino_Yun_Comm::timeString(){
	unsigned long now = getTime();
	_START_TIME = now;
	timeRollovers(now);
	return "t" + String(now) + "r" + String(_ROLLOVERS);
}

void Arduino_Yun_Comm::timeSyncStartup(bool sync){
	_STARTUP_TIME_SYNC = sync;
}
