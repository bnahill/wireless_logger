# -*- coding: utf-8 -*-
"""
@author: Ben Nahill <bnahill@gmail.com>
"""
import sys

from PySide.QtCore import *
from PySide.QtGui import *

from logger_link import LoggerLink

class LoggerUI(QMainWindow):
	def __init__(self):
		QMainWindow.__init__(self)
		self.statusBar().showMessage("Ready")
		self.menuBar().addMenu("&File")
		self.toolbar = self.addToolBar('Exit')
		exitAction = QAction(QIcon('exit.png'), '&Exit', self)
		exitAction.triggered.connect(self.close)
		self.toolbar.addAction(exitAction)


if __name__=='__main__':
	app = QApplication(sys.argv)
	logger = LoggerUI()
	logger.resize(600,400)
	logger.show()
	sys.exit(app.exec_())

