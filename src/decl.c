#include "decl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next )
{
    struct decl* d = calloc(1, sizeof(struct decl));
    d->name = strdup(name);
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    return d;
}

void decl_delete( struct decl *d )
{
    if (!d) return;
}

void decl_print( struct decl *d, int indent )
{

}