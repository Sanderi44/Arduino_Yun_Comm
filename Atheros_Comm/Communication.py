import serial, time, os, Queue, threading

class Communication():
	def __init__(self, port="/dev/ttyATH0"):
		self.baudrate = 9600
		self.port = port
		self.ser = serial.Serial(self.port, self.baudrate)
		self.filepath = ""
		self.filename = "/data.txt"
		self.write = False
		self.sdAvailable = False
		self.CONNECTED = False
		self.timeSyncOnStartup = True

	def connectArduino(self):
		self.ser.baudrate = self.baudrate
		self.ser.open()
		self.sendConnect()

	def sendConnect(self):
		print "ATHEROS_SENT: " + "CONNECT"
		self.writeSerial("CONNECT")

	def disconnectArduino(self):
		self.CONNECTED = False
		print "ATHEROS_SENT: " + "DISCONNECT"
		self.writeSerial("DISCONNECT")
		time.sleep(0.5)
		self.ser.close()

	def resetArduino(self):
		self.disconnectArduino()
		self.ser.baudrate = 1200
		self.ser.open()
		self.ser.close()

	def getTime(self):
		print "ATHEROS_SENT: " + "TIME"
		self.writeSerial("TIME")

	def commandHandler(self, line, RUN, connect=None, dataProcess=None, deviceSpecific=None, timeFunction=None):
		print "ARDUINO_SENT: " + line
		if line is not None:
			if line.startswith("connect") == True:
				print "ATHEROS_DOES: " + line
				self.CONNECTED = connect()
				if self.CONNECTED == True:
					self.writeSerial("READY")
					print "ATHEROS_SENT: " + "READY"

			elif line.startswith("start") == True:
				if RUN == False and self.CONNECTED == True:
					print "ATHEROS_DOES: " + line
					command = "START"
					if self.timeSyncOnStartup == False:
						command = command + "F"
					self.writeSerial(command)
					print "ATHEROS_SENT: " + "START"
					RUN = True
			elif line.startswith("stop") == True:
				if RUN == True and self.CONNECTED == True:
					print "ATHEROS_DOES: " + line
					self.writeSerial("STOP")
					print "ATHEROS_SENT: " + "STOP"
					RUN = False
			elif line.startswith("resetme") == True:
				print "ATHEROS_DOES: " + line
				self.resetArduino()
			elif line.startswith("timesync") == True:
				self.writeSerial("TIME")
			elif line.startswith("time:") == True:
				print "ATHEROS_DOES: " + line
				timeFunction(line)
			elif line.startswith("devicespecific"):
				print "ATHEROS_DOES: " + line
				deviceSpecific(line)
			elif line.startswith("$") == True:
				if RUN == True and self.CONNECTED == True:
					print "ATHEROS_DOES: " + "DATAPROCESS"
					dataProcess(line)
				else:
					print "ATHEROS_DOES: " + "NOT DATAPROCESS"

		return RUN


	def serialDataAvailable(self):
		if self.ser.inWaiting > 0:
			return True
		else:
			return False

	def readSerial(self):
		return self.ser.readline()

	def writeSerial(self, line):
		self.ser.write(line+"\n")

	def flushSerial(self):
		self.ser.flush()

	def writeToSdCard(self, write=True):
		self.write = write
		if self.write == True:
			self.findSD()
			if self.sdAvailable == True:
				self.createFile()

	def findSD(self, path="/mnt/"):
		res = None
		dirs = os.listdir(path)
		for dir in dirs:
			if dir.find("sd") == 1:
				res = path + dir
				self.sdAvailable = True
			else:
				self.sdAvailable = False
		self.sd = res

	def createFile(self, name=None, header=""):
		if self.sd is not None:
			if name is None:
				name = self.filename
			path = self.sd + name

			if os.path.isfile(path):
				os.remove(path)
			f = open(path, "w")
			f.write(header)
			f.close()
			self.filepath = path

	def writeListToFile(self, dataList, path=None):
		if self.write == True:
			if path is None:
				path = self.filepath
			with open(path, 'a') as f:
				f.write(dataList + "\n")
		
	def readListFromFile(self, path=None):
		if self.write == True:
			if path is None:
				path = self.filepath
			f = open(path, "r")
			dataList = f.readlines()
			f.close()
			return dataList

	def syncTime(self):
		self.writeSerial("SYNC")