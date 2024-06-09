# Makefile for crawler module
#
# Ben Pable - CS50 S24L = ../libcs50

L = ../libcs50

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L -I../common
PROG = crawler
OBJS = crawler.o
LIBS = -lm -lcurl
LLIBS = $L/libcs50-given.a ../common/common.a
MAKE = make

.PHONY: all clean

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -L$(L) -I$(L) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

# object files depend on include files
crawler.o: $L/bag.h $L/hashtable.h $L/webpage.h
	$(CC) $(CFLAGS) -I$(L) -c crawler.c

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM