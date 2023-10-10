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
%token TOKEN_WHILE				// Not implemented
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
%token TOKEN_ERROR				// For scanner emitting error only

%start program

%union {
	struct decl* decl;
	struct expr* expr;
	struct stmt* stmt;
	struct type* type;
	struct param_list* param_list;
};

%type <expr> expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 atomic opt_expr opt_expr_list opt_expr_block expr_list
%type <decl> decl decl_list init
%type <type> type
%type <stmt> stmt stmt_list stmt_block stmt_outer stmt_nested stmt_atomic
%type <param_list> param_list opt_param_list

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

struct decl* result = 0;
%}

%%
program		:	decl_list		{ } // { result = $1; return 0; }
			;

/* Declarations */
/* FIXME */
decl_list	:	decl decl_list			{ } // { $$ = $1; $1->next = $2; }
			|	/* Epsilon */			{ } // { $$ = 0; }
			;

decl		:	TOKEN_IDENT TOKEN_COLON type init TOKEN_SEMI			{ }		// Global variables, function declaration
			|	TOKEN_IDENT TOKEN_COLON type TOKEN_ASSIGN stmt_block	{ }		// Function definition
			;

init		:	TOKEN_ASSIGN opt_expr_block		{ }
			|	/* Epsilon */					{ } // { $$ = 0; }
			;

/* Statements */
stmt_list	:	stmt stmt_list			{ }
			|	/* Epsilon */			{ } // { $$ = 0; }
			;

stmt		:	stmt_outer			{ }
			|	stmt_nested			{ }
			;

stmt_outer	:	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_nested TOKEN_ELSE stmt_outer	{ }
			|	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_nested							{ }
			;

stmt_nested	:	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_nested TOKEN_ELSE stmt_nested	{ }			// Solving dangling else problem
			| 	TOKEN_FOR TOKEN_LPAREN opt_expr TOKEN_SEMI opt_expr TOKEN_SEMI opt_expr TOKEN_RPAREN stmt_nested	{ }
			|	stmt_atomic					{ }
			;

stmt_atomic	:	decl						{ }
			|	stmt_block					{ }
			|	expr TOKEN_SEMI				{ }
			|	TOKEN_RETURN opt_expr TOKEN_SEMI		{ }
			|	TOKEN_PRINT opt_expr_list TOKEN_SEMI	{ }
			;

stmt_block	:	TOKEN_LBRACE stmt_list TOKEN_RBRACE	{ }
			;

/* Expressions */
// What exactly do we mean by precedence?
expr		: 	expr1 TOKEN_ASSIGN expr	{ } // { $$ = expr_create(EXPR_ASSIGN, $1, $3); }	// Right associative
			|	expr1					{ } // { $$ = $1; }
			;

expr1		:	expr1 TOKEN_OR expr2	{ } // { $$ = expr_create(EXPR_OR, $1, $3); }
			|	expr2					{ } // { $$ = $1; }
			;

expr2		:	expr2 TOKEN_AND	expr3	{ } // { $$ = expr_create(EXPR_AND, $1, $3); }
			|	expr3					{ } // { $$ = $1; }
			;

expr3		:	expr3 TOKEN_LT expr4	{ } // { $$ = expr_create(EXPR_LT, $1, $3) ; }
			|	expr3 TOKEN_LE expr4	{ } // { $$ = expr_create(EXPR_LE, $1, $3) ; }
			|	expr3 TOKEN_GT expr4	{ } // { $$ = expr_create(EXPR_GT, $1, $3) ; }
			|	expr3 TOKEN_GE expr4	{ } // { $$ = expr_create(EXPR_GE, $1, $3) ; }
			|	expr3 TOKEN_EQ expr4	{ } // { $$ = expr_create(EXPR_EQ, $1, $3) ; }
			|	expr3 TOKEN_NE expr4	{ } // { $$ = expr_create(EXPR_NE, $1, $3) ; }
			| 	expr4					{ } // { $$ = $1; }
			;

expr4		:	expr4 TOKEN_ADD expr5	{ } // { $$ = expr_create(EXPR_ADD, $1, $3); }
			|	expr4 TOKEN_SUB expr5	{ } // { $$ = expr_create(EXPR_SUB, $1, $3); }
			|	expr5					{ } // { $$ = $1; }
			;

