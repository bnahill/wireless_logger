# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""

import struct
import serial
import time
import glob
import platform

from params import *

class LoggerLink:
	def __init__(self):
		self.connected = False
		self.s = None
		self.baud = 115200*2
		
	def write_cmd(self, cmd):
		self.s.write(struct.pack('b',len(cmd)) + cmd)
	
	def read_response(self):
		length = self.s.read(4)
		if len(length) != 4:
			return (None, None)
		length = struct.unpack('>I', length)
		data = ''
		while True:
			newdata = self.s.read(length, timeout=timeout)
			if len(newdata) == 0:
				print "Incomplete packet received"
				time.sleep(1)
				self.s.flushInput()
				print "Flushed input buffer"
				return None
			length -= len(newdata)
			data += newdata
			if length == 0:
				break
		
		retcode = self.s.read(2, timeout=timeout)
		self.s.flushInput()
		return (data, retcode)
		

	def connect(self, port):
		""" Try to open the port and ping the device there
		"""
		try:
			self.s = serial.Serial(str(port), self.baud, timeout=1)
			self.s.flushInput()
			self.write_cmd("ping")
			(data, retcode) = self.read_response()
			
			if(data == "pong" and retcode == "OK"):
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
		self.write_cmd("listcmds")
		data = self.s.read(2000).split("\r\n")[:-2]
		commands = []
		for d in data:
			c = parse_cmd(d)
			if c:
				commands.append(c)
			else:
				print "Error parsing %s" % d
		return commands

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
	
	def exec_command(self, cmd):
		""" Execute a command and return the values
		"""
		pass