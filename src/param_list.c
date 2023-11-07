#include "param_list.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct param_list * param_list_create( const char *name, struct type *type, struct param_list *next )
{
    struct param_list* a = calloc(1, sizeof(struct param_list));
    a->name = strdup(name);
    a->type = type;
    a->next = next;
    return a;
}

void param_list_delete( struct param_list *a )
{
    if (!a) return;
    free((char*)a->name);
    type_delete(a->type);
    param_list_delete(a->next);
    free(a);
}

void param_list_print( struct param_list *a )
{
    if (!a) return;
    printf("%s: ", a->name);
    type_print(a->type);
    if (a->next) {
        printf(", ");
        param_list_print(a->next);
    }
}


void param_list_resolve(struct param_list *a) {
    if (!a) return;
    struct symbol* sym = symbol_create(SYMBOL_PARAM, a->type, a->name);
    sym->allow_redecl = 1;
    a->symbol = sym;
    if (scope_lookup_current(a->name)) {
        // Multiple definitions of the same name
        error("Resolve error: multiple definitions of %s", a->name);
        resolve_error++;
    } else {
        scope_bind(a->name, sym);
    }
    param_list_resolve(a->next);
}


int param_list_cmp( struct param_list *a1, struct param_list *a2 ) {
    /* Compare if two param lists are the same */
    // Return 0 if a1 and a2 are the same, 1 otherwise
    if (!a1 && !a2) return 0;
    if (!a1 || !a2) return 1;
    if (a1->name != a2->name || type_cmp(a1->type, a2->type)!=0) return 1;
    else return param_list_cmp(a1->next, a2->next);
}