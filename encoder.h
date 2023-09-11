#ifndef ENCODER_H
#define ENCODER_H

#define MAXSTRLEN 255
#define MINASCII 32
#define MAXASCII 126

typedef struct {
    const char *sequence;
    char character;
} EscapeMap;

int string_decode(const char *es, char *s);
int string_encode(const char *s, char *es);

#endif