#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"

typedef enum {
	TYPE_ARRAY,
	TYPE_AUTO,
	TYPE_VOID,
	TYPE_BOOL,
	TYPE_CHAR,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STR,
	TYPE_FUNC,
} type_t;

struct type {
	type_t kind;
	struct expr  *arr_size;	// used for array type
	struct param_list *params;
	struct type *subtype;
};

struct type * type_create( type_t kind );

struct type * type_create_array( struct type *subtype, struct expr *arr_size );
struct type * type_create_function( struct type *subtype, struct param_list *params );
struct type * type_copy( struct type *src );

void		  type_delete( struct type *t );
void          type_print( struct type *t );
int			  type_cmp( struct type* t1, struct type* t2);

#endif
