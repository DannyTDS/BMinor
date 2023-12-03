
#ifndef SYMBOL_H
#define SYMBOL_H

// This declaration must be put before #includes
// Solves implicit declaration warning inside scope.h
typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	const char *name;
	int which;
	int nvar;					// Number of local variables. Used in moving stack pointers.
	int is_prototype;			// Is this symbol a function prototype?
	int allow_redecl;			// Does this symbol allow redeclaration? (Special case for params)
};

#include "scope.h"
#include "type.h"
#include <stdio.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, const char *name );
void symbol_delete( struct symbol *s );
void symbol_print( struct symbol *s );

// Update attributes of a symbol. Useful in redeclaring function params.
void symbol_update( struct symbol *old, struct symbol *new );

const char* symbol_codegen( struct symbol *s );

#endif
