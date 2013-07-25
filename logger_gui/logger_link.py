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
		self.baud = 115200*8

	def write_cmd(self, command):
		self.s.flushInput()
		print "Sending command:"
		print command
		c = CmdParamString('', initial=str(command.name))
		c.to_stream(self.s)
		for p in command.params:
			p.to_stream(self.s)
		
	def get_response(self, command):
		for r in command.returns:
			r.from_stream(self.s)
		try:
			(retcode,) = struct.unpack("B",self.s.read(1))
			return retcode
		except:
			return None

	def validate_command(self, command):
		for p in command.params:
			if not p.validate():
				return False
		return True

	def run_command(self, command):
		self.write_cmd(command)
		retcode = self.get_response(command)
		for r in command.returns:
			print(r.name + ": " + str(r))
		return retcode

	def ping(self, s):
		cmd = Cmd()
		cmd.from_cmd_string("ping s(s:ping)")
		cmd.params[0].value = s
		cmd.to_stream(self.s)
		self.get_response(cmd)
		print("Command: ", cmd)
		print("Returns: ", cmd.returns)
		return cmd.returns[0].value

	def read_response(self):
		timeout = 0.5
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
			self.s = serial.Serial(str(port), timeout=0.50)
			self.s.flushInput()
			data = self.ping("pong")
			print("Data: ",data)
			if data == "pong":
				print("Ping succeeded")
				self.connected = True
				return True
			else:
				print("Ping failed!")

		except serial.SerialException as e:
			print "EXCEPTION"
			print e
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
		try:
			return response[0]
		except:
			return None


	def get_command_list(self):
		""" Query the device for available commands
		"""
		if not self.connected:
			return []

		self.write_cmd(Cmd("listcmds",[],[]))
		(num_commands,) = struct.unpack("B",self.s.read(1))
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