# Name:       Gordon O'Neill
# File Name:  Makefile
# Purpose:    Base level makefile, creates executable for CommThreadTest

PROJECT_ROOT = ../../..
LIBDIR = $(PROJECT_ROOT)/lib/
BINDIR = $(PROJECT_ROOT)/bin/
INCLDIR = $(PROJECT_ROOT)/include/

SRCS = CommThreadTest.cpp
LIBS = $(LIBDIR)/libLogMgr.a $(LIBDIR)/libEncDec.a
OBJS = $(SRCS:.cpp=.o)
PROJECT = CommThreadTest
FLAGS = -I$(INCLDIR) -lrt
CC = g++ $(FLAGS)

#Suffix Rule
.cpp.o:
	$(CC) -c $<

$(PROJECT): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

it: $(PROJECT)

install: $(BINDIR)/$(PROJECT)
$(BINDIR)/$(PROJECT): $(PROJECT)
	cp $(PROJECT) $(BINDIR)	

clean:
	rm -f $(OBJS) $(PROJECT)

depend: $(SRCS)
	$(CC) -M $(SRCS) > dependList
	sed -e '1,/^# DO NOT DELETE/!d' Makefile > make.tmp
	cat dependList >> make.tmp
	mv make.tmp Makefile
	rm dependList

# DO NOT DELETE THIS LINE
