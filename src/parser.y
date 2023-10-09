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

%start program

%union {
	struct decl* decl;
	struct expr* expr;
	struct stmt* stmt;
	struct type* type;
	struct param_list* param_list;
};

%type <expr> expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 atomic

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

extern char* yytext;
extern int yylex();
extern int yyerror(char* str);

struct expr* result = 0;
%}

%%
program		:	expr TOKEN_SEMI			{ result = $1; return 0; }
			;

/* Expressions */
// What exactly do we mean by precedence?
expr		: 	expr1 TOKEN_ASSIGN expr	{ $$ = expr_create(EXPR_ASSIGN, $1, $3); }
			|	expr1					{ $$ = $1; }
			;

expr1		:	expr1 TOKEN_OR expr2	{ $$ = expr_create(EXPR_OR, $1, $3); }
			|	expr2					{ $$ = $1; }
			;

expr2		:	expr2 TOKEN_AND	expr3	{ $$ = expr_create(EXPR_AND, $1, $3); }
			|	expr3					{ $$ = $1; }
			;

expr3		:	expr3 TOKEN_LT expr4	{ $$ = expr_create(EXPR_LT, $1, $3) ; }
			|	expr3 TOKEN_LE expr4	{ $$ = expr_create(EXPR_LE, $1, $3) ; }
			|	expr3 TOKEN_GT expr4	{ $$ = expr_create(EXPR_GT, $1, $3) ; }
			|	expr3 TOKEN_GE expr4	{ $$ = expr_create(EXPR_GE, $1, $3) ; }
			|	expr3 TOKEN_EQ expr4	{ $$ = expr_create(EXPR_EQ, $1, $3) ; }
			|	expr3 TOKEN_NE expr4	{ $$ = expr_create(EXPR_NE, $1, $3) ; }
			| 	expr4					{ $$ = $1; }
			;

expr4		:	expr4 TOKEN_ADD expr5	{ $$ = expr_create(EXPR_ADD, $1, $3); }
			|	expr4 TOKEN_SUB expr5	{ $$ = expr_create(EXPR_SUB, $1, $3); }
			|	expr5					{ $$ = $1; }
			;

expr5		:	expr5 TOKEN_MULT expr6	{ $$ = expr_create(EXPR_MUL, $1, $3); }
			|	expr5 TOKEN_DIV expr6	{ $$ = expr_create(EXPR_DIV, $1, $3); }
			|	expr5 TOKEN_MOD expr6	{ $$ = expr_create(EXPR_MOD, $1, $3); }
			|	expr6					{ $$ = $1; }
			;

expr6		:	expr6 TOKEN_EXP expr7	{ $$ = expr_create(EXPR_EXP, $1, $3); }
			|	expr7					{ $$ = $1; }
			;

expr7		:	TOKEN_LPAREN expr TOKEN_RPAREN	{ $$ = $2; }	// Grouping
			|	TOKEN_SUB atomic		{ $$ = expr_create(EXPR_NEG, $2, 0); }
			|	atomic TOKEN_INCRE		{ $$ = expr_create(EXPR_INCRE, $1, 0); }	// FIXME Will this make sense?
			|	atomic TOKEN_DECRE		{ $$ = expr_create(EXPR_DECRE, $1, 0); }
			|	TOKEN_NOT atomic		{ $$ = expr_create(EXPR_NOT, $2, 0); }
			|	atomic					{ $$ = $1; }
			;

atomic		:	TOKEN_INT_LIT			{ $$ = expr_create_integer_literal(strtol(yytext, NULL, 10)); }
			|	TOKEN_FLOAT_LIT			{ $$ = expr_create_float_literal(strtod(yytext, NULL)); }
			|	TOKEN_CHAR_LIT			{ $$ = expr_create_char_literal(*yytext); }
			|	TOKEN_STR_LIT			{ $$ = expr_create_string_literal(yytext); }
			|	TOKEN_TRUE				{ $$ = expr_create_boolean_literal(1); }
			|	TOKEN_FALSE				{ $$ = expr_create_boolean_literal(0); }
			|	TOKEN_IDENT				{ $$ = expr_create_name(yytext); }
			;
%%

/* Called when parse error */
int yyerror( char *str )
{
	error("Parse error: %s",str);
	return 0;
}