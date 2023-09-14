#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF       ,
    /* Identifier and assignment */
    TOKEN_IDENT     ,
    TOKEN_ASSIGN    ,
    /* Keywords */
    TOKEN_ARRAY     ,
    TOKEN_AUTO      ,
    TOKEN_VOID      ,
    TOKEN_IF        ,
    TOKEN_ELSE      ,
    TOKEN_FOR       ,
    TOKEN_WHILE     ,
    TOKEN_TRUE      ,
    TOKEN_FALSE     ,
    TOKEN_FUNC      ,
    TOKEN_PRINT     ,
    TOKEN_RETURN    ,
    /* Type keywords and literals */
    TOKEN_INT       ,
    TOKEN_FLOAT     ,
    TOKEN_STR       ,
    TOKEN_CHAR      ,
    TOKEN_BOOL      ,
    TOKEN_INT_LIT   ,
    TOKEN_FLOAT_LIT ,
    TOKEN_STR_LIT   ,
    TOKEN_CHAR_LIT  ,
    /* Logical operation */
    TOKEN_NOT       ,
    TOKEN_AND       ,
    TOKEN_OR        ,
    /* Math operation */
    TOKEN_EXP       ,
    TOKEN_MULT      ,
    TOKEN_DIV       ,
    TOKEN_MOD       ,
    TOKEN_ADD       ,
    /* Scanner cannot decide if this token is subtraction or unary negation */
    TOKEN_SUB_OR_NEG,
    TOKEN_INCRE     ,
    TOKEN_DECRE     ,
    TOKEN_LT        ,
    TOKEN_LE        ,
    TOKEN_GT        ,
    TOKEN_GE        ,
    TOKEN_EQ        ,
    TOKEN_NE        ,
    /* Syntax */
    TOKEN_LPAREN    ,
    TOKEN_RPAREN    ,
    TOKEN_LBRACKET  ,
    TOKEN_RBRACKET  ,
    TOKEN_LBRACE    ,
    TOKEN_RBRACE    ,
    TOKEN_SEMI      ,
    TOKEN_COLON     ,
    TOKEN_COMMA     ,
    /* Error */
    TOKEN_ERROR
} token_t;



/* Scanner functions */
int yywrap();
int check_yyleng(token_t token, int yyleng);
int clean_string(token_t token, char* yytext);

#endif