# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 10:07:44 2013

@author: ben
"""

from PySide.QtCore import *
from PySide.QtGui import *

import datetime

class CmdParam:
	def __init__(self, name, doc=""):
		self.name = name
		self.doc = doc
		self.widget = None

class CmdParamReal(CmdParam):
	def __init__(self, name, doc="", initial=0.0):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial), parent)
		self.widget.setValidator(QDoubleValidator())
		return self.widget


class CmdParamInt(CmdParam):
	def __init__(self, name, doc="", initial=0.0, bits=32):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
		self.bits = 32
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial),parent)
		self.widget.setValidator(QIntValidator())
		return self.widget


class CmdParamString(CmdParam):
	def __init__(self, name, doc="", initial="", maxlength=None, minlength=0):
		CmdParam.__init__(self, name, doc)
		self.initial = initial
		self.maxlength = maxlength
		self.minlength = minlength
	def make_widget(self, parent=None):
		self.widget = QLineEdit(unicode(self.initial),parent)
		if self.maxlength	:
			self.widget.setMaxLength(maxlength)
		return self.widget

class CmdParamDateTime(CmdParam):
	def __init__(self, name, doc, timestamp=None):
		CmdParam.__init__(self, name, doc)
		if timestamp:
			self.timestamp = timestamp
		else:
			self.timestamp = datetime.datetime.now()
	def make_widget(self, parent=None):
		return None

class CmdParamOptions(CmdParam):
	pass