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
from copy import copy
import cmdsupport

def parse_cmd(cmd):
	""" Takes the full command definition string and parses to a tuple of the
	command name, input arguments, and return type
	"""

	print "Matching %s" % cmd
	#m = cmd_pattern.match(cmd)
	try:
		parsed = PARSER.parse(cmd)
	except PARSER.ParseException:
		print("Error in parsing")
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
	if s and not isinstance(s, basestring):
		params = list()
		for c in s:
			params.append(mk_param(c))
		return CmdParamArray("", fields=params)

	s = s.split(":")
	t = s[0]

	if len(s) > 1:
		name = s[1]
	else:
		name = ''

	cmd = None

	kwparams = dict()
	for w in filter(lambda x: "=" in x, s[1:]):
		kwsplit = w.split('=')
		if len(kwsplit) == 2:
			kwparams[kwsplit[0]] = kwsplit[1]


	if t == "f":
		cmd = CmdParamReal(name, params=kwparams)
	elif t == "s":
		cmd = CmdParamString(name, params=kwparams)
	elif t == "logbuffer":
		pass
	elif t == "stream":
		pass
	elif t == "u":
		cmd = CmdParamInt(name, signed=False, params=kwparams)
	elif t == "i":
		cmd = CmdParamInt(name, signed=True, params=kwparams)
	elif t == "datetime":
		cmd = CmdParamDateTime(name, params=kwparams)
	elif t == "buffer":
		cmd = CmdParamBuffer(name, params=kwparams)
	elif t == "logbuffer":
		cmd = CmdParamLogBuffer(name, params=kwparams)

	return cmd

class Cmd:
	def __init__(self, name="", params=[], returns=[]):
		self.set_name(name)
		self.params = params
		self.returns = returns

	def set_name(self, name):
		self.name = name
		try:
			handlerclass = eval("cmdsupport.CMD_{}".format(name))
			self.handler = handlerclass(self)
		except AttributeError:
			self.handler = None

	def from_cmd_string(self, cmd_string):
		(name, params, returns) = parse_cmd(cmd_string)
		self.set_name(name)
		self.params = params
		self.returns = returns

	def to_buffer(self):
		buf = self.name + "\0"
		for p in filter(bool,self.params):
			buf += p.to_buffer()
		buf = struct.pack("I",len(buf)) + buf
		print("Send buffer:",buf)
		return buf

class CmdParam:
	def __init__(self, name, doc=""):
		self.name = name
		self.doc = doc
		self.widget = None
		self.value = None
	def update_value(self):
		pass
	def __str__(self):
		self.update_value()
		return str(self.value)

class CmdParamArray(CmdParam):
	def __init__(self, name, fields=[]):
		CmdParam.__init__(self, "Array", "")
		self.name = name
		self.fields = fields
		self.field_widgets = []
		# The actual expanded fields for receiving data
		self.expanded_fields = []
		# The number of array elements
		self.count = 0

	def make_widget(self, parent=None):
		self.widget = QWidget(parent)
		layout = QGridLayout(self.widget)
		self.field_widgets = []

		for (i,f) in zip(range(len(self.fields)),self.fields):
			l = QLabel(f.name, parent=self.widget)
			w = f.make_widget(self.widget)
			self.field_widgets.append(w)
			layout.addWidget(l, i, 0)
			layout.addWidget(w, i, 1)

		return self.widget

	def update_value(self):
		for f in self.fields:
			f.update_value()

	def validate(self):
		""" Validate *all* fields in serial
		"""
		valid = True
		for f in self.fields:
			valid = valid and f.validate()
		return valid

	def to_buffer(self):
		pass

	def from_buffer(self, buff):
		self.count = 0
		self.expanded_fields = []
		while(True):
			(count_in_frame,) = struct.unpack("B", buff[:1])
			self.count += count_in_frame
			buff = buff[1:]
			if count_in_frame == 0:
				return buff

			print("From_buffer: found %u entries" % count_in_frame)
			for i in range(count_in_frame):
				these_fields = []
				for f in self.fields:
					print("Buffer: " + str(buff))
					new_f = copy(f)
					buff = new_f.from_buffer(buff)
					print("Buffer after field:", buff)
					these_fields.append(new_f)
				self.expanded_fields.append(these_fields)

	def __str__(self):
		s = []
		for f in self.expanded_fields:
			for p in f:
				s.append(str(p))
		return "\n".join(s)

