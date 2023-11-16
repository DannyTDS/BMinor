#include "helper.h"

int resolve_error = 0;
int typecheck_error = 0;

int typecheck() {
    if (resolve("simple") != SUCCESS) return FAILURE;
    decl_typecheck(root);
    if (typecheck_error != 0) {
        info("Typecheck failed: %d error(s)", typecheck_error);
        return FAILURE;
    }
    else {
        info("Typecheck successful!");
        return SUCCESS;
    }
}

int resolve(char* mode) {
    /* Halt on parse / scan error */
    if (parse("simple") != SUCCESS) return FAILURE;
    scope_enter();
    decl_resolve(root);
    scope_exit();
    if (resolve_error != 0) {
        info("Name resolution failed: %d error(s)", resolve_error);
        return FAILURE;
    } else {
        if (streq(mode, "verbose")) info("Name resolution successful!");
        return SUCCESS;
    }
}

int pprint() {
    /* Trivia: print operation will always return SUCCESS since it implies a valid BMinor program */
    /* Halt on parse / scan error */
    if (parse("simple") != SUCCESS) return FAILURE;
    decl_print(root, 0);
    return SUCCESS;
}

int parse(char* mode) {
    /* Halt on scan error */
    if (scan("simple") != SUCCESS) return FAILURE;
    rewind(yyin);
    if (yyparse() != SUCCESS) {
        info("Parse failed.");
        return FAILURE;
    }
    if (streq(mode, "verbose")) info("Parse successful!");
    return SUCCESS;
}

int scan(char* mode) {
    /* Loops over the input and scans the text */
    token_t token;
    while (1) {
        token = yylex();
        if (token == TOKEN_EOF) {
            break;
        }
        if (token == TOKEN_ERROR) {
            info("Scan failed.");
            return FAILURE;
        }
        if (streq(mode, "verbose")) print_token(token, yytext);
    }
    if (streq(mode, "verbose")) info("Scan successful!");
    return SUCCESS;
}

int encode(FILE* f) {
    /* decode and encode string */
    /* read input string */
    char input[BUFSIZ];
    fgets(input, BUFSIZ, f);
    /* remove trailing newline if one presents */
    if (input[strlen(input)-1] == '\n') {
        input[strlen(input)-1] = '\0';
    }
    /* excise the functions */
    char decoded[strlen(input)+1];
    if (string_decode(input, decoded) != SUCCESS) {
        error("Decode string error: cannot decode string %s.\n", input);
        return(FAILURE);
    } else {
        char encoded[strlen(input)+1];
        string_encode(decoded, encoded);
        info("Successfully decoded and encoded string.\n");
        info("Input  : %s\n", input);
        info("Decoded: %s\n", decoded);
        info("Encoded: %s\n", encoded);
    }
    /* return success upon completion */
    return SUCCESS;
}

int print_token(token_t token, char* yytext) {
    switch (token) {
        case TOKEN_IDENT:
            printf("%s\t%s\n", "IDENTIFIER", yytext);
            break;
        case TOKEN_ASSIGN:
            printf("%s\n", "ASSIGNMENT");
            break;
        case TOKEN_ARRAY:
            printf("%s\n", "ARRAY");
            break;
        case TOKEN_AUTO:
            printf("%s\n", "AUTO");
            break;
        case TOKEN_VOID:
            printf("%s\n", "VOID");
            break;
        case TOKEN_IF:
            printf("%s\n", "IF");
            break;
        case TOKEN_ELSE:
            printf("%s\n", "ELSE");
            break;
        case TOKEN_FOR:
            printf("%s\n", "FOR");
            break;
        case TOKEN_WHILE:
            printf("%s\n", "WHILE");
            break;
        case TOKEN_TRUE:
            printf("%s\n", "TRUE");
            break;
        case TOKEN_FALSE:
            printf("%s\n", "FALSE");
            break;
        case TOKEN_FUNC:
            printf("%s\n", "FUNCTION");
            break;
        case TOKEN_PRINT:
            printf("%s\n", "PRINT");
            break;
        case TOKEN_RETURN:
            printf("%s\n", "RETURN");
            break;
        case TOKEN_INT:
            printf("%s\n", "INTEGER");
            break;
        case TOKEN_FLOAT:
            printf("%s\n", "FLOAT");
            break;
        case TOKEN_STR:
            printf("%s\n", "STRING");
            break;
        case TOKEN_CHAR:
            printf("%s\n", "CHAR");
            break;
        case TOKEN_BOOL:
            printf("%s\n", "BOOLEAN");
            break;
        case TOKEN_INT_LIT:
            printf("%s\t%s\n", "INTEGER_LITERAL", yytext);
            break;
        case TOKEN_FLOAT_LIT:
            printf("%s\t%s\n", "FLOAT_LITERAL", yytext);
            break;
        /* For string and char literals, utilize string_decode to format printable string */
        case TOKEN_STR_LIT:
            printf("%s\t%s\n", "STRING_LITERAL", yytext);
            break;
        case TOKEN_CHAR_LIT:
            printf("%s\t%s\n", "CHAR_LITERAL", yytext);
            break;
        case TOKEN_NOT:
            printf("%s\n", "NOT");
            break;
        case TOKEN_AND:
            printf("%s\n", "AND");
            break;
        case TOKEN_OR:
            printf("%s\n", "OR");
            break;
        case TOKEN_EXP:
            printf("%s\n", "EXPONENT");
            break;
        case TOKEN_MULT:
            printf("%s\n", "MULTIPLY");
            break;
        case TOKEN_DIV:
            printf("%s\n", "DIVIDE");
            break;
        case TOKEN_MOD:
            printf("%s\n", "MODULO");
            break;
        case TOKEN_ADD:
            printf("%s\n", "ADD");
            break;
        case TOKEN_SUB:
            printf("%s\n", "SUBTRACT");
            break;
        case TOKEN_INCRE:
            printf("%s\n", "INCREMENT");
            break;
        case TOKEN_DECRE:
            printf("%s\n", "DECREMENT");
            break;
        case TOKEN_LT:
            printf("%s\n", "LT");
            break;
        case TOKEN_LE:
            printf("%s\n", "LE");
            break;
        case TOKEN_GT:
            printf("%s\n", "GT");
            break;
        case TOKEN_GE:
            printf("%s\n", "GE");
            break;
        case TOKEN_EQ:
            printf("%s\n", "EQ");
            break;
        case TOKEN_NE:
            printf("%s\n", "NE");
            break;
        case TOKEN_LPAREN:
            printf("%s\n", "PARENTHESIS_L");
            break;
        case TOKEN_RPAREN:
            printf("%s\n", "PARENTHESIS_R");
            break;
        case TOKEN_LBRACKET:
            printf("%s\n", "BRACKET_L");
            break;
        case TOKEN_RBRACKET:
            printf("%s\n", "BRACKET_R");
            break;
        case TOKEN_LBRACE:
            printf("%s\n", "BRACE_L");
            break;
        case TOKEN_RBRACE:
            printf("%s\n", "BRACE_R");
            break;
        case TOKEN_SEMI:
            printf("%s\n", "SEMICOLON");
            break;
        case TOKEN_COLON:
            printf("%s\n", "COLON");
            break;
        case TOKEN_COMMA:
            printf("%s\n", "COMMA");
            break;
        default:
            /* Halt on scan error */
            return FAILURE;
    }

    return SUCCESS;
}