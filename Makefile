-include Makefile.rules

CC=gcc
CFILESBASE=src/base_*.c src/validator.c
CFILESRO=src/ro_*.c ro_main.c
CFILESRW=src/rw_*.c rw_main.c

LIBCFILES=*.c

TESTFILES=tests/*.c

TARGET=json-parser

JSONFILESDIR=files

NOPRINT=-DVALGRING_DISABLE_PRINT -Wno-unused-parameter

all: clean $(TARGET)
	./$(TARGET) ./$(JSONFILESDIR)/t.json

rw: clean
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) ./$(JSONFILESDIR)/t.json

# Makes the parse use stdint's 'least' types to use less memory, at the
# potential cost of performance
mem-least: clean
	$(CC) $(CFLAGS) -DLEAST $(CFILESBASE) $(CFILESRO) -o $(TARGET)
	./$(TARGET) ./$(JSONFILESDIR)/t.json

noprint: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRO) -o $(TARGET)
	./$(TARGET) ./$(JSONFILESDIR)/flights-1m.json

noprintrw: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRW) -o $(TARGET)
	./$(TARGET) ./$(JSONFILESDIR)/flights-1m.json

.PHONY:
$(TARGET):
	$(CC) $(CFLAGS) $(CFILESBASE) $(CFILESRO) -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi
	if [ -f "$(TARGET)-tests" ]; then rm $(TARGET)-tests; fi

# Testing
valgrind-compile: clean
	$(CC) $(CFLAGS) $(NOPRINT) $(CFILESBASE) $(CFILESRO) -o $(TARGET) -g

calgrind: valgrind-compile
	valgrind --tool=callgrind --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) ./$(JSONFILESDIR)/flights-1m.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) ./$(JSONFILESDIR)/big.json

valgrind-compile-rw: clean
	$(CC) $(CFLAGS)  $(CFILESBASE) $(CFILESRW) -o $(TARGET) -g

calgrind-rw: valgrind-compile-rw
	valgrind --tool=callgrind --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) ./$(JSONFILESDIR)/big.json

leaks-rw: valgrind-compile-rw
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) ./$(JSONFILESDIR)/big.json

check:
	$(CC) $(CFLAGS) -DNOVALIDATION src/*.c $(TESTFILES) -o json-parser-tests -lcriterion
	./json-parser-tests

# Publishing
release: clean
	$(CC) $(CFLAGS) -O2 $(CFILESBASE) $(CFILESRO) -o $(TARGET)

releaserw: clean
	$(CC) $(CFLAGS) -O2 $(CFILESBASE) $(CFILESRW) -o $(TARGET)

lib:
	cd src && $(CC) $(CFLAGS) $(LIBCFILES) -c -fpic
	cd src && $(CC) -shared -o lib$(TARGET).so *.o
	cd src && rm *.o
	cd src && mv lib$(TARGET).so ..

