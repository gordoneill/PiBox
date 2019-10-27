# Name:       Gordon O'Neill
# File Name:  Makefile
# Purpose:    Base level makefile, creates library BluetoothIF

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/lib
INCLDIR = $(PROJECT_ROOT)/include

SRCS = BluetoothIF.cpp
OBJS = $(SRCS:.cpp=.o)
PROJECT = libBluetoothIF.a
FLAGS = -I$(INCLDIR) -lbluetooth
CC = g++ $(FLAGS)

#Suffix Rule
.cpp.o:
	$(CC) -c $<
	
$(PROJECT): $(OBJS)
	ar -cr $(PROJECT) $(OBJS)

it: $(PROJECT)

install: $(LIBDIR)/$(PROJECT)
$(LIBDIR)/$(PROJECT): $(PROJECT)
	cp $(PROJECT) $(LIBDIR)	

clean:
	rm -f $(OBJS) $(PROJECT)

depend: $(SRCS)
	$(CC) -M $(SRCS) > dependList
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependList >> make.tmp
	mv make.tmp Makefile
	rm dependList

# DO NOT DELETE THIS LINE
