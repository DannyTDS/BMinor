#include "scanner.h"

int scan(FILE* input) {
    yyin = input;

    /* Loops over the input and scans the text */
    token_t token;
    do {
        token = yylex();
        if (print_token(token, yytext) != SUCCESS) {
            return FAILURE;
        };
    } while (token != TOKEN_EOF);
    return SUCCESS;
}

int print_token(token_t token, char* yytext) {
    /* Reached end of input */
    if (token == TOKEN_EOF) return SUCCESS;

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
            if (decode_yytext(token, yytext) == SUCCESS) {
                printf("%s\t%s\n", "STRING_LITERAL", yytext);
            } else {
                printf("%s\t%s\n", "STRING_LITERAL", "Error in decoding the string.");
                return FAILURE;
            }
            break;
        case TOKEN_CHAR_LIT:
            if (decode_yytext(token, yytext) == SUCCESS) {
                printf("%s\t%s\n", "CHAR_LITERAL", yytext);
            } else {
                printf("%s\t%s\n", "CHAR_LITERAL", "Error in decoding the character.");
                return FAILURE;
            }
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
        case TOKEN_SUB_OR_NEG:
            printf("%s\n", "SUBTRACT_OR_NEGATE");
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
        /* Halt on scan error */
        case TOKEN_ERROR:
            printf("Scan error: %s is not a valid character.\n", yytext);
            return FAILURE;
        default:
            printf("Scan error: unknown token %d scanned from text %s.\n", token, yytext);
            return FAILURE;
    }

    return SUCCESS;
}

int decode_yytext(token_t token, char* yytext) {
    /* Calls the string_decode function and set yytext to the decoded string */
    /* From the scanner, we are promised that the literal is closed by '' or "" */
    if (token == TOKEN_CHAR_LIT) {
        /* Tricks the decode function by replacing ' with " */
        char* head = yytext;
        *yytext = '\"';
        while (*yytext) {
            if (*yytext == '\'' && *(yytext+1) == '\0') {
                *yytext = '\"';
            }
            yytext++;
        }
        yytext = head;
    }
    char decoded[MAXSTRLEN + 1];
    int status = string_decode(yytext, decoded);
    if (status == SUCCESS) {
        strcpy(yytext, decoded);
    }
    return status;
}