#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "expr.h"
#include "type.h"
#include "symbol.h"
#include "scope.h"

struct param_list {
	const char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( const char *name, struct type *type, struct param_list *next );
void param_list_delete( struct param_list *a );
void param_list_print( struct param_list *a );
void param_list_resolve( struct param_list *a );

int param_list_cmp( struct param_list *a1, struct param_list *a2 );

#endif
