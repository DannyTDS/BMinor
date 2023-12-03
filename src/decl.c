#include "decl.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* argreg[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
int MAX_NARG = 6;               // Maximum number of arguments to a function

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

    // Resolve array length *RECURSIVELY*
    if (d->type->kind == TYPE_ARRAY) {
        for (struct type* arr_type = d->type; arr_type->kind == TYPE_ARRAY; arr_type = arr_type->subtype) {
            expr_resolve(arr_type->arr_size);
        }
    }

    // Mark function declarations for special handling
    if (d->type->kind == TYPE_FUNC && !d->code) {
        sym->nvar = scope_size();
        sym->is_prototype = 1;
        sym->allow_redecl = 1;
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
                    sym->nvar = found_sym->nvar;            // Only the arguments.
                    warn("Resolve warning: multiple prototypes of %s", d->name);
                } else {
                    // Not allowed different prototypes
                    error("Resolve error: different prototypes of %s", d->name);
                    resolve_error++;
                }
            } else if (sym->type->kind==TYPE_FUNC && d->code) {
                // Trying to resolve a function definition to a previously declared prototype
                if (found_sym->allow_redecl) {
                    if (type_cmp(sym->type, found_sym->type)==0) {
                    // Bind definition to prototype, and disallow any other definitions
                    printf("%s defines ", d->name);
                    symbol_print(found_sym);
                    printf("\n");
                    // Disallow other definitions
                    found_sym->allow_redecl = 0;
                    } else {
                        // Definition doesn't agree with prototype signature
                        error("Resolve error: definition doesn't match prototype: %s", d->name);
                        resolve_error++;
                    }
                } else {
                    // Multiple definitions of non-prototypes is not OK
                    error("Resolve error: multiple definitions of %s", d->name);
                    resolve_error++;
                }
            } else {
                // Previous prototype is re-defined to another type
                error("Resolve error: prototype is redeclared as non-function type: %s", d->name);
                resolve_error++;
            }
        } else if (found_sym->kind==SYMBOL_PARAM && found_sym->allow_redecl) {
            // Redeclaration of a param. Warn, bind with new declaration
            warn("Resolve warning: redeclaring param in local scope: %s", d->name);
            symbol_update(found_sym, sym);
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
        sym->nvar = scope_size();       // Now nvar holds the number of local variables within the function.
        if (found_sym) found_sym->nvar = sym->nvar;
        scope_exit();
    }

    decl_resolve(d->next);
}


