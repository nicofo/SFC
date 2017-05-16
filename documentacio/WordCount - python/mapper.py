#!/usr/bin/env python

import sys
import re

counts = {}

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        try:
            counts[word] += 1
        except:
            counts[word] = 1

for key in counts:
   print '%s\t%s' % (key, counts[key])

# write the results to STDOUT (standard output);
# what we output here will be the input for the
# Reduce step, i.e. the input for reducer.py
#
# tab-delimited; the trivial word count is 1
# print '%s\t%s' % (word, 1)
