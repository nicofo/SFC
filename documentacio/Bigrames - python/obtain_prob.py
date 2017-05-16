from __future__ import division
import sys
import re

# argv[1] -> english model
# argv[2] -> catalan model

# Carreguem els bigrames en angles
with open(sys.argv[1], 'rb') as f:
	en_counts = dict([(l.lower().strip().split('\t')) for l in f.readlines()])

# Convertim els valors en float
en_counts = dict([(key, float(value)) for key,value in en_counts.iteritems()])

# Carreguem els bigrames en catala
with open(sys.argv[2], 'rb') as f:
	cat_counts = dict([(l.lower().strip().split('\t')) for l in f.readlines()])

# Convertim els valors en float
cat_counts = dict([(key, float(value)) for key,value in cat_counts.iteritems()])

# Creem la interseccio de les dues llistes
bigrams = dict([(key, [en_counts[key], cat_counts[key]]) for key in en_counts if key in cat_counts.keys()])

# creem una reg. exp. per a filtrar els bigrames amb caracters incorrectes
pattern = r'([a-zA-Z][a-zA-Z])'
regexp = re.compile(pattern)

'''
2on Examen Parcial TNUI 2014  
'''
# donada una paraula i un model, predim la llengua en que esta escrita
def predict(word,model):
	# accedim als contadors de bigrames del model
	bow = model
	
	# definim les probabilitats a priori
	prior_0 = 0.5
	prior_1 = 0.5 
	# calculem els bigrames de 'word'
	s = set()
	for i in range(len(word)-1):
		if len(regexp.findall(word[i:i+2])) > 0:
			s.add(word[i:i+2])
	# calculem la probabilitat del conjunt de bigrames de 'word'
	p_0 = 1.0
	p_1 = 1.0	
	for w in s:
		# calcul de P(bj | Li) fent un suavitzat Laplacia
		try:

			p_0 *= ((bow[w][0]) + 1.0)/(bow['total'][0]+1.0*(len(bow)))
			p_1 *= ((bow[w][1]) + 1.0)/(bow['total'][1]+1.0*(len(bow)))
		except KeyError, e:
			p_0 = 0.0000000001 
			p_1 = 0.0000000001
	p0 = prior_0 * p_0 
	p1 = prior_1 * p_1 
	
	# classifiquem segons el model "mes probable"
	if p0>p1:
		res = 'ENG'
	else:
		res= 'CAT'
	return [res,[p0,p1]]

# Demanem input a l'usuari
end = False
while not end:
	sentence = raw_input('Introdueix una frase, enter per a sortir:\n')
	if sentence == '': end = True
	else: print predict(sentence, bigrams)