#include "symbol.h"

#include <stdlib.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name )
{
    struct symbol* s = calloc(1, sizeof(struct symbol));
    return s;
}

void symbol_delete( struct symbol *s )
{
    
}