CC=gcc
CFLAGS=-W -Wall -Werror -std=c99 -pedantic

SANITIZE=-g -fsanitize=address

CFILES=$(wildcard src/*.c)
OBJS=${CFILES:.c=.o}

JSONFILES=tests/*.json

CTEST=src/linked_lists.c src/parser.c tests/tests.c
TESTSOBJS=${CTEST:.c=.o}

all: json-parser
	./json-parser $(JSONFILES)

.PHONY:
json-parser: $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(OBJS) -o json-parser

clean:
	rm src/*.o
	rm tests/*.o
	rm json-parser
	rm run-tests

test: $(TESTSOBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(TESTSOBJS) -o run-tests
	./run-tests
