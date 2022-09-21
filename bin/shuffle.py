#!/usr/bin/python

from sys import stdin, stdout
from random import shuffle 
import re

if __name__ == "__main__" :
	items = re.split("\s", stdin.readline())
	shuffle(items)
	print "".join(items)
	#stdout.writelines(stdin.readlines())
