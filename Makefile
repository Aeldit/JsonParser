-include Makefile.rules

CC=gcc
CFILES=src/linked_list.c \
	src/values_storage.c \
	src/main.c \
	src/json_api.c \
	src/printing.c \
	src/parser.c

all: clean json-parser
	./json-parser t.json

.PHONY:
json-parser:
	$(CC) $(CFLAGS) $(CFILES) -o json-parser \
		-lm # math library for pow function

clean:
	if [ -f "json-parser" ]; then rm json-parser; fi

valgrind: clean json-parser
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes \
		--collect-jumps=yes ./json-parser ../JsonParserCPP/big.json

leaks: clean json-parser
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./json-parser t.json
