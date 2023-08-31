CC=			gcc
CFLAGS=		-g -Wall -std=gnu99
LD=			gcc
LDFLAGS=	-L.
TARGET=		bminor

all: $(TARGET)

bminor: bminor.o encoder.o
	$(LD) $(LDFLAGS) -o $@ $^

encoder.o:	encoder.c
	$(CC) $(CFLAGS) -c -o $@ $^

bminor.o:	bminor.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) *.o

test:
	./runtest.sh

.PHONY: test