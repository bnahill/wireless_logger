# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""
import sys

from PySide.QtCore import *
from PySide.QtGui import *

from logger_link import LoggerLink

ll = LoggerLink()

class LoggerUI(QMainWindow):
	def __init__(self):
		QMainWindow.__init__(self)
		self.setWindowTitle("Logger GUI")
		
		self.setup_layout()
		
		self.setup_menubar()
		self.setup_toolbar()
	
	def setup_menubar(self):
		self.menuBar().addMenu("&File")
		
	def setup_toolbar(self):
		exitAction = QAction(
			QIcon('icons/application-exit-4.png'),
			'&Exit', self
		)
		exitAction.triggered.connect(self.close)
		
		self.connectAction = QAction(
			QIcon('icons/network-disconnect-3.png'),
			'&Connect', self
		)
		self.connectAction.triggered.connect(self.connect_disconnect)
		
		self.port_list = PortList(self)
		self.port_list.click.connect(self.update_port_list)
		
		self.toolbar = self.addToolBar('Exit')
		self.toolbar.addAction(exitAction)
		self.toolbar.addAction(self.connectAction)
		self.toolbar.addWidget(self.port_list)
		self.update_port_list()
	
	def setup_layout(self):
		self.mainbox = QWidget(self)
		self.setCentralWidget(self.mainbox)
		
		self.hbox = QHBoxLayout()
		self.mainbox.setLayout(self.hbox)
		
		self.action_list = ActionList(self)
		self.action_list.setMaximumWidth(200)
		self.action_list.disable()
		
		self.hbox.addWidget(self.action_list)
		
		self.console = QTextEdit(self)
		self.console.setReadOnly(True)
		self.hbox.addWidget(self.console)
		
		self.action_list.update()
	
	def handle_action(self, item):
		pass
		
			
	def update_port_list(self):
		l = ll.list_ports()
		self.port_list.clear()
		self.port_list.addItems(l)
		
	
	def connect_disconnect(self):
		""" Either connect or disconnect using the port specified
		"""
		if ll.connected:
			ll.disconnect()
			self.on_disconnect()
			self.status("Disconnected")
		else:
			port = self.port_list.currentText()
			if "" == port:
				self.statusBar().showMessage("No port selected")
				return

			self.status("Connecting to port %s..." % port)

			if ll.connect(port):
				self.on_connect(port)

			else:
				self.on_disconnect()
				self.status("Error connecting to port %s" % port)

	def on_connect(self, port):
		self.connectAction.setIcon(
			QIcon('icons/network-connect-3.png')
		)
		self.status("Connected to port %s" % port)
		self.action_list.enable()
		self.action_list.update()
	
	def on_disconnect(self):
		self.connectAction.setIcon(
			QIcon('icons/network-disconnect-3.png')
		)
		self.action_list.clear()
		self.action_list.disable()
		
	def status(self, msg):
		""" Update the status bar"""
		self.statusBar().showMessage(msg)

class ActionList(QListWidget):
	launch = Signal()
	
	def disable(self):
		self.setDisabled(True)
	
	def enable(self):
		self.setDisabled(False)
	
	def update(self):
		self.clear()
		self.addItems(ll.get_command_list())
	

class PortList(QComboBox):
	""" Just a combobox capable of emitting a signal to update its entries
	whenever it is selected
	"""
	click = Signal()
	def __init__(self,parent=None):
		QComboBox.__init__(self,parent)
		self.setMinimumContentsLength(18)
	
	def mousePressEvent(self, evt):
		self.click.emit()
		return QComboBox.mousePressEvent(self,evt)



if __name__=='__main__':
	app = QApplication(sys.argv)
	logger = LoggerUI()
	logger.resize(600,400)
	logger.show()
	sys.exit(app.exec_())

