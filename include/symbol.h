
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
	int is_prototype;
};

#include "scope.h"
#include "type.h"
#include <stdio.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, const char *name );
void symbol_delete( struct symbol *s );
void symbol_print( struct symbol *s );

#endif
