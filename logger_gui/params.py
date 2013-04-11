# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 10:07:44 2013

@author: ben
"""

from PySide.QtCore import *
from PySide.QtGui import *

import re
import datetime
import time
import struct
from parser import PARSER

def parse_cmd(cmd):
	""" Takes the full command definition string and parses to a tuple of the
	command name, input arguments, and return type
	"""

	print "Matching %s" % cmd
	#m = cmd_pattern.match(cmd)
	try:
		parsed = PARSER.parse(cmd)
	except PARSER.ParseException:
		print("Error in parsing", e)
		return None

	print("Parsed %s" % parsed)
	
	name = parsed[0]
	outparams = parsed[1]
	inparams = parsed[2]
	
	returns = []
	for p in outparams:
		param = mk_param(p)
		if param:
			returns.append(param)
	
	params = []
	for p in filter(bool, inparams):
		param = mk_param(p)
		if param:
			params.append(param)
	
	return (name, params, returns)
	
	
def mk_param(s):
	""" Parse a single argument into a CmdParam field
	"""
	
	# If it is an array, return it as an array of params
	if s and not isinstance(s[0], basestring):
		param = list()
		for c in s:
			param.append(mk_param(c))
		return param

	t = s[0]

	if len(s) > 1:
		name = s[1]
	else:
		name = ''
	
	cmd = None
	
	if t == "f":
		cmd = CmdParamReal(name, params=s[1:])
	elif t == "s":
		cmd = CmdParamString(name, params=s[1:])
	elif t == "logbuffer":
		pass
	elif t == "stream":
		pass
	elif t == "u":
		cmd = CmdParamInt(name, signed=False, params=s[1:])
	elif t == "i":
		cmd = CmdParamInt(name, signed=True, params=s[1:])
	elif t == "datetime":
		cmd = CmdParamDateTime(name, params=s[1:])
	elif t == "logbuffer":
		cmd = CmdParamLogBuffer(name, params=s[1:])
	
	return cmd

class Cmd:
	def __init__(self, name="", params=[], returns=[]):
		self.name = name
		self.params = params
		self.returns = returns

	def from_cmd_string(self, cmd_string):
		(name, params, returns) = parse_cmd(cmd_string)
		self.name = name
		self.params = params
		self.returns = returns

class CmdParam:
	def __init__(self, name, doc=""):
		self.name = name
		self.doc = doc
		self.widget = None
	def update_value(self):
		pass
	def __str__(self):
		self.update_value()
		return str(self.value)
		
class CmdArray:
	def __init__(self, fields=[]):
		pass
	def from_buffer(self, buff):
		pass

class CmdParamReal(CmdParam):
	def __init__(self, name, doc="", initial=0.0, params=None):
		CmdParam.__init__(self, name, doc)
		self.value = initial
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.value), parent)
		self.widget.setValidator(QDoubleValidator())
		return self.widget
	def validate(self):
		return self.widget.hasAcceptableInput()
	def update_value(self):
		pass
	def __str__(self):
		self.update_value()
		return str(self.value)


class CmdParamInt(CmdParam):
	def __init__(self, name, doc="", initial=0.0, signed=True, params=None):
		CmdParam.__init__(self, name, doc)
		self.value = initial
		self.signed = signed
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.value),parent)
		self.widget.setValidator(QIntValidator())
		return self.widget
	def validate(self):
		if not self.widget.hasAcceptableInput():
			return False
		return True
	def to_buffer(self):
		self.update_value()
		return struct.pack("<I",self.value)
		
	def update_value(self):
		if self.widget:
			self.value = int(self.widget.text())
		if self.signed:
			return struct.pack("<i",self.value)

	def __str__(self):
		self.update_value()
		return str(self.value)
		

class CmdParamString(CmdParam):
	def __init__(self, name, doc="", initial="", params=[]):
		CmdParam.__init__(self, name, doc)
		self.value = initial
		self.minlength = 0
		self.maxlength = None
		if len(params) > 1:
			self.minlength = params[1]
		if len(params) > 2:
			self.maxlength = params[2]
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.value),parent)
		if self.maxlength	:
			self.widget.setMaxLength(maxlength)
		return self.widget

	def validate(self):
		if not self.widget.hasAcceptableInput():
			return False
		l = len(self.widget.text())
		if l < self.minlength:
			return False
		return True
	
	def to_buffer(self):
		self.update_value()
		return struct.pack('%ds' % (len(self.value)+1), self.value)
		
	def update_value(self):
		if self.widget:
			self.value = str(self.widget.text())

	def __str__(self):
		self.update_value()
		return str(self.value)

class CmdParamDateTime(CmdParam):
	def __init__(self, name, doc="", timestamp=None, params=[]):
		CmdParam.__init__(self, name, doc)
		if timestamp:
			self.timestamp = timestamp
		else:
			t = datetime.datetime.now()
			self.timestamp = t.strftime("%y:%m:%d %H:%M:%S")

	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.timestamp),parent)
		self.widget.setMaxLength(17)
		return self.widget
		
	def validate(self):
		if self.widget.text().strip() == "":
			return True
		if not self.widget.hasAcceptableInput():
			return False
		if len(self.widget.text()) != 17:
			return False
		return True
	
	def to_buffer(self):
		self.update_value()
		return struct.pack("<%dsb" % len(self.timestamp), self.timestamp, 0)

	def update_value(self):
		if self.widget:
			if self.widget.text().strip() == "":
				t = datetime.datetime.now()
				self.timestamp = t.strftime("%y:%m:%d %H:%M:%S")
			else:
				self.timestamp = self.widget.text()
				
	def __str__(self):
		self.update_value()
		return str(self.value)


class CmdParamLogBuffer(CmdParam):
	def __init__(self, name, doc="", params=None):
		CmdParam.__init__(self, name, doc)
		self.value = None
	def make_widget(self, parent=None):
		pass
	def validate(self):
		return False
	def update_value(self):
		pass
	def __str__(self):
		self.update_value()
		return str(self.value)

class CmdParamOptions(CmdParam):
	pass