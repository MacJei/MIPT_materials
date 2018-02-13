#!/usr/bin/env python3

import sys
for line in sys.stdin:
	line = line.split()
	print(line[1], line[0], sep='\t')
