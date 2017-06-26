#!/usr/bin/python

import linecache
import sys

if len(sys.argv) < 1:
	print "Please specify filename.txt"
filename = sys.argv[1]
print filename
nlines = 0
linenum = 0
with open(filename) as f:
	for line in f:
		nlines += 1
		if (line.find("kGood=0") >=  0):
			print "Found bad event!"
			linenum = nlines - 6
			print linecache.getline(filename,linenum)
			print linecache.getline(filename,nlines-2)



