#!/usr/bin/env python3

import sys
word_prev = None
count = None
for line in sys.stdin:
	word_cur = line.split('\t')[0]
	if word_prev is None:
		word_prev = word_cur
		count = 1
		continue

	if word_prev != word_cur:
		print (count, word_prev)
		word_prev = word_cur
		count = 1
	else:
		count += 1

print (count, word_prev)