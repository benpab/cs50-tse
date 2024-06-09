# Makefile for querier
#
# Ben Pable
# CS5 - S24

# object files
LIBS = ../common/common.a ../libcs50/libcs50-given.a
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make

.PHONY: clean querier test all

all: querier

#dependencies
querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

# indextest: indextest.o $(LIBS)
#	$(CC) $(CFLAGS) $^ -o $@

querier.o: ../libcs50/file.h ../common/index.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/counters.h

#indextest.o: ../common/index.h 

test: 
	chmod +x test.sh
	./test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f querier
	rm -rf data/