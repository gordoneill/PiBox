# Name: 		Gordon O'Neill
# File Name: 	Makefile
# Purpose: 		High level makefile, propogates commands down

SUB = src

it install depend:
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "<<< $$i >>>"; cd $$X/$$i; make $@; done

clean:
	rm -f bin/* lib/*
	-X=`pwd`; \
	for i in ${SUB}; \
	do echo "<<< $$i >>>"; cd $$X/$$i; make $@; done
	