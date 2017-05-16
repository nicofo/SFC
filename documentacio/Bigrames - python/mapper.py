#!/usr/bin/env python

import sys


import re
pattern = r'([a-zA-Z][a-zA-Z])'
regexp = re.compile(pattern)

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        for i in range(len(word)-1):
            if len(regexp.findall(word[i:i+2])) > 0:
                print '%s\t%s' % (word[i:i+2], 1)
                
        # write the results to STDOUT (standard output);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        # print '%s\t%s' % (word, 1)