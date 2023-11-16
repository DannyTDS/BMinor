#include "stmt.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next )
{
    struct stmt* s = calloc(1, sizeof(struct stmt));
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    s->use_indent = true;
    s->use_endl = true;
    return s;
}

void stmt_delete( struct stmt *s )
{
    if (!s) return;
    if (s->decl) decl_delete(s->decl);
    if (s->init_expr) expr_delete(s->init_expr);
    if (s->expr) expr_delete(s->expr);
    if (s->next_expr) expr_delete(s->next_expr);
    if (s->body) stmt_delete(s->body);
    if (s->else_body) stmt_delete(s->else_body);
    stmt_delete(s->next);
}

void stmt_print( struct stmt *s, int indent )
{
    if (!s) return;
    switch(s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            break;
        case STMT_EXPR:
            indent_by(indent);
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_IF_ELSE:
            if (s->use_indent) indent_by(indent);
            printf("if (");
            expr_print(s->expr);
            printf(") ");
            s->body = stmt_wrap(s->body);
            s->body->use_indent = false;
            if (s->else_body) s->body->use_endl = false;
            stmt_print(s->body, indent);
            if (s->else_body) {
                printf("else ");
                /* Special case of else-if clauses, without indentation and newline */
                if (s->else_body->kind == STMT_IF_ELSE) {
                    s->else_body->use_indent = false;
                    s->else_body->use_endl = false;
                    stmt_print(s->else_body, indent);
                } else {
                    s->else_body = stmt_wrap(s->else_body);
                    s->else_body->use_indent = false;
                    stmt_print(s->else_body, indent);
                }
            }
            break;        
        case STMT_FOR:
            indent_by(indent);
            printf("for (");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(") ");
            s->body = stmt_wrap(s->body);
            s->body->use_indent = false;
            stmt_print(s->body, indent);
            break;
        case STMT_PRINT:
            indent_by(indent);
            printf("print");
            if (s->expr) {
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;
        case STMT_RETURN:
            indent_by(indent);
            printf("return");
            if (s->expr) {
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;
        case STMT_BLOCK:
            // Remove redundant braces only if s->body is the only nested stmt
            if (s->body && s->body->kind == STMT_BLOCK && !s->body->next) stmt_print(s->body, indent);
            else {
                if (s->use_indent) indent_by(indent);
                printf("{\n");
                stmt_print(s->body, indent + 1);
                indent_by(indent);
                printf("} ");
                if (s->use_endl) printf("\n");
            }
            break;
    }
    // Print all stmt of the same level
    stmt_print(s->next, indent);
}


struct stmt * stmt_wrap(struct stmt * s)
{
    /* Wrap the given stmt in braces. Useful in printing one-line if-else or for loops */
    if (s->kind != STMT_BLOCK) return stmt_create(STMT_BLOCK, 0, 0, 0, 0, s, 0, 0);
    else return s;
}


void stmt_resolve(struct stmt* s) {
    if (!s) return;

    switch (s->kind) {
        case STMT_DECL:
            decl_resolve(s->decl);
            break;
        case STMT_EXPR:
            expr_resolve(s->expr);
            break;
        case STMT_IF_ELSE:
            expr_resolve(s->expr);
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            scope_enter();
            stmt_resolve(s->else_body);
            scope_exit();
            break;
        case STMT_FOR:
            expr_resolve(s->init_expr);
            expr_resolve(s->expr);
            expr_resolve(s->next_expr);
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            break;
        case STMT_PRINT:
            expr_resolve(s->expr);
            break;
        case STMT_RETURN:
            expr_resolve(s->expr);
            break;
        case STMT_BLOCK:
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            break;
    }

    stmt_resolve(s->next);
}


/**
 * @param func_decl: Pass in function decl where the stmt is defined in. Helps to determine function return type.
*/
void stmt_typecheck( struct stmt *s, struct decl *func_decl ) {
    if (!s) return;

    struct type* rtype;
    struct type* init_type;
    struct type* next_type;

    switch(s->kind) {
        case STMT_DECL:
            decl_typecheck(s->decl);
            break;
        case STMT_PRINT:
        case STMT_EXPR:
            expr_typecheck(s->expr);
            break;
        case STMT_IF_ELSE:
            /* Check condition is boolean */
            rtype = expr_typecheck(s->expr);
            if (rtype->kind != TYPE_BOOL) {
                printf("[ERROR]    Type error: if-else condition must be boolean, found ");
                type_print(rtype);
                printf(" (");
                expr_print(s->expr);
                printf(")\n");
                typecheck_error++;
            }
            /* Check bodies */
            stmt_typecheck(s->body, func_decl);
            stmt_typecheck(s->else_body, func_decl);
            break;
        case STMT_FOR:
            /* Check conditions */
            init_type = expr_typecheck(s->init_expr);
            if (init_type && init_type->kind != TYPE_INT) {
                printf("[ERROR]    Type error: for-loop start must be integer, found ");
                type_print(init_type);
                printf(" (");
                expr_print(s->init_expr);
                printf(")\n");
                typecheck_error++;
            }
            rtype = expr_typecheck(s->expr);
            if (rtype && rtype->kind != TYPE_BOOL) {
                printf("[ERROR]    Type error: for-loop end must be boolean, found ");
                type_print(rtype);
                printf(" (");
                expr_print(s->expr);
                printf(")\n");
                typecheck_error++;
            }
            next_type = expr_typecheck(s->next_expr);
            if (next_type && next_type->kind != TYPE_INT) {
                printf("[ERROR]    Type error: for-loop step must be integer, found ");
                type_print(next_type);
                printf(" (");
                expr_print(s->next_expr);
                printf(")\n");
                typecheck_error++;
            }
            stmt_typecheck(s->body, func_decl);
            break;
        case STMT_RETURN:
            rtype = expr_typecheck(s->expr);
            if (!rtype) rtype = type_create(TYPE_VOID);
            if (type_cmp(func_decl->symbol->type->subtype, rtype)!=0) {
                printf("[ERROR]    Type error: function '%s' expects return type ", func_decl->symbol->name);
                type_print(func_decl->symbol->type->subtype);
                printf(" but actually returned ");
                type_print(rtype);
                printf(" (");
				expr_print(s->expr);
				printf(")\n");
                typecheck_error++;
            }
            break;
        case STMT_BLOCK:
            stmt_typecheck(s->body, func_decl);
            break;
    }

    stmt_typecheck(s->next, func_decl);
}