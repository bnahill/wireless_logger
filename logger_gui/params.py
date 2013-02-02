# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 10:07:44 2013

@author: ben
"""

from PySide.QtCore import *
from PySide.QtGui import *

import re
import datetime
import struct

cmd_pattern = re.compile(r"\A(?P<name>[a-zA-Z0-9_]*)\s*" +
	r"(?P<out>[a-zA-Z0-9_:,\[\]]*)\s*\((?P<in>[a-zA-Z0-9_:,\[\]]*)\)\s*")

def parse_cmd(cmd):
	""" Takes the full command definition string and parses to a tuple of the
	command name, input arguments, and return type
	"""
	m = cmd_pattern.match(cmd)
	if not m:
		print "Error " + cmd
		return None
	
	name = m.group("name")
	outparams = m.group("out").split(',')
	inparams = m.group("in").split(',')
	
	params = []
	for p in outparams:
		param = mk_param(p)
		if param:
			params.append(param)
	outparams = params		
	
	params = []
	for p in inparams:
		param = mk_param(p)
		if param:
			params.append(param)
	inparams = params
	
	return (name, outparams, inparams)
	
	
	
def mk_param(string):
	""" Parse a single argument into a CmdParam field
	"""
	split = string.split(':')
	t = split[0]

	if len(split) > 1:
		name = split[1]
	else:
		name = ''
	
	cmd = None
	
	if t == "f":
		cmd = CmdParamReal(name, params=split[1:])
	elif t == "s":
		cmd = CmdParamString(name, params=split[1:])
	elif t == "logbuffer":
		pass
	elif t == "u":
		cmd = CmdParamInt(name, signed=False, params=split[1:])
	elif t == "i":
		cmd = CmdParamInt(name, signed=True, params=split[1:])
	elif t == "datetime":
		cmd = CmdParamDateTime(name, params=split[1:])
	elif t == "logbuffer":
		cmd = CmdParamLogBuffer(name, params=split[1:])
	
	return cmd

class CmdParam:
	def __init__(self, name, doc=""):
		self.name = name
		self.doc = doc
		self.widget = None

class CmdParamReal(CmdParam):
	def __init__(self, name, doc="", initial=0.0, params=None):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial), parent)
		self.widget.setValidator(QDoubleValidator())
		return self.widget
	def validate(self):
		return self.widget.hasAcceptableInput()


class CmdParamInt(CmdParam):
	def __init__(self, name, doc="", initial=0.0, signed=True, params=None):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
		self.signed = signed
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial),parent)
		self.widget.setValidator(QIntValidator())
		return self.widget
	def validate(self):
		if not self.widget.hasAcceptableInput():
			return False
		return True
	def to_buffer(self):
		i = int(self.widget.text())
		if self.signed:
			return struct.pack(">i",i)
		return struct.pack(">I",i)
		

class CmdParamString(CmdParam):
	def __init__(self, name, doc="", initial="", params=None):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
		self.minlength = 0
		self.maxlength = None
		if len(params) > 1:
			self.minlength = params[1]
		if len(params) > 2:
			self.maxlength = params[2]
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial),parent)
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
		text = str(self.widget.text())
		return struct.pack('%ss' % (len(text)+1), text)

class CmdParamDateTime(CmdParam):
	def __init__(self, name, doc="", timestamp=None, params=None):
		CmdParam.__init__(self, name, doc)
		if timestamp:
			self.timestamp = timestamp
		else:
			self.timestamp = datetime.datetime.now()
	def make_widget(self, parent=None):
		return None
		
	def validate(self):
		return False

class CmdParamLogBuffer(CmdParam):
	def __init__(self, name, doc="", params=None):
		CmdParam.__init__(self, name, doc)
		
	def make_widget(self, parent=None):
		pass
	def validate(self):
		return False

class CmdParamOptions(CmdParam):
	pass