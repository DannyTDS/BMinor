#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SUCCESS 0
#define FAILURE 1

#define TRUE 1
#define FALSE 0

#define streq(a, b)         (strcmp(a, b) == 0)
/* TODO: Temporarily write all messages to stdout. To be compatible with typechecker error messages.
   If want to print to stderr, must rewrite all the printer functions for all structures. */
#define error(M, ...) \
    fprintf(stdout, "[ERROR]    " M "\n", ##__VA_ARGS__)
#define warn(M, ...) \
    fprintf(stdout, "[WARN]     " M "\n", ##__VA_ARGS__)
#define info(M, ...) \
    fprintf(stdout, "[INFO]     " M "\n", ##__VA_ARGS__)
#define debug(M, ...) \
    fprintf(stdout, "[DEBUG]    " M "\n", ##__VA_ARGS__)

/* Compatible with Bison output header */
typedef enum yytokentype token_t;

/* For name resolving and typechecking */
extern int resolve_error;
extern int typecheck_error;

#endif