CC=gcc
CFLAGS=-W -Wall -Werror -std=c99 -pedantic

SANITIZE=-g -fsanitize=address

CFILES=src/lists/json_array.c \
	   src/lists/linked_lists.c \
	   src/json.c \
	   src/main.c \
	   src/parser2.c \
	   src/printing.c
OBJS=${CFILES:.c=.o}

JSONFILES=tests/*.json

all: json-parser
	./json-parser $(JSONFILES)

.PHONY:
json-parser: $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(OBJS) -o json-parser

clean:
	rm src/*.o
	rm -r src/lists/*.o
	rm $(TESTSOBJS)
	rm json-parser
