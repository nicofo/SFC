#!/bin/bash

mkdir tests
for i in {1..5}; do (time -p ./main < opcions10.txt) 2>> tests/tempsLlista10.txt; done


for i in {1..5}; do (time -p ./main < opcionsTot.txt) 2>> tests/tempsLlistaTot.txt; done


for i in {1..5}; do (time -p ./main < opcionsOrdenat.txt) 2>> tests/tempsLlistaOrdenada.txt; done

