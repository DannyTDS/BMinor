#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "param_list.h"
#include "symbol.h"
#include "scope.h"

struct decl {
	const char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( const char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_delete( struct decl *d );
void decl_print( struct decl *d, int indent );
void decl_resolve( struct decl *d );
void decl_typecheck( struct decl *d );

void indent_by( int amt );

#endif


