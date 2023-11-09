#include "scope.h"
#include "utils.h"
#include <stdlib.h>

/* Top of the stack */
struct scope* stack;

void scope_enter() {
    struct scope* new_scope = calloc(1, sizeof(struct scope));
    new_scope->ht = hash_table_create(0, 0);
    // Put the new node on the top of the stack
    // FIXME: rational of the which field
    if (!stack) {
        new_scope->level = 0;
        new_scope->size = 0;
    } else {
        new_scope->next = stack;
        new_scope->level = stack->level+1;
        // For nested local scopes, update the size of the new scope
        new_scope->size = (new_scope->level > 1) ? stack->size : 0;
    }
    stack = new_scope;
};

void scope_exit() {
    if (!stack) return;
    struct scope* next_scope = stack->next;
    // Update size field for consistency in local scopes
    if (stack->level > 1) {
        next_scope->size = stack->size;
    }
    hash_table_delete(stack->ht);
    stack = next_scope;
};

int scope_level() {
    return (stack) ? stack->level : -1;
};

void scope_bind( const char *name, struct symbol *sym ) {
    sym->which = stack->size;
    stack->size++;
    if (!hash_table_insert(stack->ht, name, sym)) {
        error("Hash table error: unable to insert key %s", name);
    }
};


struct symbol *scope_lookup( const char *name ) {
    // Attempt to lookup and return the symbol struct associated with name iteratively through all scopes
    // @return symbol associated with the name, NULL if not found
    struct symbol* sym;
    struct scope* curr = stack;
    while (curr) {
        sym = hash_table_lookup(curr->ht, name);
        if (sym) break;
        else curr = curr->next;
    }
    return sym;
};

struct symbol *scope_lookup_current( const char *name ) {
    return hash_table_lookup(stack->ht, name);
};


// For debugging
void scope_print() {
    puts("===============");
    struct scope* curr = stack;
    while (curr) {
        printf("[%d] {", curr->level);
        hash_table_firstkey(curr->ht);
        char* key;
        void* sym;
        while (hash_table_nextkey(curr->ht, &key, &sym) != 0 ) {
            printf("%s: ", key);
            symbol_print((struct symbol*)sym);
            printf(",");
        }
        printf("}\n");
        curr = curr->next;
    }
    puts("===============");
}