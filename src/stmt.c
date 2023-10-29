#include "stmt.h"

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
            printf("if(");
            expr_print(s->expr);
            printf(") ");
            stmt_print(s->body, indent);
            if (s->else_body) {
                printf(" else ");
                stmt_print(s->else_body, indent);
            }
            break;        
        case STMT_FOR:
            indent_by(indent);
            printf("for(");
            expr_print(s->init_expr);
            printf(";");
            expr_print(s->expr);
            printf(";");
            expr_print(s->next_expr);
            printf(") ");
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
            // Remove redundant braces
            if (s->body && s->body->kind == STMT_BLOCK) stmt_print(s->body, indent);
            else {
                printf("{\n");
                stmt_print(s->next, indent + 1);
                indent_by(indent);
                printf("}");
                break;
            }
    }
    // Print all stmt of the same level
    stmt_print(s->next, indent);
}