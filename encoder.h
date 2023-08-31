#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1
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