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

    // Mark function declarations for special handling
    if (d->type->kind == TYPE_FUNC && !d->code) {
        sym->is_prototype = 1;
    }

    // Bind symbol after expression has been evaluated.
    // Avoid situations like "x = x+1" at first declaration.
    struct symbol* found_sym = scope_lookup_current(d->name);
    if (found_sym) {
        // Function prototypes can be declared more than once, as long as they have the same signature
        if (found_sym->is_prototype) {
            if (sym->is_prototype) {
                // Evaluating two function prototypes
                if (type_cmp(sym->type, found_sym->type)==0) {
                    // Allowed, but don't bind
                    warn("Resolve warning: multiple prototypes of %s", d->name);
                } else {
                    // Not allowed different prototypes
                    error("Resolve error: different prototypes of %s", d->name);
                    resolve_error++;
                }
            } else if (sym->type->kind==TYPE_FUNC && d->code) {
                // Trying to resolve a function definition to a previously declared prototype
                if (type_cmp(sym->type, found_sym->type)==0) {
                    // Bind definition to prototype, and disallow any other definitions
                    printf("%s defines ", d->name);
                    symbol_print(found_sym);
                    printf("\n");
                    // Disallow other binds
                    found_sym->is_prototype = 0;
                } else {
                    // Definition doesn't agree with prototype signature
                    error("Resolve error: definition doesn't match prototype: %s", d->name);
                    resolve_error++;
                }
            } else {
                // Previous prototype is re-defined to another type
                error("Resolve error: prototype is redeclared as non-function type: %s", d->name);
                resolve_error++;
            }
        } else {
            // Multiple definitions of non-prototypes is not OK
            error("Resolve error: multiple definitions of %s", d->name);
            resolve_error++;
        }
    } else {
        scope_bind(d->name, sym);
    }
    d->symbol = sym;

    // Resolve function definitions
    if (d->code) {
        scope_enter();
        param_list_resolve(d->type->params);
        // Function definitions are a STMT_BLOCK by itself in the AST.
        // Override so that we don't create a new scope (other than the param_list) for this STMT_BLOCK.
        stmt_resolve(d->code->body);
        scope_exit();
    }

    decl_resolve(d->next);
}