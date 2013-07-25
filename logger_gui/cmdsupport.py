# -*- coding: utf-8 -*-

from PySide.QtGui import *
from PySide.QtCore import *
import struct


class CmdSupport:
	# Undefined actions
	handle_result = None
	prompt = None


def get_hex(data, columns=8):
	text = ""
	for i in range(len(data)):
		if i % columns == 0:
			text += "\n%08X|" % i
		text += " %02X" % struct.unpack("B",data[i])[0]
	return text[1:]

class CMD_fs_read(CmdSupport):
	def __init__(self, cmd):
		self.cmd = cmd
	
	def handle_run(self, parent=None):
		pass

	def handle_result(self, parent=None):
		try:
			fname = self.cmd.returns[0].rline.text()
			with open(fname, 'r') as f:
				name = fname
		except:
			name = None
			
		# Concatenate all of the data!
		data = buffer("")
		text = ""
		data += self.cmd.returns[0].value
		text += "File Read: {}\n".format(self.cmd.params[0].value)
		text += "Requested {} bytes\n".format(self.cmd.params[1].value)
		text += "Received {} bytes\n".format(self.cmd.returns[0].len)
		if name:
			text += "Data Dumped to \"{}\"\n".format(name)
		else:
			text += "Data:\n"
			text += get_hex(data) + "\n"
		return text

class CMD_flash_read_sector(CmdSupport):
	sig = "buffer:data,buffer:spare(u:block,u:sector)"
	def __init__(self, cmd):
		self.cmd = cmd

	def handle_result(self, parent=None):
		self.block = self.cmd.params[0].value
		self.sector = self.cmd.params[1].value
		self.data = self.cmd.returns[0].value
		self.spare = self.cmd.returns[1].value

		#frame = QFrame(None, Qt.Popup | Qt.Window)
		#frame.resize(400,400)
		#textedit = QTextEdit(frame)
		#textedit.setPlainText("")

		text = "Read Sector:\n" + \
		"Block {}\n".format(self.block) + \
		"Sector {}\n".format(self.sector)

		if self.sector == 1:
			# This will indicate the type of block
			(block_type,) = struct.unpack("B",self.spare[:1])
			if block_type == 1:
				# This is inode
				(block_type, nothing, inode_index) = \
					struct.unpack("BBH",self.spare[:4])
				text += "Block Type: Inode\n"
				text += "Inode Index: {}\n".format(inode_index)
			elif block_type == 2:
				# This is a file header
				(block_type, nothing, n) = \
					struct.unpack("BBH",self.spare)
				(ts, age, file_id) = struct.unpack("<III",self.data[:12])
				(n,) = struct.unpack("<H",self.spare[2:4])
				text += "Block Type: File\n"
				text += "Timestamp: {}\n".format(ts)
				text += "Age: {}\n".format(age)
				text += "File ID: {}\n".format(file_id)
				text += "Num Bytes: {}\n".format(n)
				text += "File Data (header skipped):\n"
				text += get_hex(self.data[12:]) + "\n"
				text += "Spare:\n"
				text += get_hex(self.spare) + "\n"
			else:
				text += "Block Type: Unknown ({})\n".format(block_type)
				text += "Data:\n"
				text += get_hex(self.data) + "\n"
				text += "Spare:\n"
				text += get_hex(self.spare) + "\n"
		else:
			text += "Block Type: Unknown (this isn't header)\n"
			text += "Data:\n"
			text += get_hex(self.data) + "\n"
			text += "Spare:\n"
			text += get_hex(self.spare) + "\n"

		#textedit.setPlainText(text)

		#frame.show()
		return text
