#include "decl.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct decl * decl_create( const char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next )
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
    if (d->name) free((char*)d->name);
    if (d->type) type_delete(d->type);
    if (d->value) expr_delete(d->value);
    if (d->code) stmt_delete(d->code);
    decl_delete(d->next);
}

void indent_by( int amt )
{
    // Indent 4 spaces for a tab
    for (int i=0; i<amt * 4; i++) printf(" ");
}

void decl_print( struct decl *d, int indent )
{
    if (!d) return;

    indent_by(indent);
    printf("%s: ", d->name);
    type_print(d->type);
    
    if (d->value) {                 // Expr
        printf(" = ");
        expr_print(d->value);
        printf(";");
    } else if (d->code) {           // Stmt
        printf(" = ");
        d->code->use_indent = false;
        d->code->use_endl = false;
        stmt_print(d->code, indent);
    } else {
        printf(";");
    }
    printf("\n");

    // Loop over the linked list
    decl_print(d->next, indent);
}


void decl_resolve(struct decl* d) {
    if (!d) return;

    symbol_t kind = (scope_level() > 0) ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    struct symbol* sym = symbol_create(kind, d->type, d->name);

    // Resolve expressions
    if (d->value) {
        expr_resolve(d->value);
    }

    // Bind symbol after expression has been evaluated.
    // Avoid situations like "x = x+1" at first declaration.
    scope_bind(d->name, sym);
    d->symbol = sym;

    // Resolve function definitions
    if (d->code) {
        scope_enter();
        param_list_resolve(d->type->params);
        stmt_resolve(d->code);
        scope_exit();
    }

    decl_resolve(d->next);
}