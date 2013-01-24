# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""

import serial
import glob
import platform

class LoggerLink:
	def __init__(self):
		self.connected = False
		self.s = None

	def connect(self, port):
		self.s = serial.Serial(port)
		return True

	def disconnect(self):
		self.s.close()

	def list_ports(self):
		system_name = platform.system()
		if system_name == "Windows":
			available = []
			for i in range(256):
				try:
					s = serial.Serial(i)
					available.append(i)
					s.close()
				except serial.SerialException:
					pass
			return available
		else:
			return glob.glob("/dev/ttyACM*") + glob.glob("/dev/ttyUSB*")