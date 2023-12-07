#ifndef EXPR_H
#define EXPR_H

#include "scope.h"
#include "symbol.h"
#include "scratch.h"
#include "label.h"
#include "helper.h"
#include <stdint.h>
#include <stdio.h>

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_EXP,
	EXPR_MOD,
	/* many more kinds of exprs to add here */
	/* literals */
	EXPR_INT_LIT,
	EXPR_FLOAT_LIT,
	EXPR_CHAR_LIT,
	EXPR_STR_LIT,
	EXPR_TRUE,
	EXPR_FALSE,
	/* logicals */
	EXPR_NOT,
	EXPR_AND,
	EXPR_OR,
	/* Unary */
	EXPR_INCRE,
	EXPR_DECRE,
	EXPR_LT,
	EXPR_LE,
	EXPR_GT,
	EXPR_GE,
	EXPR_EQ,
	EXPR_NE,
	/* Misc */
	EXPR_ASSIGN,
	EXPR_IDENT,		// Identifier
	EXPR_FCALL,		// Function call
	EXPR_INDEX,		// Array indexing
	EXPR_TERM,		// Single expr in an expr_list, separated by comma
	EXPR_BLOCK,		// A block of expressions grouped by braces {}
	EXPR_GROUP,		// A group of expressions surrounded by parens ()
} expr_t;

typedef enum {
	ASSOC_LEFT,
	ASSOC_RIGHT,
	ASSOC_NONE,
} assocRule_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int64_t int_literal;
	double float_literal;
	const char * string_literal;
	struct symbol *symbol;

	/* used to determine grouping */
	int precedence;

	/* codegen */
	int reg;
};

// TODO: add precedence
struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, int precedence);

struct expr * expr_create_name( const char *n, int precedence );
struct expr * expr_create_integer_literal( int64_t d, int precedence );
struct expr * expr_create_float_literal( double f, int precedence );
struct expr * expr_create_char_literal( char c, int precedence );
struct expr * expr_create_string_literal( const char *str, int precedence );

void expr_delete( struct expr *e );
void expr_print( struct expr *e );
void expr_print_binary( struct expr *e, const char *op );
struct expr * expr_wrap(struct expr *e);
struct expr * expr_wrap_auto(struct expr *e, assocRule_t rule);			// Automatically group lower precedence expr

void expr_resolve( struct expr *e );
struct type * expr_typecheck( struct expr *e );
struct type* expr_typecheck_value( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc );
void expr_typecheck_boolean( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc );
void expr_typecheck_error( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc );
int expr_typecheck_constant( struct expr *e );

extern FILE* output;
extern int MAX_NARG;
extern const char* argreg[];
void expr_codegen( struct expr *e );
void expr_codegen_cmp( struct expr *e );		// Comparison codegen
void expr_codegen_fcall( struct expr *e );		// Function call codegen

int64_t expr_evaluate_literal( struct expr* e );
#endif
