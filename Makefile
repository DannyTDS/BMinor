CC=			gcc
CFLAGS=		-g -Wall -std=gnu99
AR=			ar
ARFLAGS=	rcs
LD=			gcc
LDFLAGS=	-L.
TARGET=		bminor

all: $(TARGET)

# Executable
bminor: bminor.o encoder.o scanner.o
	@echo Linking $@...
	@$(LD) $(LDFLAGS) -o $@ $^

# Main function
bminor.o:	bminor.c
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c -o $@ $^

# Scanner
scanner.o:	scanner_main.o scanner_rule.o
	@echo Linking $@...
	@$(AR) $(ARFLAGS) $@ $^

scanner_main.o: scanner.c
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c -o $@ $^

scanner_rule.o:	scanner_rule.c
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c -o $@ $^

scanner_rule.c:	scanner.flex
	@echo Compiling $@...
	@flex -o scanner_rule.c scanner.flex

# Encoder
encoder.o:	encoder.c
	@echo Compiling $@...
	@$(CC) $(CFLAGS) -c -o $@ $^

clean:
# Executable
	rm -f $(TARGET)
# Object files
	rm -f *.o
# Scanner - flex output
	rm -f scanner_rule.c
# Remove test output files
	rm -rf test/*/*.out

test:
	./runtest.sh

.PHONY: test