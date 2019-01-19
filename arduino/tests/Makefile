CC=gcc
CFLAGS=-Wall -Wextra

.PHONY: all

all: test

test: packet.o

packet.o: packet.c packet.h

clean:
	$(RM) test *.o
