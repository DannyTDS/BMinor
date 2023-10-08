#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1

#define streq(a, b)         (strcmp(a, b) == 0)
#define error(M, ...) \
    fprintf(stderr, "[ERROR]    " M "\n", ##__VA_ARGS__)
#define info(M, ...) \
    fprintf(stderr, "[INFO]     " M "\n", ##__VA_ARGS__)
#define debug(M, ...) \
    fprintf(stderr, "[DEBUG]     " M "\n", ##__VA_ARGS__)

/* Compatible with Bison output header */
typedef enum yytokentype token_t;
#define TOKEN_EOF 0

#endif