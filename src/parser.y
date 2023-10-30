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
%token TOKEN_EOF 0

%start program

%union {
	struct decl* decl;
	struct expr* expr;
	struct stmt* stmt;
	struct type* type;
	struct param_list* param_list;
};

%type <expr> expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 atomic opt_expr opt_expr_list opt_expr_block_list expr_block expr_block_list expr_list id
%type <decl> decl decl_list program
%type <type> type
%type <stmt> stmt stmt_list stmt_block stmt_outer stmt_nested stmt_atomic
%type <param_list> param_list opt_param_list

%define parse.error verbose

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
extern int yyerror(const char* str);

struct decl* root = 0;
%}

%%
program		:	decl_list TOKEN_EOF		{ root = $1; return 0; }
			;

/* Declarations */
decl_list	:	decl decl_list			{ $$ = $1; $1->next = $2; }
			|	/* Epsilon */			{ $$ = 0; }
			;

decl		:	id TOKEN_COLON type TOKEN_ASSIGN opt_expr_block_list TOKEN_SEMI			{ $$ = decl_create($1->name, $3, $5, 0, 0); }		// Global variables
			|	id TOKEN_COLON type TOKEN_SEMI				{ $$ = decl_create($1->name, $3, 0, 0, 0); }		// Function declaration
			|	id TOKEN_COLON type TOKEN_ASSIGN stmt_block	{ $$ = decl_create($1->name, $3, 0, $5, 0); }		// Function definition
			;

/* Statements */
stmt_list	:	stmt stmt_list			{ $$ = $1; $1->next = $2; }
			|	/* Epsilon */			{ $$ = 0; }
			;

stmt		:	stmt_outer			{ $$ = $1; }
			|	stmt_nested			{ $$ = $1; }
			;

stmt_outer	:	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_nested TOKEN_ELSE stmt_outer	{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
			|	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt								{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, 0, 0); }
			| 	TOKEN_FOR TOKEN_LPAREN opt_expr TOKEN_SEMI opt_expr TOKEN_SEMI opt_expr TOKEN_RPAREN stmt_outer		{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
			;

stmt_nested	:	TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_nested TOKEN_ELSE stmt_nested	{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }			// Solving dangling else problem
			| 	TOKEN_FOR TOKEN_LPAREN opt_expr TOKEN_SEMI opt_expr TOKEN_SEMI opt_expr TOKEN_RPAREN stmt_nested	{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
			|	stmt_atomic					{ $$ = $1; }
			;

stmt_atomic	:	decl						{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
			|	stmt_block					{ $$ = $1; }
			|	expr TOKEN_SEMI				{ $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0); }
			|	TOKEN_RETURN opt_expr TOKEN_SEMI		{ $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0, 0, 0); }
			|	TOKEN_PRINT opt_expr_list TOKEN_SEMI	{ $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0, 0); }
			;

stmt_block	:	TOKEN_LBRACE stmt_list TOKEN_RBRACE	{ $$ = stmt_create(STMT_BLOCK, 0, 0, 0, 0, $2, 0, 0); }
			;

/* Expressions */
expr		: 	expr1 TOKEN_ASSIGN expr	{ $$ = expr_create(EXPR_ASSIGN, $1, $3, 0); }	// Right associative
			|	expr1					{ $$ = $1; }
			;

expr1		:	expr1 TOKEN_OR expr2	{ $$ = expr_create(EXPR_OR, $1, $3, 1); }
			|	expr2					{ $$ = $1; }
			;

expr2		:	expr2 TOKEN_AND	expr3	{ $$ = expr_create(EXPR_AND, $1, $3, 2); }
			|	expr3					{ $$ = $1; }
			;

expr3		:	expr3 TOKEN_LT expr4	{ $$ = expr_create(EXPR_LT, $1, $3, 3) ; }
			|	expr3 TOKEN_LE expr4	{ $$ = expr_create(EXPR_LE, $1, $3, 3) ; }
			|	expr3 TOKEN_GT expr4	{ $$ = expr_create(EXPR_GT, $1, $3, 3) ; }
			|	expr3 TOKEN_GE expr4	{ $$ = expr_create(EXPR_GE, $1, $3, 3) ; }
			|	expr3 TOKEN_EQ expr4	{ $$ = expr_create(EXPR_EQ, $1, $3, 3) ; }
			|	expr3 TOKEN_NE expr4	{ $$ = expr_create(EXPR_NE, $1, $3, 3) ; }
			| 	expr4					{ $$ = $1; }
			;

expr4		:	expr4 TOKEN_ADD expr5	{ $$ = expr_create(EXPR_ADD, $1, $3, 4); }
			|	expr4 TOKEN_SUB expr5	{ $$ = expr_create(EXPR_SUB, $1, $3, 4); }
			|	expr5					{ $$ = $1; }
			;

expr5		:	expr5 TOKEN_MULT expr6	{ $$ = expr_create(EXPR_MUL, $1, $3, 5); }
			|	expr5 TOKEN_DIV expr6	{ $$ = expr_create(EXPR_DIV, $1, $3, 5); }
			|	expr5 TOKEN_MOD expr6	{ $$ = expr_create(EXPR_MOD, $1, $3, 5); }
			|	expr6					{ $$ = $1; }
			;

