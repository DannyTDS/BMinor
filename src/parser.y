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
#include "utils.h"

#define YYSTYPE struct expr*

extern char* yytext;
extern int yylex();
extern int yyerror(char* str);

struct expr* result = 0;
%}

%start program

%union {
	struct decl* decl;
	struct expr* expr;
	struct stmt* stmt;
	struct type* type;
	struct param_list* param_list;
};

%type <expr> expr expr1 expr2 expr3

%%
program		:	expr TOKEN_SEMI			{ result = $1; return 0; }
			;

/* Expressions */

expr		: 	expr1 TOKEN_ASSIGN expr
			|	expr1
			;

expr1		:	expr1 TOKEN_ADD expr2	{ $$ = expr_create(EXPR_ADD, $1, $3); }
			|	expr1 TOKEN_SUB expr2	{ $$ = expr_create(EXPR_SUB, $1, $3); }
			|	expr2					{ $$ = $1; }
			;

expr2		:	expr2 TOKEN_MULT expr3	{ $$ = expr_create(EXPR_MUL, $1, $3); }
			|	expr2 TOKEN_DIV expr3	{ $$ = expr_create(EXPR_DIV, $1, $3); }
			|	expr3					{ $$ = $1; }
			;

expr3		:	factor
			;

factor		:	TOKEN_LPAREN factor TOKEN_RPAREN	{ $$ = $2; }
			|	TOKEN_NEG factor		{ $$ = expr_create(EXPR_SUB, expr_create_value(0) ,$2); }
			|	TOKEN_INT				{ $$ = expr_create_integer_literal(atoi(yytext)); }
			;
%%

/* Called when parse error */
int yyerror( char *str )
{
	error("Parse error: %s",str);
	return 0;
}