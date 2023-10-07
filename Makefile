CC=			gcc
CFLAGS=		-g -std=gnu99 -Wall -Iinclude -fPIC
AR=			ar
ARFLAGS=	rcs
LD=			gcc
LDFLAGS=	-Llib

TARGET=		bminor
HEADERS=	$(wildcard include/*.h)
SOURCES=	$(wildcard src/*.c)
OBJECTS=	$(SOURCES:.c=.o) src/scanner.o

TOKENLIB=	token.h

all: $(TARGET)

$(TARGET):		$(OBJECTS)
	@echo Linking $@...
	@$(LD) $(LDFLAGS) -o $@ $^

%.o:			%.c
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

src/scanner.c:	src/scanner.flex
	@echo Compiling $@...
	@flex -o $@ $^

# src/parser.c:	src/parser.bison
# 	@echo Compiling $@...
# 	@bison --defines=$(TOKENLIB) -o parser.c -v $^

clean:
# Executable
	rm -f $(TARGET)
# Object files
	rm -f $(OBJECTS)
# Scanner - flex output
	rm -f src/scanner.c
# Parser - bison output
	rm -f src/parser.c
# Remove test output files
	rm -rf test/*/*.out

test: $(TARGET)
	./runtest.sh

.PHONY: test