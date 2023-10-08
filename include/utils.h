#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1

#define streq(a, b)         (strcmp(a, b) == 0)

/* Compatible with Bison output header */
typedef enum yytokentype token_t;

#endif