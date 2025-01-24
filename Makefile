-include Makefile.rules

CC=gcc
CFILES=src/*.c

TESTFILES=tests/*.c

TARGET=json-parser

all: clean $(TARGET)
	./$(TARGET) t.json

rw: clean
	$(CC) $(CFLAGS) $(CFILES) rw_main.c -o $(TARGET)
	./$(TARGET) t.json

.PHONY:
$(TARGET):
	$(CC) $(CFLAGS) $(CFILES) ro_main.c -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi
	if [ -f "json-parser-tests" ]; then rm json-parser-tests; fi

valgrind-compile: clean
	$(CC) $(CFLAGS) \
		-DVALGRING_DISABLE_PRINT \
		$(CFILES) ro_main.c -o $(TARGET)

valgrind: valgrind-compile
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) big.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) t.json

check:
	$(CC) $(CFLAGS) $(CFILES) $(TESTFILES) -o json-parser-tests -lcriterion
	./json-parser-tests

