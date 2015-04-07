/*
  Arduino_Yun_Comm.h - Library for Device Communication on Arduino Yun
*/

#ifndef ARDUINO_YUN_COMM_h
#define ARDUINO_YUN_COMM_h

#include "Arduino.h"

class Arduino_Yun_Comm
{
	public:
		bool _START_FLAG;
		int _ROLLOVERS;
		bool _READY_TO_ROLL;
		unsigned long _START_TIME;
		bool _STARTUP_TIME_SYNC;

		RelayDevice();
		String cmd(void);
		void sendCmd(String);
		void sendDeviceCmd(String);
		void sendData(String);
		void softReset(void);
		bool connection(void);
		bool commandHandler(bool, String);
		int timeRollovers(unsigned long);
		unsigned long getTime(void);
		String timeString(void);
		void timeSyncStartup(bool);
	private:
		bool isCmd(void);
		String rcvCmd(void);
		String doCmd(String);
};






#endif
