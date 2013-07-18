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
			text += "\n%04X|" % (i * columns)
		text += " %02X" % struct.unpack("B",data[i])[0]
	return text[1:]

class CMD_flash_read_sector(CmdSupport):
	sig = "buffer:data,buffer:spare(u:block,u:sector)"
	def __init__(self, cmd):
		self.cmd = cmd

	def handle_result(self):
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

		if self.sector == 0:
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
				(age, file_id) = struct.unpack("II",self.data[:8])
				text += "Block Type: File\n"
				text += "Age: {}\n".format(age)
				text += "File ID: {}\n".format(file_id)
				text += "File Data:\n"
				text += get_hex(self.data[8:]) + "\n"
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
