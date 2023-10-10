#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Create one node in an expression tree and return the structure.
*/

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right )
{
	/* Shortcut: sizeof(*e) means "the size of what e points to" */
	struct expr *e = calloc(1, sizeof(*e));

	e->kind = kind;
	e->left = left;
	e->right = right;

	return e;
}

struct expr * expr_create_name( const char *n )
{
	struct expr *e = expr_create(EXPR_INT_LIT, NULL, NULL);
    e->name = strdup(n);
    return e;
}

struct expr * expr_create_integer_literal( int64_t d )
{
    struct expr *e = expr_create(EXPR_INT_LIT, NULL, NULL);
    e->int_literal = d;
    return e;
}

struct expr * expr_create_float_literal( double f )
{
	struct expr *e = expr_create(EXPR_FLOAT_LIT, NULL, NULL);
	e->float_literal = f;
	return e;
}

struct expr * expr_create_boolean_literal( int b )
{
	expr_t kind = (b == 0) ? EXPR_FALSE : EXPR_TRUE;
    struct expr *e = expr_create(kind, NULL, NULL);
    return e;
}

struct expr * expr_create_char_literal( char c )
{
	struct expr *e = expr_create(EXPR_CHAR_LIT, NULL, NULL);
	e->int_literal = (int) c;
	return e;
}

struct expr * expr_create_string_literal( const char *str )
{
	struct expr *e = expr_create(EXPR_STR_LIT, NULL, NULL);
	e->string_literal = strdup(str);
	return e;
}

/*
Recursively delete an expression tree.
*/

void expr_delete( struct expr *e )
{
	/* Careful: Stop on null pointer. */
	if(!e) return;
	expr_delete(e->left);
	expr_delete(e->right);
	if(e->name) free((char*)e->name);
	if(e->string_literal) free((char*)e->string_literal);
	free(e);
}

/*
Recursively print an expression tree by performing an
in-order traversal of the tree, printing the current node
between the left and right nodes.
*/

void expr_print( struct expr *e )
{
	/* Careful: Stop on null pointer. */
	if(!e) return;

	printf("(");

	expr_print(e->left);

	switch(e->kind) {
		case EXPR_ADD:
			printf("+");
			break;
		case EXPR_SUB:
			printf("-");
			break;
		case EXPR_MUL:
			printf("*");
			break;
		case EXPR_DIV:
			printf("/");
			break;
		case EXPR_EXP:
			printf("^");
			break;
		case EXPR_MOD:
			printf("%%");
			break;
        case EXPR_INT_LIT:
			printf("%ld", e->int_literal);
			break;
		case EXPR_FLOAT_LIT:
			printf("%lf", e->float_literal);
			break;
		case EXPR_CHAR_LIT:
			printf("%c", (char)e->int_literal);
			break;
		case EXPR_STR_LIT:
			printf("%s", e->string_literal);
			break;
		case EXPR_TRUE:
			printf("true");
			break;
		case EXPR_FALSE:
			printf("false");
			break;
		case EXPR_NOT:
			printf("!");
			break;
		case EXPR_AND:
			printf("&&");
			break;
		case EXPR_OR:
			printf("||");
			break;
		case EXPR_INCRE:
			printf("++");
			break;
		case EXPR_DECRE:
			printf("--");
			break;
		case EXPR_LT:
			printf("<");
			break;
		case EXPR_LE:
			printf("<=");
			break;
		case EXPR_GT:
			printf(">");
			break;
		case EXPR_GE:
			printf(">=");
			break;
		case EXPR_EQ:
			printf("==");
			break;
		case EXPR_NE:
			printf("!=");
			break;
		case EXPR_ASSIGN:
			printf("=");
			break;
	}

	expr_print(e->right);
	printf(")");
}