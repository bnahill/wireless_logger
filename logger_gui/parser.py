# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 07:00:22 2013

@author: ben
"""

from pyparsing import *

class CommandParser:
	""" A pyparsing-based command string parser
	
	Instantiate and parse
	"""
	ParseException = ParseException
	def __init__(self):
		lparen = Suppress("(")
		rparen = Suppress(")")
		lbrack = Suppress("[")
		rbrack = Suppress("]")
		colon  = Suppress(":")
		
		unnamed_param = Word(alphas + "_")
		named_param = Word(alphas + "_") + colon + Word(alphanums + "_")
		
		command_name = Word(alphanums + "_")
		
		out_param = Forward()
		out_param_array =  lbrack + Group(delimitedList(out_param)) + rbrack
		out_param << Or([unnamed_param, named_param, out_param_array])
		out_params = Group(Optional(delimitedList(out_param)))
		
		in_param = Forward()
		in_param_array =  lbrack + delimitedList(in_param) + rbrack
		in_param << Group(Or([named_param, in_param_array]))
		in_params = lparen + Group(Optional(delimitedList(in_param))) + rparen
		
		self.expr = command_name + out_params + in_params

	def parse(self, cmd):
		""" Parse a command string
		
		throws ParseException if there is an error
		"""
		return self.expr.parseString(cmd)


PARSER = CommandParser()

if __name__ == '__main__':
	##############################################
	# Just some test values
	##############################################
	TEST = list()
	TEST.append("test [s:name,s:format]()")
	TEST.append("get_date (datetime:date)")
	TEST.append("ping   [s]()")
	TEST.append("fetchbuffer u,logbuffer(s:buffer_name)")
	for t in TEST:
		print(PARSER.parse(t))