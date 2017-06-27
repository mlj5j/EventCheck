#!/usr/bin/python

import linecache
import sys

if len(sys.argv) < 1:
	print "Please specify filename.txt"
filename = sys.argv[1]
print filename
nlines = 0
linenum = 0
text = 'DIGI found:'

with open(filename) as f:
	for line in f:
		nlines += 1
		if (line.find("kGood=0") >=  0):
    			print "Found bad event!"
			print linecache.getline(filename,nlines-6)
			print linecache.getline(filename,nlines-2)
 			s1 = linecache.getline(filename,nlines+17)
  			if (len(s1)==13):
			    for i in range(18,28):
				print linecache.getline(filename,nlines+i)



