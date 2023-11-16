#include "expr.h"
#include "utils.h"
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
		printf("\'%s\'", ec);
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
	/* Higher precedence = higer level in the AST */
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


void expr_resolve( struct expr* e ) {
	if (!e) return;
	
	// Resolve identifiers
	if (e->kind == EXPR_IDENT) {
		e->symbol = scope_lookup(e->name);
		if (!e->symbol) {
			error("Resolve error: undefined name %s.", e->name);
			resolve_error++;
		} else {
			printf("%s resolves to ", e->name);
			symbol_print(e->symbol);
			printf("\n");
		}
	} else {
		expr_resolve(e->left);
		expr_resolve(e->right);
	}
}


struct type* expr_typecheck( struct expr* e ) {
	if (!e) return 0;

	struct type* ltype = expr_typecheck(e->left);
	struct type* rtype = expr_typecheck(e->right);
	struct type* res;

	switch (e->kind) {
		/* Special handling for unary add/sub signs */
		case EXPR_ADD:
			/* If unary, expr is stored in right leaf */
			if (!e->left) {
				res = expr_typecheck_value(e->right, 0, rtype, 0, "add");
			} else {
				res = expr_typecheck_value(e->left, e->right, ltype, rtype, "add");
			}
			break;
		case EXPR_SUB:
			if (!e->left) {
				res = expr_typecheck_value(e->right, 0, rtype, 0, "sub");
			} else {
				res = expr_typecheck_value(e->left, e->right, ltype, rtype, "sub");
			}
			break;
		case EXPR_MUL:
			res = expr_typecheck_value(e->left, e->right, ltype, rtype, "multiple");
			break;
		case EXPR_DIV:
			res = expr_typecheck_value(e->left, e->right, ltype, rtype, "divide");
			break;
		case EXPR_EXP:
			res = expr_typecheck_value(e->left, e->right, ltype, rtype, "exponent");
			break;
		case EXPR_MOD:
			res = expr_typecheck_value(e->left, e->right, ltype, rtype, "mod");
			break;
		/* Literals only return the type they represent */
		case EXPR_INT_LIT:
			res = type_create(TYPE_INT);
			break;
		case EXPR_FLOAT_LIT:
			res = type_create(TYPE_FLOAT);
			break;
		case EXPR_CHAR_LIT:
			res = type_create(TYPE_CHAR);
			break;
		case EXPR_STR_LIT:
			res = type_create(TYPE_STR);
			break;
		case EXPR_TRUE:
		case EXPR_FALSE:
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_NOT:
			/* NOT is unary */
			expr_typecheck_boolean(e->right, 0, rtype, 0, "logical not");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_AND:
			expr_typecheck_boolean(e->left, e->right, ltype, rtype, "logical and");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_OR:
			expr_typecheck_boolean(e->left, e->right, ltype, rtype, "logical or");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_INCRE:
			/* Unary, value stored in left child. Always return TYPE_INT */
			res = expr_typecheck_value(e->left, 0, ltype, 0, "increment");
			break;
		case EXPR_DECRE:
			res = expr_typecheck_value(e->left, 0, ltype, 0, "decrement");
			break;
		case EXPR_LT:
			/* Override return value from expr_typecheck_value since comparisons always return boolean */
			expr_typecheck_value(e->left, e->right, ltype, rtype, "less than");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_LE:
			expr_typecheck_value(e->left, e->right, ltype, rtype, "less than or equal to");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_GT:
			expr_typecheck_value(e->left, e->right, ltype, rtype, "greater than");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_GE:
			expr_typecheck_value(e->left, e->right, ltype, rtype, "greater than or equal to");
			res = type_create(TYPE_BOOL);
			break;
		/* EQ and NE require some special handling over types, but they also always return boolean */
		case EXPR_EQ:
			expr_typecheck_value(e->left, e->right, ltype, rtype, "equal");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_NE:
			expr_typecheck_value(e->left, e->right, ltype, rtype, "not equal");
			res = type_create(TYPE_BOOL);
			break;
		case EXPR_ASSIGN:
			//FIXME:
			/* Can only assign to non-function non-array identifier or array index */
			if (ltype->kind == TYPE_FUNC) {
				error("Type error: cannot assign to function type '%s'", e->left->name);
				typecheck_error++;
			} else if (ltype->kind == TYPE_ARRAY && e->left->kind != EXPR_INDEX) {
				error("Type error: cannot assign to array type '%s', use indexing instead", e->left->name);
				typecheck_error++;
			} else if (e->left->kind != EXPR_IDENT && e->left->kind != EXPR_INDEX) {
				printf("[ERROR]    Type error: left side of assignent cannot be ");
				type_print(ltype);
				printf("\n");
				typecheck_error++;
			} else if (type_cmp(ltype, rtype) != 0) {
				/* Assignment type must match */
				printf("[ERROR]    Type error: cannot assign ");
				type_print(rtype);
				printf(" (");
				expr_print(e->right);
				printf(") to ");
				type_print(ltype);
				printf(" (%s)\n", e->left->name);
				typecheck_error++;
			}
			res = type_copy(rtype);
			break;
		case EXPR_IDENT:
			res = type_copy(e->symbol->type);
			break;
		case EXPR_FCALL:
			if (ltype->kind != TYPE_FUNC) {
				printf("[ERROR]    Type error: type ");
				type_print(ltype);
				printf(" (");
				expr_print(e->left);
				printf(") is not callable\n");
				typecheck_error++;
				res = type_copy(ltype);
			} else {
				/* Check argument type */
				struct expr* arg = e->right;
				struct param_list* param = ltype->params;
				struct type* atype;
				while (arg && param) {
					atype = expr_typecheck(arg);
					if (type_cmp(atype, param->type)!=0) {
						printf("[ERROR]    Type error: expect argument '%s' to function '%s' to be ", param->name, e->left->name);
						type_print(param->type);
						printf(" but found ");
						type_print(atype);
						printf(" (");
						expr_print(arg);
						printf(")\n");
						typecheck_error++;
					}
					arg = arg->right;
					param = param->next;
				}
				if (arg) {
					printf("[ERROR]    Type error: too many arguments to function '%s'\n", e->left->name);
					typecheck_error++;
				}
				if (param) {
					printf("[ERROR]    Type error: too few arguments to function '%s'\n", e->left->name);
					typecheck_error++;
				}
				/* Return function return type */
				res = type_copy(ltype->subtype);
			}
			break;
		case EXPR_INDEX:
			/* Left resolves to identifier. Right resolves to integer */
			if (ltype->kind != TYPE_ARRAY) {
				printf("[ERROR]    Type error: type ");
				type_print(ltype);
				printf(" is not subscriptable\n");
				typecheck_error++;
				res = type_copy(ltype);
			} else if (rtype->kind != TYPE_INT) {
				printf("[ERROR]    Type error: index cannot be ");
				type_print(rtype);
				printf(" (");
				expr_print(e->right);
				printf(")\n");
				typecheck_error++;
				res = type_copy(ltype->subtype);
			} else {
				res = type_copy(ltype->subtype);
			}
			break;
		case EXPR_TERM:
			/* Arguments */
			res = type_copy(ltype);
			break;
		case EXPR_BLOCK: {
			/* Only appears in array declaration. Typecheck *RECURSIVELY* */
			struct type* exp_type=0;			// Expected type
			struct type* act_type;			// Actual type
			if (e->left->kind == EXPR_BLOCK) {
				/* Check for nested arrays */
				for (struct expr* block = e->left; block; block = block->right) {
					act_type = expr_typecheck(block);
					/* Compare the subtype of arrays */
					if (!exp_type) exp_type = type_copy(act_type);		// Set expected type to first block's type
					else if (type_cmp(exp_type->subtype, act_type->subtype)!=0) {
						printf("[ERROR]    Type error: inconsistent types found in nested array instatiation: expect array of ");
						type_print(exp_type->subtype);
						printf(", but found array of ");
						type_print(act_type->subtype);
						printf("\n");
						typecheck_error++;
					}
				}
			} else {
				for (struct expr* term = e->left; term; term = term->right) {
					act_type = expr_typecheck(term);
					if (!exp_type) exp_type = type_copy(act_type);		// Set expected type to first term's type
					else if (type_cmp(exp_type, act_type)!=0) {
						printf("[ERROR]    Type error: inconsistent types found in array instatiation: expect ");
						type_print(exp_type);
						printf(", but found ");
						type_print(act_type);
						printf("\n");
						typecheck_error++;
					}
				}
			}
			res = type_create_array(exp_type, 0);
			break;
		}
		case EXPR_GROUP:
			/* Grouped exprs */
			res = type_copy(ltype);
			break;
	}
	
