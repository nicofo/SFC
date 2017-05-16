#!/usr/bin/env python

import sys




# input comes from STDIN (standard input)
vol={}

for line in sys.stdin:
	# remove leading and trailing whitespace
	line = line.strip()
	# split the line into words
	words = line.split(',')
	
	year = words[0]
	origin = words[16]
	
	#cancelled = words[21] #En cas de ser 1 la casella 22 es el motiu
	#depDelay = words[15] # Retraso del Dep

	# convert count (currently a string) to int
	try:
		cancelled = int(words[21])
		depDelay = int(words[15])
	except ValueError:
		# count was not a number, so silently
		# ignore/discard this line
		continue
  
	if year != "Year":
		if not vol.has_key(year):
			vol[year]={}
			
		  
		if not vol[year].has_key(origin):
			vol[year][origin]={}
			vol[year][origin]['vols'] = 0
			vol[year][origin]['cancelled'] = 0
			vol[year][origin]['delay'] = 0
		
		if not cancelled != 0:
			vol[year][origin]['vols'] += 1
			vol[year][origin]['cancelled'] += cancelled
			vol[year][origin]['delay'] += depDelay
        

	#print  '%s,%s\t%s:%s'% (year, origin, cancelled, depDelay)
	
for year, dades in vol.items():
	for key, values in dades.items():
		print '%s,%s\t%s:%s'% (year, key, values['vols'], values['delay'])
		

