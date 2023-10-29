#include "expr.h"
#include "encoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Create one node in an expression tree and return the structure.
*/

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, int precedence )
{
	/* Shortcut: sizeof(*e) means "the size of what e points to" */
	struct expr *e = calloc(1, sizeof(*e));

	e->kind = kind;
	e->left = left;
	e->right = right;
	e->precedence = precedence;

	return e;
}

/* All literals are created in the highest precedence */
/* Still takes precedence as arg for potential changes in parser */
struct expr * expr_create_name( const char *n, int precedence )
{
	struct expr *e = expr_create(EXPR_IDENT, NULL, NULL, precedence);
    e->name = strdup(n);
    return e;
}

struct expr * expr_create_integer_literal( int64_t d, int precedence )
{
    struct expr *e = expr_create(EXPR_INT_LIT, NULL, NULL, precedence);
    e->int_literal = d;
    return e;
}

struct expr * expr_create_float_literal( double f, int precedence )
{
	struct expr *e = expr_create(EXPR_FLOAT_LIT, NULL, NULL, precedence);
	e->float_literal = f;
	return e;
}

struct expr * expr_create_char_literal( char c, int precedence )
{
	struct expr *e = expr_create(EXPR_CHAR_LIT, NULL, NULL, precedence);
	e->int_literal = (int64_t) c;
	return e;
}

struct expr * expr_create_string_literal( const char *str, int precedence )
{
	struct expr *e = expr_create(EXPR_STR_LIT, NULL, NULL, precedence);
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

	/* Special handling for char and string literals */
	/* Since we decoded the string to verify its correctness in scanner, we need to encode it back before printing */
	if (e->kind == EXPR_CHAR_LIT) {
		char es[BUFSIZ];
		char s[2] = "\0";
		s[0] = (char)e->int_literal;
		string_encode(s, es);
		/* Take the encoded char part in between ""s */
		char ec[BUFSIZ];
		sscanf(es, "\"%[^\"]", ec);
		printf("%s", ec);
		return;
	} else if (e->kind == EXPR_STR_LIT) {
		char es[BUFSIZ];
		string_encode(e->string_literal, es);
		printf("%s", es);
		return;
	}

	/* All other kinds */
	switch(e->kind) {
		case EXPR_ADD:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "+");
			break;
		case EXPR_SUB:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "-");
			break;
		case EXPR_MUL:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "*");
			break;
		case EXPR_DIV:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "/");
			break;
		case EXPR_EXP:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "^");
			break;
		case EXPR_MOD:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "%");
			break;
        case EXPR_INT_LIT:
			printf("%ld", e->int_literal);
			break;
		case EXPR_FLOAT_LIT:
			/* %g decides between floating pt and scientific notation */
			printf("%g", e->float_literal);
			break;
		case EXPR_TRUE:
			printf("true");
			break;
		case EXPR_FALSE:
			printf("false");
			break;
		case EXPR_NOT:
			e = expr_wrap_auto(e, ASSOC_NONE);
			printf("!");
			expr_print(e->right);
			break;
		case EXPR_AND:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "&&");
			break;
		case EXPR_OR:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "||");
			break;
		case EXPR_INCRE:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print(e->left);
			printf("++");
			break;
		case EXPR_DECRE:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print(e->left);
			printf("--");
			break;
		case EXPR_LT:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "<");
			break;
		case EXPR_LE:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "<=");
			break;
		case EXPR_GT:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, ">");
			break;
		case EXPR_GE:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, ">=");
			break;
		case EXPR_EQ:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "==");
			break;
		case EXPR_NE:
			e = expr_wrap_auto(e, ASSOC_LEFT);
			expr_print_binary(e, "!=");
			break;
		case EXPR_ASSIGN:
			e = expr_wrap_auto(e, ASSOC_RIGHT);
			expr_print_binary(e, "=");
			break;
		case EXPR_IDENT:
			printf("%s", e->name);
			break;
		case EXPR_FCALL:
			expr_print(e->left);
			printf("(");
			expr_print(e->right);
			printf(")");
			break;
		case EXPR_INDEX:
			expr_print(e->left);
			printf("[");
			expr_print(e->right);
			printf("]");
			break;
		case EXPR_TERM:
			expr_print(e->left);
			if (e->right) printf(", ");
			expr_print(e->right);
			break;
		case EXPR_BLOCK:
			printf("{");
			expr_print(e->left);
			printf("}");
			if (e->right) printf(", ");
			expr_print(e->right);
			break;
		case EXPR_GROUP:
			/* Check for nested parens */
			if (e->left->kind == EXPR_GROUP) expr_print(e->left);
			else {
				printf("(");
				expr_print(e->left);
				printf(")");
			}
			break;
		default:
			break;
	}
}


void expr_print_binary(struct expr* e, const char* op) {
	expr_print(e->left);
	printf("%s", op);
	expr_print(e->right);
}


struct expr * expr_wrap(struct expr* e) {
	/* Wrap the given expr in parens. Useful in printing nested grouping */
    if (e->kind != EXPR_GROUP) return expr_create(EXPR_GROUP, e, 0, 8);
    else return e;
}

struct expr * expr_wrap_auto(struct expr* e, assocRule_t rule) {
	/* Check the precedence value of two exprs. Apply necessary grouping. */
	/* Higher precedence = higer precedence in the AST = lower precedence */
	if (e->left && e->left->precedence < e->precedence) e->left = expr_wrap(e->left);
	if (e->right && e->right->precedence < e->precedence) e->right = expr_wrap(e->right);
	if (rule == ASSOC_LEFT) {
		if (e->right && e->right->precedence == e->precedence) e->right = expr_wrap(e->right);
	}
	if (rule == ASSOC_RIGHT) {
		if (e->left && e->left->precedence == e->precedence) e->left = expr_wrap(e->left);
	}
	return e;
}