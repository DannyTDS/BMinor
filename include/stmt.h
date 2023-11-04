#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "symbol.h"
#include "scope.h"
#include <stdbool.h>

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
	bool use_indent;		// Override whether indent / new line are printed
	bool use_endl;
};

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );

void stmt_delete( struct stmt *s );
void stmt_print( struct stmt *s, int indent );
void stmt_resolve( struct stmt *s );

struct stmt * stmt_wrap( struct stmt * s );

#endif
