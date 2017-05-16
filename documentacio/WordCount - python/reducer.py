#!/usr/bin/env python

import re
import sys

current_key = None
counts = {}

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parse the input we got from mapper.py
    word, count = line.split('\t', 1)

    # this IF-switch only works because Hadoop sorts map output
    # by key (here: word) before it is passed to the reducer
    if current_key == word:
        counts[current_key] = count
    else:
        # write result to STDOUT
        if current_key:
             print '%s\t%s' % (current_key, counts[current_key])
            
        current_key = word
        counts[current_key] = count

# write result of last word
print '%s\t%s' % (current_key, counts[current_key])

    
