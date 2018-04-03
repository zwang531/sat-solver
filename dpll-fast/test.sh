#!/bin/bash
FILES=../petite/*.cnf
for filename in $FILES; do 
	echo $filename
	./sat* $filename 
done
