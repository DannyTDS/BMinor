#include "param_list.h"

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
    
}