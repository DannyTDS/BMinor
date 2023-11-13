#ifndef HELPER_H
#define HELPER_H

#include "scope.h"
#include "utils.h"
#include "token.h"
#include "encoder.h"
#include "decl.h"

/* External variables from compiled scanner.flex file */
extern FILE *yyin;
extern int yylex();
extern char *yytext;
/* Root node of the AST */
extern struct decl *root;

int typecheck();
int resolve(char* mode);
int pprint();
int parse(char* mode);
int scan(char* mode);
int encode(FILE* f);
int print_token(token_t token, char* yytext);

#endif