	return res;
}


/**
 * Used to print detailed typechecking error message for expr structure.
 * @param desc: Descriptive string of the expression's action.
*/
void expr_typecheck_error( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc) {
	char* opkind = (rtype) ? "binary" : "unary";
	printf("[ERROR]    Type error: invalid operands to %s %s: ", opkind, desc);
	type_print(ltype);
	printf(" (");
	expr_print(lexpr);
	printf(")");
	if (rtype) {
		printf(" and ");
		type_print(rtype);
		printf(" (");
		expr_print(rexpr);
		printf(")");
	}
	printf("\n");
	typecheck_error++;
}

/**
 * Checks if left and right side have the same type, and that type has to be int or float.
 * 
 * Supports unary and binary typecheck. If typechecking an unary operator, pass in `rexpr` and `rtype` as NULL.
 * 
 * @param desc: Descriptive string of the action taken by the operator.
 * @return Return a copy of `ltype` upon success. Otherwise return a fake type of `TYPE_INT`.
*/
struct type* expr_typecheck_value( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc ) {
	/* Typecheck unary */
	if (!rtype) {
		if (ltype->kind == TYPE_INT || ltype->kind == TYPE_FLOAT) {
			return type_copy(ltype);
		} else {
			/* Unary sign applied to unsupported type */
			expr_typecheck_error(lexpr, 0, ltype, 0, desc);
			return type_create(TYPE_INT);
		}
	} else {
		/* Typecheck binary*/
		if (type_cmp(ltype, rtype)!=0) {
			/* Not the same type */
			expr_typecheck_error(lexpr, rexpr, ltype, rtype, desc);
			return type_create(TYPE_INT);
		} else if (streq(desc, "equal") || streq(desc, "not equal")) {
			/* The equality operators != and == may be applied to any type except
			void, array, or function and always return boolean. */
			if (ltype->kind == TYPE_VOID || ltype->kind == TYPE_FUNC || ltype->kind == TYPE_ARRAY) {
				expr_typecheck_error(lexpr, rexpr, ltype, rtype, desc);
			}
			return type_create(TYPE_INT);
		} else if (ltype->kind != TYPE_INT && ltype->kind != TYPE_FLOAT) {
			/* Type error */
			expr_typecheck_error(lexpr, rexpr, ltype, rtype, desc);
			return type_create(TYPE_INT);
		} else {
			return type_copy(ltype);
		}
	}
}

