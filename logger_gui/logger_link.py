# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""

import serial
import glob
import platform

from params import *

class LoggerLink:
	def __init__(self):
		self.connected = False
		self.s = None
		self.baud = 115200*2

	def connect(self, port):
		""" Try to open the port and ping the device there
		"""
		try:
			self.s = serial.Serial(str(port), self.baud, timeout=1)
			self.s.flushInput()
			self.s.write("ping\r")
			data = self.s.read(10)
			
			if(data == "pong\r\nOK\r\n"):
				self.connected = True
				return True
			
		except serial.SerialException:
			return False
		return False
		
	def get_command_list(self):
		""" Query the device for available commands
		"""
		if not self.connected:
			return []
		
		self.s.flushInput()
		self.s.write("listcmds\r")
		data = self.s.read(2000).split("\r\n")
		return data[:-2]

	def disconnect(self):
		if self.connected:
			self.s.close()
			self.connected = False

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
	