#ifndef SCANNER_H
#define SCANNER_H

#include "utils.h"
#include "token.h"
#include "encoder.h"

/* External variables from compiled scanner.flex file */
extern FILE *yyin;
extern int yylex();
extern char *yytext;

int scan(FILE* input);
int print_token(token_t token, char* yytext);
int decode_yytext(token_t token, char* yytext);

#endif