expr6		:	expr6 TOKEN_EXP expr7	{ $$ = expr_create(EXPR_EXP, $1, $3, 6); }
			|	expr7					{ $$ = $1; }
			;

expr7		:	TOKEN_ADD expr8			{ $$ = expr_create(EXPR_ADD, 0, $2, 7); }		// Unary add (positive) sign
			|	TOKEN_SUB expr8			{ $$ = expr_create(EXPR_SUB, 0, $2, 7); }		// Unary sub (negative) sign
			|	TOKEN_NOT expr8			{ $$ = expr_create(EXPR_NOT, 0, $2, 7); }
			|	expr8					{ $$ = $1; }
			;

expr8		:	TOKEN_LPAREN expr TOKEN_RPAREN						{ $$ = $2; }	// Grouping
			|	expr8 TOKEN_LBRACKET expr TOKEN_RBRACKET			{ $$ = expr_create(EXPR_INDEX, $1, $3, 8); }				// Indexing
			|	id TOKEN_LPAREN opt_expr_list TOKEN_RPAREN			{ $$ = expr_create(EXPR_FCALL, $1, $3, 8); }				// Function call
			|	expr8 TOKEN_INCRE		{ $$ = expr_create(EXPR_INCRE, $1, 0, 8); }
			|	expr8 TOKEN_DECRE		{ $$ = expr_create(EXPR_DECRE, $1, 0, 8); }
			|	atomic					{ $$ = $1; }
			;

atomic		:	TOKEN_INT_LIT			{ $$ = expr_create_integer_literal(strtol(yytext, NULL, 10), 9); }
			|	TOKEN_FLOAT_LIT			{ $$ = expr_create_float_literal(strtod(yytext, NULL), 9); }
			|	TOKEN_CHAR_LIT			{ $$ = expr_create_char_literal(*yytext, 9); }
			|	TOKEN_STR_LIT			{ $$ = expr_create_string_literal(yytext, 9); }
			|	TOKEN_TRUE				{ $$ = expr_create(EXPR_TRUE, 0, 0, 9); }
			|	TOKEN_FALSE				{ $$ = expr_create(EXPR_FALSE, 0, 0, 9); }
			|	id						{ $$ = $1; }
			;

opt_expr	:	expr					{ $$ = $1; }
			|	/* Epsilon */			{ $$ = 0; }
			;

expr_list	:	expr TOKEN_COMMA expr_list		{ $$ = expr_create(EXPR_TERM, $1, $3, 8); }
			|	expr							{ $$ = expr_create(EXPR_TERM, $1, 0, 8); }
			;

opt_expr_list	:	expr_list			{ $$ = $1; }
				|	/* Epsilon */		{ $$ = 0; }
				;

expr_block	:	TOKEN_LBRACE expr_list TOKEN_RBRACE	{ $$ = expr_create(EXPR_BLOCK, $2, 0, 8); }		// Array initializer
			;

expr_block_list	:	expr_block TOKEN_COMMA expr_block_list	{ $$ = $1; $$->right = $3; }
				|	expr_block								{ $$ = $1; }

opt_expr_block_list	:	TOKEN_LBRACE expr_block_list TOKEN_RBRACE		{ $$ = expr_create(EXPR_BLOCK, $2, 0, 8); }
					|	expr_block										{ $$ = $1; }
					|	expr											{ $$ = $1; }
					;

/* Types */
type		:	TOKEN_INT				{ $$ = type_create(TYPE_INT); }
			|	TOKEN_FLOAT				{ $$ = type_create(TYPE_FLOAT); }
			|	TOKEN_BOOL				{ $$ = type_create(TYPE_BOOL); }
			|	TOKEN_CHAR				{ $$ = type_create(TYPE_CHAR); }
			|	TOKEN_STR				{ $$ = type_create(TYPE_STR); }
			|	TOKEN_AUTO				{ $$ = type_create(TYPE_AUTO); }
			|	TOKEN_VOID				{ $$ = type_create(TYPE_VOID); }
			|	TOKEN_ARRAY TOKEN_LBRACKET opt_expr TOKEN_RBRACKET type		{ $$ = type_create_array($5, $3); }	// Do we permit array with empty length?
			|	TOKEN_FUNC type TOKEN_LPAREN opt_param_list TOKEN_RPAREN	{ $$ = type_create_function($2, $4); }
			;

/* Params */
param_list	:	id TOKEN_COLON type TOKEN_COMMA param_list		{ $$ = param_list_create($1->name, $3, $5); }
			|	id TOKEN_COLON type								{ $$ = param_list_create($1->name, $3, 0); }
			;

opt_param_list	:	param_list			{ $$ = $1; }
				|	/* Epsilon */		{ $$ = 0; }
				;

/* Misc */
id			:	TOKEN_IDENT		{ $$ = expr_create_name(yytext, 9); }
			;

%%

/* Called when parse error */
int yyerror( const char *str )
{
	error("Parse error: %s",str);
	return 1;
}