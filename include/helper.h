#ifndef HELPER_H
#define HELPER_H

#include "utils.h"
#include "token.h"
#include "encoder.h"

/* External variables from compiled scanner.flex file */
extern FILE *yyin;
extern int yylex();
extern char *yytext;

int parse();
int scan(char* mode);
int print_token(token_t token, char* yytext);
int encode(FILE* f);

#endif