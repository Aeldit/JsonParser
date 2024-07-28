-include Makefile.rules

CFILES=src/lists/json_array.c \
	src/lists/linked_lists.c \
	src/json.c \
	src/parser.c \
	src/printing.c

OBJS=${CFILES:.c=.o}

JSONFILES=tests/*.json

all: json-parser
	./json-parser $(JSONFILES)

.PHONY:
json-parser: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o json-parser

lib:
	$(CC) $(CFLAGS) -c $(CFILES) -fpic
	$(CC) $(CFLAGS) *.o -shared -o libjson-parser.so
	rm *.o

clean:
	rm src/*.o
	rm -r src/lists/*.o
	rm json-parser
	rm *.so
