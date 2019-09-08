CC 	= gcc
CFLAGS 	= -Wall -c -g

LIBS 	= -lclass
LIBDIR	= lib
INCLUDE	= include

PROJECT	= encode
TARGET 	= bin/$(PROJECT)
SOURCES = src/main.c src/input.c src/options.c src/text.c
SOURCES += src/encoder.c src/url-encoder.c src/b64-encoder.c
SOURCES += src/util.c
OBJECTS	= $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CC) -o $@ $(OBJECTS) -L$(LIBDIR) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $<

.PHONY: clean
clean: 
	rm -rf $(OBJECTS) $(TEST_OBJ) $(TARGET)
