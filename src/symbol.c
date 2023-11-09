#include "symbol.h"

#include <stdlib.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, const char *name )
{
    struct symbol* s = calloc(1, sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;
    s->is_prototype = 0;
    s->allow_redecl = 0;
    return s;
}

void symbol_delete( struct symbol *s )
{
    if (!s) return;
    type_delete(s->type);
    if (s->name) free((char*)s->name);
    free(s);
}


void symbol_print(struct symbol* s) {
    if (!s) return;
    switch (s->kind) {
        case SYMBOL_GLOBAL:
            if (s->is_prototype) printf("global prototype %s", s->name);
            else printf("global %s", s->name);
            break;
        case SYMBOL_LOCAL:
            printf("local %d", s->which);
            break;
        case SYMBOL_PARAM:
            printf("param %d", s->which);
            break;
    }
}


void symbol_update( struct symbol* old, struct symbol* new) {
    // Update old's attributes to new's
    old->kind = new->kind;
    old->name = new->name;
    old->type = new->type;
    // Omit the which field
    old->is_prototype = new->is_prototype;
    old->allow_redecl = new->allow_redecl;
}