# Name: 		Gordon O'Neill
# File Name: 	Makefile
# Purpose: 		Mid level makefile, propogates commands down

SUB = CommThread TestCommThread

it clean install depend:
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "<<< $$i >>>"; cd $$X/$$i; make $@; done
	