#ifndef TOKEN_H
#define TOKEN_H

/* Identifier and assignment */
#define TOKEN_IDENT
#define TOKEN_ASSIGN

/* Type keywords and literals */
#define TOKEN_INT       
#define TOKEN_FLOAT     
#define TOKEN_STR       
#define TOKEN_CHAR      
#define TOKEN_BOOL      
#define TOKEN_INT_LIT   
#define TOKEN_FLOAT_LIT 
#define TOKEN_STR_LIT   
#define TOKEN_CHAR_LIT  

/* Logical operation */
#define TOKEN_NEG
#define TOKEN_NOT
#define TOKEN_AND
#define TOKEN_OR

/* Math operation */
#define TOKEN_EXP
#define TOKEN_MULT
#define TOKEN_DIV
#define TOKEN_MOD
#define TOKEN_ADD
#define TOKEN_SUB
#define TOKEN_INCRE
#define TOKEN_DECRE
#define TOKEN_LT
#define TOKEN_LE
#define TOKEN_GT
#define TOKEN_GE
#define TOKEN_EQ
#define TOKEN_NE

/* Syntax */
#define TOKEN_LPAREN
#define TOKEN_RPAREN
#define TOKEN_LBRACKET
#define TOKEN_RBRACKET
#define TOKEN_LBRACE
#define TOKEN_RBRACE
#define TOKEN_SEMI
#define TOKEN_COLON
#define TOKEN_COMMA

/* Error */
#define TOKEN_ERROR

#endif