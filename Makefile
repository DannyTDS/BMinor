CC=			gcc
CFLAGS=		-g -Wall -std=gnu99
LD=			gcc
LDFLAGS=	-L.
TARGET=		bminor

all: $(TARGET)

bminor: bminor.o encoder.o scanner.o
	$(LD) $(LDFLAGS) -o $@ $^

scanner.o:	scanner.c
	$(CC) $(CFLAGS) -c -o $@ $^

scanner.c:	scanner.flex
	flex -o scanner.c scanner.flex

encoder.o:	encoder.c
	$(CC) $(CFLAGS) -c -o $@ $^

bminor.o:	bminor.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	# Executable
	rm -f $(TARGET)
	# Object files
	rm -f *.o
	# Scanner - flex output
	rm -f scanner.c

test:
	./runtest.sh

.PHONY: test