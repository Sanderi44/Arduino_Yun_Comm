# Arduino Yun Communication Library
The Arduino Yun Communication Library was developed as a replacement 
for the bridge library.  The goal was to allow the user to send and recieve 
messages from either the Arduino side or the Linux side.  The library also lets the 
Arduino perfom tasks independently of the Linux system.  Instead of relying 
on the Arduino to make Linux system calls and having to wait for them to finish,
you can now perform tasks indepently.  This library allows you to sample a sensor 
continuously with out being interrupted by other processes.

The Arduino side is a c++ library that can be added to any Arduino sketch 
in the Arduino IDE.  The Linux side is a python library that should be added 
to any program that needs to communicate with the Arduino.


## Setup
### Here are the steps to set up your Arduino Yun for the Arduino_Yun_Comm library

1. Install the latest Arduino IDE 
2. Connect Yun to computer via micro USB
3. Follow the wifi setup procedure here: [http://arduino.cc/en/Guide/ArduinoYun](http://arduino.cc/en/Guide/ArduinoYun, "Arduino Yun")
(make sure you that the wireless network is connected to the internet)
4. Run Bridge -> YunSerialTerminal in Arduino IDE under examples.  This is a simple
command line interface to the Linux side, via the Arduino.
5. Type "ifconfig" and look for the wlan ip address.  This is your Yun's IP. 
 You can also use arduino.local or (whatever else you named it).local for ssh 
(although you may have to add it to the hosts file).
6. On a separate computer ssh into the Yun with "ssh root@<name of yun>.local" and password: arduino
7. Once ssh'd in, run these commands to get pip and pyserial:
```
    opkg update
    opkg install distribute
    opkg install python-openssl
    easy_install pip
    pip install pyserial
```
8. The Serial connection between the Atheros and the Arduino is currently in 
use, so we need to turn it off.  In an ssh session:
```
	nano /etc/inittab
	// Comment out:
	ttyAth0::askfirst:/bin/ash --login
	// Save and exit
```
9. To add your yun to the list of known hosts on your linux computer:
```
	nano /etc/hosts
	<ip of Yun> <name of Yun> 
	//Save and exit
```
10. Make sure that the arduino library is in your Libraries folder.  Also, 
make sure that the python library is in your PATH.

Now you can write code via ssh with nano or vi.  You can copy files to the yun 
with scp. 

## Use

To use this library you need to write python code on the Linux side and use: 
```
	from Communication import Communication
```

On the arduino side, simply add:
```
	#include <Arduino_Yun_Comm.h>
```

See the example files in each folder for more detailed use.
