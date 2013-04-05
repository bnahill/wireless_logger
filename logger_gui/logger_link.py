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
		
	def write_cmd(self, command):
		self.s.flushInput()
		print "Sending command:"
		print command
		c = CmdParamString('', initial=str(command.name))
		buf = c.to_buffer()
		for p in command.params:
			buf += p.to_buffer()
		buf = struct.pack("B%ss" % len(buf), len(buf), buf)
		print buf
		self.s.write(buf)
		
	def ping(self, s):
		c = struct.pack('b5s%ds' % (len(s)+1), 6+len(s), "ping", s)
		print c
		self.s.write(c)
	
	def read_response(self):
		length = self.s.read(4)
		if len(length) != 4:
			return (None, None)
		length = struct.unpack('<I', length)
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
			self.ping("pong")
			data = self.read_string_response()
			print data
			
			if(data == "pong"):
				self.connected = True
				return True
			
		except serial.SerialException:
			return False
		return False

	def set_current_time(self):
		cmd = Cmd("settime", [CmdParamDateTime('')])
		self.write_cmd(cmd)
		return self.read_return_code() == 0
	
	def read_string_response(self):
		s = ""
		while True:
			c = self.s.read(1)
			if not c:
				return None
			if ord(c[0]) == 0:
				return s
			s += c[0]
	def read_return_code(self):
		response = self.s.read(1)
		return response[0]
			
		
	def get_command_list(self):
		""" Query the device for available commands
		"""
		if not self.connected:
			return []
		
		self.write_cmd(Cmd("listcmds",[],[]))
		num_commands = struct.unpack("<I",self.s.read(4))[0]
		print "%d commands available" % num_commands
		commands = []		
		for i in range(num_commands):
			r = self.read_string_response()
			cmd = Cmd()
			cmd.from_cmd_string(r)
			if cmd:
				commands.append(cmd)
			else:
				print "Error parsing %s" % r
		
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