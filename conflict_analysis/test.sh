#!/bin/bash
FILES=../petite/*.cnf
for filename in $FILES; do 
	echo $filename
	./sat* $filename --verbose --time 10
done
