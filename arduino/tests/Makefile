CC=gcc
CFLAGS=-Wall -Wextra -D_TEST -g

.PHONY: all

all: test

test: packet.o

packet.o: packet.c packet.h

clean:
	$(RM) test *.o
