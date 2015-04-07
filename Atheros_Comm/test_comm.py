from Communication import Communication
from Relay import Relay
import time

def main():
	"Initialize Local Communication"
	com = Communication()
	com.resetArduino()
	com.connectArduino()
	com.flushSerial()

	"Initialize Relay"
	relay = Relay('relay3.visalab.com')
	relay.configID("sand0002000000000000000000000000", 1)
	relay.statusUpdate("200")

	START = False
	print "Initialized"
	com.sendConnect()
	print "sent connect"
	while True:
		try:
			if com.serialDataAvailable() == True:
				line = com.readSerial()
				START = com.commandHandler(line, START, connect=connect(com, relay))
				
			if com.CONNECTED == False:
				com.sendConnect()
			time.sleep(0.03)
			
		except Exception as a:
			print "EXCEPTION: " + a
			RUN = False
			com.disconnectArduino()
			time.sleep(0.1)
			break


def connect(com, relay):
	timestamp = 0
	timestamp = float(relay.getTime())
	print timestamp
	if timestamp > 0:
		return True
	else:
		return False



if __name__ == "__main__":
	main()