-include Makefile.rules

CC=gcc
CFILESBASE=src/base_*.c src/validator.c
CFILESRO=src/ro_*.c ro_main.c
CFILESRW=src/rw_*.c rw_main.c

TESTFILES=tests/*.c

TARGET=json-parser

all: clean $(TARGET)
	./$(TARGET) err.json

rw: clean
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) t.json

.PHONY:
$(TARGET):
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRO) -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi
	if [ -f "json-parser-tests" ]; then rm json-parser-tests; fi

valgrind-compile: clean
	$(CC) $(CFLAGS) \
		-DVALGRING_DISABLE_PRINT \
		$(CFILESBASE) $(CFILESRO) -o $(TARGET)

valgrind: valgrind-compile
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) big.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) t.json

check:
	$(CC) $(CFLAGS) src/*.c $(TESTFILES) -o json-parser-tests -lcriterion
	./json-parser-tests

