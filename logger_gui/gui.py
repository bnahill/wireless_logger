# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""
import sys

from PySide.QtCore import *
from PySide.QtGui import *

from logger_link import LoggerLink
from params import *

LL = LoggerLink()

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

		self.set_time_flag = QCheckBox("Set clock on connect?", self)
		self.set_time_flag.setCheckState(Qt.CheckState.Checked)

		self.port_list = PortList(self)
		self.port_list.click.connect(self.update_port_list)

		self.toolbar = self.addToolBar('Exit')
		self.toolbar.addAction(exitAction)
		self.toolbar.addAction(self.connectAction)
		self.toolbar.addWidget(self.port_list)
		self.toolbar.addWidget(self.set_time_flag)
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

		self.console = Console(self)
		self.console.setReadOnly(True)
		self.hbox.addWidget(self.console)

		self.action_list.update()

	def handle_action(self, item):
		pass


	def update_port_list(self):
		l = LL.list_ports()
		self.port_list.clear()
		self.port_list.addItems(l)


	def connect_disconnect(self):
		""" Either connect or disconnect using the port specified
		"""
		if LL.connected:
			LL.disconnect()
			self.on_disconnect()
			self.status("Disconnected")
		else:
			port = self.port_list.currentText()
			if "" == port:
				self.statusBar().showMessage("No port selected")
				return

			self.status("Connecting to port %s..." % port)

			if LL.connect(port):
				self.on_connect(port)
			else:
				self.on_disconnect()
				self.status("Error connecting to port %s" % port)

	def on_connect(self, port):
		self.connectAction.setIcon(
			QIcon('icons/network-connect-3.png')
		)
		self.status("Connected to port %s" % port)
		if self.set_time_flag.checkState() == Qt.CheckState.Checked:
			LL.set_current_time()
		self.action_list.enable()
		self.action_list.update()

	def on_disconnect(self):
		self.connectAction.setIcon(
			QIcon('icons/network-disconnect-3.png')
		)
		self.action_list.disable()

	def status(self, msg):
		""" Update the status bar"""
		self.statusBar().showMessage(msg)


class Console(QTextEdit):
	def __init__(self, parent=None):
		QTextEdit.__init__(self, parent)
		self.setFont(QFont("Courier", 9))

	def clear(self):
		self.setPlainText("")

	def write_text(self, text):
		self.setPlainText(self.toPlainText() + text + "\n")

class ActionList(QListWidget):
	""" This is that left column listing available actions
	"""
	launch = Signal()

	def __init__(self, parent=None):
		QListWidget.__init__(self,parent)
		self.itemDoubleClicked.connect(self.select_item)
		self.items = []

	def disable(self):
		self.setDisabled(True)

	def enable(self):
		self.setDisabled(False)

	def update(self):
		self.items = LL.get_command_list()
		self.clear()

		for i in self.items:
			item = ActionListItem(i, self)

	def select_item(self, item):
		command = item.command
		dialog = ParamDialog(command)
		layout = QGridLayout(dialog)

		cmdlabel = QLabel("<b>Command: %s</b>" % command.name, parent=dialog)
		cmdlabel.setAlignment(Qt.AlignHCenter)
		layout.addWidget(cmdlabel,0,0,1,2)

		i = 1
		for param in command.params:
			w = param.make_widget(dialog)
			if not w:
				continue
			l = QLabel(param.name, parent=dialog)
			layout.addWidget(l, i, 0)
			layout.addWidget(w, i, 1)
			i += 1

		execbutton = QPushButton("Execute", dialog)
		cancelbutton = QPushButton("Cancel", dialog)

		def cancel():
			dialog.done(1)

		def execute():
			error_fields = []
			for param in command.params:
				if not param.validate():
					error_fields.append(param.name)
			if len(error_fields):
				msg = QMessageBox(dialog)
				msg.setText("<b>The following fields are invalid</b>:" +
					"<br />" +	"<br/>".join(error_fields))
				msg.setVisible(True)
				return
			# TODO: Let the command itself do execution so it may make
			# use of support hooks
			retcode = LL.run_command(command)
			logger.console.write_text("Executing %s:" % command.name)
			if command.handler and command.handler.handle_result:
				logger.console.write_text(
					str(command.handler.handle_result()) + "\n"
				)
			else:
				logger.console.write_text(
					"\n".join(map(str,command.returns)) + "\n"
				)
			logger.console.write_text(
				"Return Code: {}".format(str(retcode))
			)
			bar = logger.console.verticalScrollBar()
			bar.setValue(bar.maximum())

			dialog.done(0)

		cancelbutton.pressed.connect(cancel)
		execbutton.pressed.connect(execute)

		group = QButtonGroup(dialog)
		group.addButton(execbutton)
		group.addButton(cancelbutton)

		layout.addWidget(execbutton, i, 1)
		layout.addWidget(cancelbutton, i, 0)
		dialog.setLayout(layout)
		dialog.exec_()

class ActionListItem(QListWidgetItem):
	def __init__(self, command, parent=None):
		self.command = command
		QListWidgetItem.__init__(self, command.name, parent)


class ParamDialog(QDialog):
	def __init__(self, item, parent=None):
		QDialog.__init__(self,parent)
		self.setModal(True)


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

logger = None

if __name__=='__main__':
	app = QApplication(sys.argv)
	logger = LoggerUI()
	logger.resize(600,400)
	logger.show()
	sys.exit(app.exec_())

