-include Makefile.rules

CC=gcc
CFILESBASE=src/base_*.c src/validator.c
CFILESRO=src/ro_*.c ro_main.c
CFILESRW=src/rw_*.c rw_main.c

TESTFILES=tests/*.c

TARGET=json-parser

NOPRINT=-DVALGRING_DISABLE_PRINT -Wno-unused-parameter

all: clean $(TARGET)
	./$(TARGET) t.json

rw: clean
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) t.json

# Makes the parse use stdint's 'least' types to use less memory, at the
# potential cost of performance
mem-least: clean
	$(CC) $(CFLAGS) -DLEAST $(CFILESBASE) $(CFILESRO) -o $(TARGET)
	./$(TARGET) t.json

noprint: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRO) -o $(TARGET)
	./$(TARGET) flights-1m.json

noprintrw: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) flights-1m.json

profile: clean
	$(CC) $(CFLAGS) -pg  $(CFILESBASE) $(CFILESRO) -o $(TARGET)
	./$(TARGET) flights-1m.json
	gprof -z $(TARGET)

profilerw: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) flights-1m.json
	gprof -z $(TARGET)

.PHONY:
$(TARGET):
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRO) -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi
	if [ -f "$(TARGET)-tests" ]; then rm $(TARGET)-tests; fi
	rm *.out

valgrind-compile: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRO) -o $(TARGET) -g

calgrind: valgrind-compile
	valgrind --tool=callgrind --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) flights-1m.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) big.json

check:
	$(CC) $(CFLAGS) -DSEPVALIDATION src/*.c $(TESTFILES) -o json-parser-tests -lcriterion
	./json-parser-tests

