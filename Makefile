CC=			gcc
CFLAGS=		-g -std=gnu99 -Wall -Iinclude
AR=			ar
ARFLAGS=	rcs
LD=			gcc
LDFLAGS=	-Llib

TARGET=		bminor
TOKENLIB=	include/token.h

HEADERS=	$(wildcard include/*.h) $(TOKENLIB)
SOURCES=	$(wildcard src/*.c)
OBJECTS=	$(SOURCES:.c=.o) src/scanner.o src/parser.o

all: $(TARGET)

$(TARGET):		$(OBJECTS)
	@echo Linking $@...
	@$(LD) $(LDFLAGS) -o $@ $^

%.o:			%.c $(HEADERS)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

src/scanner.c:	src/scanner.l
	@echo Compiling $@...
	@flex -o $@ $^

src/parser.c $(TOKENLIB):	src/parser.y
	@echo Compiling $@...
	@bison --defines=$(TOKENLIB) -o $@ $^ --debug

clean:
# Executable
	rm -f $(TARGET)
# Object files
	rm -f $(OBJECTS)
# Scanner - flex output
	rm -f src/scanner.c
# Parser - bison output
	rm -f src/parser.c $(TOKENLIB)
# Remove test output files
	rm -rf test/*/*.out

test: $(TARGET)
	@echo Testing...
	@./runtest.sh

.PHONY: test