expr5		:	expr5 TOKEN_MULT expr6	{ } // { $$ = expr_create(EXPR_MUL, $1, $3); }
			|	expr5 TOKEN_DIV expr6	{ } // { $$ = expr_create(EXPR_DIV, $1, $3); }
			|	expr5 TOKEN_MOD expr6	{ } // { $$ = expr_create(EXPR_MOD, $1, $3); }
			|	expr6					{ } // { $$ = $1; }
			;

expr6		:	expr6 TOKEN_EXP expr7	{ } // { $$ = expr_create(EXPR_EXP, $1, $3); }
			|	expr7					{ } // { $$ = $1; }
			;

expr7		:	TOKEN_ADD expr8			{ } // { $$ = expr_create(EXPR_ADD, 0, $2); }		// Unary add (positive) sign
			|	TOKEN_SUB expr8			{ } // { $$ = expr_create(EXPR_SUB, 0, $2); }		// Unary sub (negative) sign
			|	TOKEN_NOT expr8			{ } // { $$ = expr_create(EXPR_NOT, $2, 0); }
			|	expr8					{ } // { $$ = $1; }
			;

expr8		:	TOKEN_LPAREN expr TOKEN_RPAREN	{ } // { $$ = $2; }	// Grouping
			|	expr8 TOKEN_LBRACKET expr TOKEN_RBRACKET	{ }				// Indexing
			|	TOKEN_IDENT TOKEN_LPAREN opt_expr_list TOKEN_RPAREN	{ }		// Function call
			|	expr8 TOKEN_INCRE		{ } // { $$ = expr_create(EXPR_INCRE, $1, 0); }
			|	expr8 TOKEN_DECRE		{ } // { $$ = expr_create(EXPR_DECRE, $1, 0); }
			|	atomic					{ } // { $$ = $1; }
			;

atomic		:	TOKEN_INT_LIT			{ } // { $$ = expr_create_integer_literal(strtol(yytext, NULL, 10)); }
			|	TOKEN_FLOAT_LIT			{ } // { $$ = expr_create_float_literal(strtod(yytext, NULL)); }
			|	TOKEN_CHAR_LIT			{ } // { $$ = expr_create_char_literal(*yytext); }
			|	TOKEN_STR_LIT			{ } // { $$ = expr_create_string_literal(yytext); }
			|	TOKEN_TRUE				{ } // { $$ = expr_create_boolean_literal(1); }
			|	TOKEN_FALSE				{ } // { $$ = expr_create_boolean_literal(0); }
			|	TOKEN_IDENT				{ } // { $$ = expr_create_name(yytext); }
			;

opt_expr	:	expr					{ } // { $$ = $1; }
			|	/* Epsilon */			{ } // { $$ = 0; }
			;

expr_list	:	expr TOKEN_COMMA expr_list		{ }
			|	expr							{ }
			;

opt_expr_list	:	expr_list			{ }
				|	/* Epsilon */		{ } // { $$ = 0; }
				;

opt_expr_block	:	TOKEN_LBRACE expr_list TOKEN_RBRACE	{ }		// Array initializer
				|	expr								{ }
				;

/* Types */
type		:	TOKEN_INT				{ } //  $$ = type_create(TYPE_INT); }
			|	TOKEN_FLOAT				{ } //  $$ = type_create(TYPE_FLOAT); }
			|	TOKEN_BOOL				{ } //  $$ = type_create(TYPE_BOOL); }
			|	TOKEN_CHAR				{ } //  $$ = type_create(TYPE_CHAR); }
			|	TOKEN_STR				{ } //  $$ = type_create(TYPE_STR); }
			|	TOKEN_AUTO				{ } //  $$ = type_create(TYPE_AUTO); }
			|	TOKEN_VOID				{ } //  $$ = type_create(TYPE_VOID); }
			|	TOKEN_ARRAY TOKEN_LBRACKET opt_expr TOKEN_RBRACKET type		{ } // { $$ = type_create_array($5, $3); }	// Do we permit array with empty length?
			|	TOKEN_FUNC type TOKEN_LPAREN opt_param_list TOKEN_RPAREN	{ }
			;

/* Params */
param_list	:	TOKEN_IDENT TOKEN_COLON type TOKEN_COMMA param_list		{ }
			|	TOKEN_IDENT TOKEN_COLON type							{ }
			;

opt_param_list	:	param_list			{ }
				|	/* Epsilon */		{ } // { $$ = 0; }
				;

%%

/* Called when parse error */
int yyerror( char *str )
{
	error("Parse error: %s",str);
	return 0;
}