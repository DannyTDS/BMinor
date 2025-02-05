#include "type.h"

#include <stdio.h>
#include <stdlib.h>

struct type * type_create( type_t kind )
{
    struct type* t = calloc(1, sizeof(struct type));
    t->kind = kind;
    return t;
}

struct type * type_create_array( struct type *subtype, struct expr *arr_size )
{
    // FIXME What if arr_size is 0 (unspecified)?
    struct type* t = type_create(TYPE_ARRAY);
    t->subtype = subtype;
    t->arr_size = arr_size;
    return t;
}

struct type * type_create_function( struct type *subtype, struct param_list *params )
{
    struct type* t = type_create(TYPE_FUNC);
    t->subtype = subtype;
    t->params = params;
    return t;
}

void type_delete( struct type *t )
{
    if (!t) return;
    expr_delete(t->arr_size);
    param_list_delete(t->params);
    type_delete(t->subtype);
    free(t);
}

void type_print( struct type *t ) {
    switch (t->kind) {
        case TYPE_ARRAY:
            printf("array [");
            expr_print(t->arr_size);
            printf("] ");
            type_print(t->subtype);
            break;
        case TYPE_AUTO:
            printf("auto");
            break;
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_BOOL:
            printf("boolean");
            break;
        case TYPE_CHAR:
            printf("char");
            break;
        case TYPE_INT:
            printf("integer");
            break;
        case TYPE_FLOAT:
            printf("float");
            break;
        case TYPE_STR:
            printf("string");
            break;
        case TYPE_FUNC:
            printf("function ");
            type_print(t->subtype);
            printf(" (");
            param_list_print(t->params);
            printf(")");
            break;
    }
}

/**
 * Compare the type structs t1 and t2.
 * @return Returns 0 if type of t1 and t2 are same. Returns 1 if different.
*/
int type_cmp( struct type* t1, struct type* t2) {
    if (t1->kind != t2->kind) return 1;
    if (t1->kind == TYPE_ARRAY) {
        if (type_cmp(t1->subtype, t2->subtype)!=0) return 1;
        /* Check length of array if defined as literal. Otherwise omit the check. */
        else if (!t1->arr_size || !t2->arr_size) return 0;
        else if (!t1->arr_size->int_literal || !t2->arr_size->int_literal) return 0;
        else if (t1->arr_size->int_literal != t2->arr_size->int_literal) return 1;
        else return 0;
    } else if (t1->kind == TYPE_FUNC) {
        if (type_cmp(t1->subtype, t2->subtype)==0 && param_list_cmp(t1->params, t2->params)==0) return 0;
        else return 1;
    } else return 0;
}

/**
 * Copy all the attributes of the src type to a new type structure.
 * @return The copied type structure.
*/
struct type * type_copy( struct type *src ) {
    struct type* dst = type_create(src->kind);
    dst->arr_size = src->arr_size;
    dst->params = src->params;
    dst->subtype = src->subtype;
    return dst;
}