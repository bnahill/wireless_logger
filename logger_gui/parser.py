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
		eq = Suppress("=")

		command_name = Word(alphanums + "_")
		kwargs_name = Word(alphanums + "_")
		kwargs_arg = Word(alphanums)

		unnamed_param = Word(alphas + "_")
		named_param = Combine(unnamed_param + ":" + command_name)
		param = Or([unnamed_param, named_param])
		kwarg = Combine(":" + kwargs_name + "=" + kwargs_arg)
		kwargs_param = Combine(param + ZeroOrMore(kwarg))

		out_param = Forward()
		out_param_array =  lbrack + Group(delimitedList(out_param)) + rbrack
		out_param << Or([param, out_param_array])
		out_params = Group(Optional(delimitedList(out_param)))

		in_param = Forward()
		in_param_array =  lbrack + delimitedList(in_param) + rbrack
		in_param << Or([kwargs_param, in_param_array])
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
	TEST.append("flash_read_sector buffer:data,buffer:spare(u:block:ml=5,u:sector)")
	TEST.append("flash_write_sector (u:block,u:sector,buffer:data:ml=512,buffer:spare:ml=4)")
	for t in TEST:
		print t
		print(PARSER.parse(t))