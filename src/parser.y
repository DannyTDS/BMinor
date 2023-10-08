/* Declaration of tokens to be included in parser.h, consumed by scanner.l */

/* Identifier and assignment */
%token TOKEN_IDENT
%token TOKEN_ASSIGN

/* Keywords */
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_VOID
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_WHILE
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_FUNC
%token TOKEN_PRINT
%token TOKEN_RETURN

/* Type keywords and literals */
%token TOKEN_INT
%token TOKEN_FLOAT
%token TOKEN_STR
%token TOKEN_CHAR
%token TOKEN_BOOL
%token TOKEN_INT_LIT
%token TOKEN_FLOAT_LIT
%token TOKEN_STR_LIT
%token TOKEN_CHAR_LIT

/* Logical operation */
%token TOKEN_NOT
%token TOKEN_AND
%token TOKEN_OR

/* Math operation */
%token TOKEN_EXP
%token TOKEN_MULT
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_ADD
%token TOKEN_SUB

/* Unary */
%token TOKEN_NEG
%token TOKEN_INCRE
%token TOKEN_DECRE
%token TOKEN_LT
%token TOKEN_LE
%token TOKEN_GT
%token TOKEN_GE
%token TOKEN_EQ
%token TOKEN_NE

/* Syntax */
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LBRACE
%token TOKEN_RBRACE
%token TOKEN_SEMI
%token TOKEN_COLON
%token TOKEN_COMMA

/* Error */
%token TOKEN_ERROR

%{
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"
#include "symbol.h"

#define YYSTYPE struct expr*

extern char* yytext;
extern int yylex();
extern int yyerror(char* str);

struct expr* result = 0;
%}

%%
program		:	expr TOKEN_SEMI			{ result = $1; return 0; }
			;

expr		:	expr TOKEN_ADD term		{ $$ = expr_create(EXPR_ADD, $1, $3); }
			|	expr TOKEN_SUB term		{ $$ = expr_create(EXPR_SUB, $1, $3); }
			|	term					{ $$ = $1; }
			;

term		:	term TOKEN_MULT factor	{ $$ = expr_create(EXPR_MUL, $1, $3); }
			|	term TOKEN_DIV factor	{ $$ = expr_create(EXPR_DIV, $1, $3); }
			|	factor					{ $$ = $1; }
			;

factor		:	TOKEN_LPAREN factor TOKEN_RPAREN	{ $$ = $2; }
			|	TOKEN_NEG factor		{ $$ = expr_create(EXPR_SUB, expr_create_value(0) ,$2); }
			|	TOKEN_INT				{ $$ = expr_create_integer_literal(atoi(yytext)); }
			;
%%

/* Called when parse error */
int yyerror( char *str )
{
	printf("Parse error: %s\n",str);
	return 0;
}