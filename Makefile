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

# src/main.o:			src/main.c $(TOKENLIB)
# 	@echo "Compiling $@"
# 	@$(CC) $(CFLAGS) -c -o $@ $<

# src/helper.o:		src/helper.c $(TOKENLIB)
# 	@echo "Compiling $@"
# 	@$(CC) $(CFLAGS) -c -o $@ $<

# src/scanner.o:		src/scanner.c $(TOKENLIB)
# 	@echo "Compiling $@"
# 	@$(CC) $(CFLAGS) -c -o $@ $<

%.o:			%.c $(HEADERS)
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c -o $@ $<

src/scanner.c:	src/scanner.l
	@echo Compiling $@...
	@flex -o $@ $^

src/parser.c:	src/parser.y
	@echo Compiling $@...
	@bison --defines=$(TOKENLIB) -o $@ $^ --debug

$(TOKENLIB):	src/parser.c

clean:
# Executable
	@echo Removing $(TARGET) executable...
	@rm -f $(TARGET)
# Object files
	@echo Removing object files...
	@rm -f $(OBJECTS)
# Scanner - flex output
	@echo Removing scanner compiled output...
	@rm -f src/scanner.c
# Parser - bison output
	@echo Removing parser compiled output...
	@rm -f src/parser.c $(TOKENLIB) src/parser.output $(TOKENLIB:.h=.output)
# Remove test output files
	@echo Removing test file outputs...
	@rm -rf test/*/*.out
# Remove codegen assembly files
	@echo Removing codegen assembly files...
	@rm -rf test/codegen/*.s
# Remove codegen executables
	@echo Removing codegen executables...
	@rm -rf test/codegen/*.exe

test: $(TARGET)
	@echo Testing...
	@./runtest.sh

.PHONY: test