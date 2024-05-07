CC=gcc
CFLAGS=-W -Wall -Werror -std=c99 -pedantic

SANITIZE=-g -fsanitize=address

CFILES=$(wildcard src/*.c)
OBJS=${CFILES:.c=.o}

JSONFILES=tests/*.json

CTEST=src/linked_lists.c tests/tests.c
TESTSOBJS=${CTEST:.c=.o}

all: json-parser
	./json-parser $(JSONFILES)

.PHONY:
json-parser: $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(OBJS) -o json-parser

clean:
	rm src/*.o
	rm json-parser

test: $(TESTSOBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(TESTSOBJS) -o run-tests
	./run-tests
