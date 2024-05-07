CC=gcc
CFLAGS=-W -Wall -Werror -std=c99 -pedantic

SANITIZE=-g -fsanitize=address

CFILES=src/*.c
OBJS=${CFILES:.c=.o}

JSONFILES=tests/*.json

all: json-parser
	./json-parser $(JSONFILES)

.PHONY:
json-parser: $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE) $(OBJS) -o json-parser

clean:
	rm src/*.o
	rm json-parser
