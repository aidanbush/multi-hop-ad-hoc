SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -Wextra -D_TEST -g

SOURCE_DIR = $(CURDIR)/..
INCLUDES += -I $(SOURCE_DIR)
VPATH += $(SOURCE_DIR)

.PHONY: all clean

all: test test_router
	
test: packet.o routing_tables.o

test_router: routing_tables.o routing_tables.h

packet.o: packet.c packet.h routing_tables.h

routing_tables.o: routing_tables.c routing_tables.h 

clean:
	$(RM) test test_router *.o
