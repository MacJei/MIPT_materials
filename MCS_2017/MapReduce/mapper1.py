#!/usr/bin/env python3

import sys
import string
traslator = str.maketrans('', '', string.puctuation)

for line in sys.stdin:
	all_words = set()
	words = line.split()
	id = words[0]
	for word in words[1:]:
		word = word.translate(traslator)
		all_words.add(word)
	for word in all_words:
		print(word, id, sep='\t')
