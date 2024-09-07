-include Makefile.rules

CC=gcc
CFILES=src/lists/json_array.c \
	src/lists/linked_lists.c \
	src/json_api.c \
	src/json.c \
	src/main.c \
	src/parser.c \
	src/printing.c

all: clean json-parser
	./json-parser ../JsonParserCPP/big.json

.PHONY:
json-parser:
	$(CC) $(CFLAGS) $(CFILES) -o json-parser \
		-lm # math library for pow function

clean:
	if [ -f "json-parser" ]; then rm json-parser; fi
