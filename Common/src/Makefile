# Name: 		Gordon O'Neill
# Course: 		System Development Unix Environment Fall 2018
# File Name: 	Makefile
# Purpose: 		Mid level makefile, propogates commands down

SUB = lib bin

it clean install depend:
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "<<< $$i >>>"; cd $$X/$$i; make $@; done
	