void decl_typecheck( struct decl *d ) {
    if (!d) return;

    /* TODO: Sanity tests before we typecheck specific fields */
    if (d->type->kind == TYPE_FUNC) {
        /* Function cannot return array or another function */
        if (d->type->subtype->kind == TYPE_ARRAY || d->type->subtype->kind == TYPE_FUNC) {
            printf("[ERROR]    Type error: function '%s' does not support return type ", d->name);
            type_print(d->type->subtype);
            printf("\n");
            typecheck_error++;
        }
        /* Check parameters */
        for (struct param_list* param = d->type->params; param; param = param->next) {
            if (param->type->kind == TYPE_FUNC) {
                /* Parameters cannot contain function type */
                error("Type error: declaring parameter '%s' of function '%s' as function type is not supported", param->name, d->name);
                typecheck_error++;
            } else if (param->type->kind == TYPE_ARRAY) {
                /* Array param cannot have declared length */
                if (param->type->arr_size) {
                    error("Type error: array parameter '%s' of function '%s' must be declared with empty length", param->name, d->name);
                    typecheck_error++;
                }
            }
        }
        /* Check functions defined in local space */
        if (d->symbol->kind == SYMBOL_LOCAL) {
            error("Type error: declaring function '%s' in local scope is not supported", d->name);
            typecheck_error++;
        }
    }

    if (d->type->kind == TYPE_ARRAY) {
        /* IMPORTANT: Check array recursively since we allow nested arrays */
        struct type* curr = d->type;
        while (curr->kind == TYPE_ARRAY) {
            /* Array of functions */
            if (curr->subtype->kind == TYPE_FUNC) {
                error("Type error: declaring array '%s' as array of functions is not supported", d->name);
                typecheck_error++;
            }
            /* Array length */
            struct type* len_type = expr_typecheck(curr->arr_size);
            if (d->symbol->kind == SYMBOL_GLOBAL) {
                if (!len_type) {
                    error("Type error: global array '%s' must be declared with constant integer length", d->name);
                    typecheck_error++;
                } else if (curr->arr_size->kind != EXPR_INT_LIT) {
                    printf("[ERROR]    Type error: expect declared length of global array '%s' to be constant integer, but found ", d->name);
                    type_print(len_type);
                    printf("\n");
                    typecheck_error++;
                }
            } else if (d->symbol->kind == SYMBOL_LOCAL) {
                /* If in local scope, non-integer evaluation */
                if (!len_type) {
                    error("Type error: local array '%s' must be declared with length that evaluates to integer", d->name);
                    typecheck_error++;
                } else if (len_type->kind != TYPE_INT) {
                    printf("[ERROR]    Type error: expect declared length of local array '%s' to be integer, but found ", d->name);
                    type_print(d->type->arr_size->symbol->type);
                    printf("\n");
                    typecheck_error++;
                }
            }
            /* Recurse to the subtype of the array */
            curr = curr->subtype;
        }
    }

    struct type* rtype;

    /* Variable definition */
    if (d->value) {
        /* 1. Check type match */
        /* Check it is NOT a function */
        if (d->type->kind == TYPE_FUNC) {
            error("Type error: %s is declared as function but received expression assignment", d->name);
            typecheck_error++;
        } else if (d->type->kind == TYPE_ARRAY) {
            rtype = expr_typecheck(d->value);       // rtype = type(TYPE_ARRAY, subtype=element type)
            /* Check rtype is array type and two array types (possibly nested arrays) are the same */
            if (rtype->kind != TYPE_ARRAY || type_cmp(rtype, d->type)!=0) {
                printf("[ERROR]    Type error: name '%s' is declared as ", d->name);
                type_print(d->type);
                printf(" but assignment is ");
                type_print(rtype);
                printf(" (");
                expr_print(d->value);
                printf(")");
                printf("\n");
                typecheck_error++;
            }
            /* Local arrays are not permitted to use array initializer */
            if (d->symbol->kind == SYMBOL_LOCAL && d->value->kind == EXPR_BLOCK) {
                error("Type error: array '%s' is defined in local scope but received array initializer assignment", d->name);
                typecheck_error++;
            }
        } else {
            rtype = expr_typecheck(d->value);
            if (type_cmp(d->type, rtype)!=0) {
                printf("[ERROR]    Type error: name '%s' is declared as ", d->name);
                type_print(d->type);
                printf(" but assignment is ");
                type_print(rtype);
                printf(" (");
                expr_print(d->value);
                printf(")");
                printf("\n");
                typecheck_error++;
            }
        }
        /* 2. If in global scope, must be constant assignment */
        if (d->symbol->kind == SYMBOL_GLOBAL && !expr_typecheck_constant(d->value)) {
            error("Type error: name '%s' is defined in global scope but received non-constant assignment", d->name);
            typecheck_error++;
        }
    }

    /* Function definition */
    if (d->code) {
        /* Check it is a function */
        if (d->type->kind != TYPE_FUNC) {
            printf("[ERROR]    Type error: name '%s' is declared as ", d->name);
            type_print(d->type);
            printf(" but received function definition\n");
            typecheck_error++;
        } else {
            stmt_typecheck(d->code, d);
        }
    }
    
    /* Loop through the decl list */
    decl_typecheck(d->next); 
}