class CmdParamBuffer(CmdParam):
	def __init__(self, name, params={}):
		CmdParam.__init__(self, name, "")
		self.value = ""
		try:
			self.max_len = int(params['max'])
		except:
			self.max_len = 99999999999
		self.value = buffer("")
		self.widget = None

	def make_widget(self, parent=None):
		self.widget = QFrame(parent)
		self.layout = QHBoxLayout(self.widget)

		self.dropdown = QComboBox(self.widget)
		self.dropdown.addItems(["Hex","Decimal","Binary","String"])
		self.textbox = QTextEdit(self.widget)
		self.textbox.setFont(QFont("Courier", 9))

		self.layout.addWidget(self.dropdown)
		self.layout.addWidget(self.textbox)
		return self.widget

	def update_value(self):
		if not self.widget:
			return

		cur = self.dropdown.currentText()
		text = str(self.textbox.toPlainText())
		if cur in ["Hex","Decimal","Binary"]:
			text = filter(lambda x: x not in (" ", "\n", "\t"), text)

		print "Type is '%s'" % cur
		print "Text is '%s'" % text

		# Let's be lazy!
		if cur == "Decimal":
			text=hex(long(text))[:-1]
			cur = "Hex"

		if cur == "String":
			self.value = buffer(text)
		elif cur == "Hex":
			if len(text) % 2 == 1:
				text += '0'
			self.value = buffer("")
			for i in range(len(text) / 2):
				self.value += struct.pack("B",int(text[2*i:2*i+2],16))
		elif cur == "Binary":
			text += '0' * ((8 - (len(text) % 8)) % 8)
			self.value = buffer("")
			for i in range(len(text) / 8):
				self.value += struct.pack("B",int(text[8*i:8*i+8],2))
		print "Value:"
		print self.value

	def validate(self):
		try:
			self.update_value()
		except:
			return False
		if len(self.value) > self.max_len:
			return False
		return True

	def from_buffer(self, buff):
		# Use the first 4 bytes as length
		l = struct.unpack("<I", buff[:4])[0]
		print "Len: {}".format(l)
		self.value = copy(buff[4:4+l])
		print self.value
		return buff[4+l:]
	def to_buffer(self):
		self.update_value()
		b = buffer(struct.pack("<I", len(self.value))) + buffer(self.value)
		print "Returned buffer len {}".format(len(b))
		return b

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
	def from_buffer(self, buff):
		self.value = struct.unpack("<f", buff[:4])[0]
		return buff[4:]

class CmdParamInt(CmdParam):
	def __init__(self, name, doc="", initial=0, signed=True, params=None):
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
		s = "<i" if self.signed else "<I"
		return struct.pack(s,self.value)

	def from_buffer(self, buff):
		s = "<i" if self.signed else "<I"
		self.value = struct.unpack(s, buff[:4])[0]
		return buff[4:]

	def update_value(self):
		if self.widget:
			self.value = int(self.widget.text())

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
			self.widget.setMaxLength(self.maxlength)
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

	def from_buffer(self, buff):
		self.value = ""
		while True:
			assert len(buff)
			char = struct.unpack("s", buff[0])[0]
			buff = buff[1:]
			if char == "\0":
				break
			self.value += char
		return buff

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
				self.timestamp = str(t.strftime("%y:%m:%d %H:%M:%S"))
			else:
				self.timestamp = str(self.widget.text())

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