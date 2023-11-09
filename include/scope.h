#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

/* Stack of hash tables, implemented as a linked list */
struct scope {
    struct hash_table *ht;
    int level;          // Level of the hash table in the stack, with 0 the lowest (global scope)
    int size;           // Number of variables present in the local scope
    struct scope *next;
};

void scope_enter();
void scope_exit();
int scope_level();
int scope_size();
void scope_size_incre();
void scope_bind( const char *name, struct symbol *sym );
struct symbol *scope_lookup( const char *name );
struct symbol *scope_lookup_current( const char *name );

void scope_print();

#endif