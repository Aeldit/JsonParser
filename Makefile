-include Makefile.rules

CC=gcc
CFILES=src/json_parser.c \
	src/json_printing.c \
	src/json_storage.c \
	src/json_write.c \
	src/main.c

TARGET=json-parser

all: clean $(TARGET)
	./$(TARGET) t.json

.PHONY:
$(TARGET):
	$(CC) $(CFLAGS) $(CFILES) -o $(TARGET) \
		-lm # math library for pow function

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi

valgrind-compile: clean
	$(CC) $(CFLAGS) -lm \
		-DVALGRING_DISABLE_PRINT \
		$(CFILES) -o $(TARGET)

valgrind:  valgrind-compile
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes \
		--collect-jumps=yes ./$(TARGET) big.json

leaks: valgrind-compile
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes ./$(TARGET) t.json