/**
 * Checks if left and right side have the same type, and that type has to be boolean.
 * 
 * Supports unary and binary typecheck. If typechecking an unary operator, pass in rexpr and rtype as NULL.
 * 
 * @param desc: Descriptive string of the action taken by the operator.
 * @return Return void since boolean operation always result in boolean.
*/
void expr_typecheck_boolean( struct expr *lexpr, struct expr *rexpr, struct type *ltype, struct type *rtype, char* desc ) {
	if (!rtype) {
		if (ltype->kind != TYPE_BOOL) {
			expr_typecheck_error(lexpr, 0, ltype, 0, desc);
		}
	} else {
		if (ltype->kind != TYPE_BOOL || rtype->kind != TYPE_BOOL) {
			expr_typecheck_error(lexpr, rexpr, ltype, rtype, desc);
		}
	}
}

/**
 * Checks whether the given expression is a constant.
 * 
 * Constant includes: literals, array initializers.
 * @return Return 1 if expression is constant, 0 otherwise.
*/
int expr_typecheck_constant( struct expr *e ) {
	/* Reached the end of linked list (nested array initialliers) */
	if (!e) return TRUE;
	switch(e->kind) {
		case EXPR_INT_LIT:
		case EXPR_FLOAT_LIT:
		case EXPR_CHAR_LIT:
		case EXPR_STR_LIT:
		case EXPR_TRUE:
		case EXPR_FALSE:
			return TRUE;
		case EXPR_TERM:
		case EXPR_BLOCK:
			return expr_typecheck_constant(e->left) && expr_typecheck_constant(e->right);
		default:
			return FALSE;
	}
	return TRUE;
}