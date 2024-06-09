# Makefile for common library
#
# Ben Pable - CS50 S24

OBJS = index.o pagedir.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb 
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS) 

# dependencies
word.o: word.h

index.o: word.h index.h
	$(CC) $(CFLAGS) -I../libcs50 -L../libcs50 -lcs50 -c index.c

pagedir.o: pagedir.h
	$(CC) $(CFLAGS) -I../libcs50 -L../libcs50 -lcs50 -c pagedir.c


.PHONY: clean sourcelist 

sourcelist: Makefile *.md *.c *.h
	@echo $^

clean:
	rm -f $(LIB) *~ *.o