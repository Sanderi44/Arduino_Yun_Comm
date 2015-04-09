# Arduino Yun Communication Library
The Arduino Yun Communication Library was developed as a replacement 
for the bridge library.  The goal was to allow the user to send and recieve 
messages from either the Arduino side or the Linux side.  It also lets the 
Arduino to perfom tasks independently of the other system, instead of relying 
on the Arduino to make Linux system calls and have to wait for them to finish. 
 This library allows you to sample a sensor continuously and not be burdened 
by other processes.

The Arduino side is c++ library that can be added to any Arduino sketch 
in the Arduino IDE.  The Linux side is a python library that should be added 
to any program that needs to communicate with the Arduino.


## Setup
### Here are the steps to set up your Arduino Yun for the Arduino_Yun_Comm
library

1. Install the latest Arduino IDE 
2. Connect Yun to computer via micro USB
3. Follow the wifi setup procedure here: http://arduino.cc/en/Guide/ArduinoYun
(make sure you that the wireless network is connected to the internet)
4. Run Bridge -> YunSerialTerminal
5. Type "ifconfig" and look for the wlan ip address.  This is your Yun's IP. 
 You can also use arduino.local or (whatever else you named it).local for ssh 
(although you may have to add it to the hosts file).
6. On a separate computer ssh into the Yun with "ssh root@<name of yun>.local" and password: arduino
7. Once ssh'd in, run these commands to get pip and pyserial:
	opkg update
	opkg install distribute
 	opkg install python-openssl
	easy_install pip
 	pip install pyserial
8. The Serial connection between the Atheros and the Arduino is currently in 
use, so we need to turn it off.  In an ssh session:
	nano /etc/inittab
	// Comment out:
	ttyAth0::askfirst:/bin/ash --login
	// Save and exit
9. To add your yun to the list of known hosts on your linux computer:
	nano /etc/hosts
	<ip of Yun> <name of Yun> 
	//Save and exit

Now you can write code via ssh with nano or vi.  To copy files to the yun 
use scp. 

## Use