void decl_codegen( struct decl *d ) {
    if (!d) return;
    
    if (d->symbol->kind==SYMBOL_GLOBAL) {
        switch (d->type->kind) {
            /* Global variables. All unintialized values default to 0. */
            case TYPE_CHAR:
            case TYPE_INT: {
                int64_t literal = 0;
                if (d->value) literal = d->value->int_literal;
                fprintf(output, ".data\n%s:\t.quad\t%ld\n", d->name, literal);
                break;
            }
            case TYPE_FLOAT:
                // TODO: Support floating point
                error("Codegen error: Floating point numbers not implemented");
			    exit(FAILURE);
            case TYPE_STR: {
                char es[BUFSIZ] = "";
			    if (d->value) string_encode(d->value->string_literal, es);
                fprintf(output, ".data\n%s:\t.string\t%s\n", d->name, es);
                break;
            }
            case TYPE_BOOL: {
                int literal = 0;
                if (d->value) literal = d->value->kind==EXPR_TRUE ? 1 : 0;
                fprintf(output, ".data\n%s:\t.quad\t%d\n", d->name, literal);
                break;
            }
            case TYPE_ARRAY:
                /* Only global, integer arrays are implemented. */
                if (d->type->subtype->kind != TYPE_INT) {
                    error("Codegen error: non-integer array not implemented: %s", d->name);
                    exit(FAILURE);
                } else if (d->value) {
                    fprintf(output, ".data\n%s:\t.quad\t", d->name);
                    /* d->value is EXPR_BLOCK. List of elements stored in d->value->left */
                    for (struct expr* e=d->value->left; e; e=e->right) {
                        fprintf(output, "%ld", e->left->int_literal);
                        if (e->right) fprintf(output, ", ");
                    }
                    fprintf(output, "\n");
                } else {
                    /* No initialization provided */
                    fprintf(output, ".data\n%s:\t.quad\t0\n", d->name);
                }
            /* Global functions */
            case TYPE_FUNC:
                if (d->code) {
                    /* Definition */
                    fprintf(output, ".text\n");
                    fprintf(output, ".global %s\n", d->name);
                    fprintf(output, "%s:\n", d->name);              // Prologue
                    fprintf(output, "\tPUSHQ %%rbp\n");             // Save base pointer
                    fprintf(output, "\tMOVQ %%rsp, %%rbp\n");       // Set new base pointer
                    int arg_cnt = 0;                                // Allocate arguments
                    for (struct param_list* p=d->type->params; p; p=p->next) {
                        if (arg_cnt == MAX_NARG) {
                            error("Codegen error: only up to 6 arguments to a function is supported: %s", d->name);
                            exit(FAILURE);
                        }
                        fprintf(output, "\tPUSHQ %%%s\n", argreg[arg_cnt]);
                        arg_cnt++;
                    }
                    fprintf(output, "\tSUBQ $%d, %%rsp\n", (d->symbol->nvar - arg_cnt) * 8);        // Allocate stack for local variables
                    for (int reg=0; reg<SCRATCH_NREG; reg++) {                                      // Push callee-saved registers
                        /* Skip caller-saved regs r10, r11 */
                        if (reg != 1 && reg != 2) {
                            fprintf(output, "\tPUSHQ %%%s\n", scratch_name(reg));
                        }
                    }
                    stmt_codegen(d->code, d->name);                 // Body
                    fprintf(output, ".%s_epilogue:\n", d->name);    // Epilogue
                    for (int reg=SCRATCH_NREG-1; reg>=0; reg--) {                                   // Pop callee-saved registers
                        /* Skip caller-saved regs r10, r11 */
                        if (reg != 1 && reg != 2) {
                            fprintf(output, "\tPOPQ %%%s\n", scratch_name(reg));
                        }
                    }
                    fprintf(output, "\tMOVQ %%rbp, %%rsp\n");
                    fprintf(output, "\tPOPQ %%rbp\n");
                    fprintf(output, "\tRET\n");
                }
            default:
                break;
        }
    } else if (d->symbol->kind==SYMBOL_LOCAL) {
        /* Local variables */
        switch (d->type->kind) {
            case TYPE_CHAR:
            case TYPE_INT:
            case TYPE_FLOAT:
            case TYPE_STR:
            case TYPE_BOOL:
                if (!d->value) break;       // Only operate if a value is given.
                /* Definition stored on rhs */
                expr_codegen(d->value);
                /* symbol_codegen returns the variable's address on stack */
                fprintf(output, "\tMOVQ %%%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
                scratch_free(d->value->reg);
                break;
            case TYPE_ARRAY:
                error("Codegen error: local array not implemented: %s", d->name);
                exit(FAILURE);
            default:
                break;
        }
    }

    decl_codegen(d->next);
}