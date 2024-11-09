-include Makefile.rules

CC=gcc
CFILES=src/json_parser.c \
	src/json_printing.c \
	src/json_storage.c \
	src/json_write.c \
	src/main.c

all: clean json-parser
	./json-parser t.json

.PHONY:
json-parser:
	$(CC) $(CFLAGS) $(CFILES) -o json-parser \
		-lm # math library for pow function

clean:
	if [ -f "json-parser" ]; then rm json-parser; fi

valgrind-compile: clean
	$(CC) $(CFLAGS) -lm \
		-DVALGRING_DISABLE_PRINT \
		$(CFILES) -o json-parser

valgrind:  valgrind-compile
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes \
		--collect-jumps=yes ./json-parser big.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./json-parser t.json
