#ifndef TOKEN_H
#define TOKEN_H

/* Identifier and assignment */
#define TOKEN_IDENT     255
#define TOKEN_ASSIGN    256

/* Keywords */
#define TOKEN_ARRAY     257
#define TOKEN_AUTO      258
#define TOKEN_VOID      259
#define TOKEN_IF        260
#define TOKEN_ELSE      261
#define TOKEN_FOR       262
#define TOKEN_WHILE     263
#define TOKEN_TRUE      264
#define TOKEN_FALSE     265
#define TOKEN_FUNC      266
#define TOKEN_PRINT     267
#define TOKEN_RETURN    268

/* Type keywords and literals */
#define TOKEN_INT       269
#define TOKEN_FLOAT     270
#define TOKEN_STR       271
#define TOKEN_CHAR      272
#define TOKEN_BOOL      273
#define TOKEN_INT_LIT   274
#define TOKEN_FLOAT_LIT 275
#define TOKEN_STR_LIT   276
#define TOKEN_CHAR_LIT  277

/* Logical operation */
#define TOKEN_NOT       278
#define TOKEN_AND       279
#define TOKEN_OR        280

/* Math operation */
#define TOKEN_EXP       281
#define TOKEN_MULT      282
#define TOKEN_DIV       283
#define TOKEN_MOD       284
#define TOKEN_ADD       285
/* Scanner cannot decide if this token is subtraction or unary negation */
#define TOKEN_SUB_OR_NEG       286
#define TOKEN_INCRE     287
#define TOKEN_DECRE     288
#define TOKEN_LT        289
#define TOKEN_LE        290
#define TOKEN_GT        291
#define TOKEN_GE        292
#define TOKEN_EQ        293
#define TOKEN_NE        294

/* Syntax */
#define TOKEN_LPAREN    295
#define TOKEN_RPAREN    296
#define TOKEN_LBRACKET  297
#define TOKEN_RBRACKET  298
#define TOKEN_LBRACE    299
#define TOKEN_RBRACE    300
#define TOKEN_SEMI      301
#define TOKEN_COLON     302
#define TOKEN_COMMA     303

/* Error */
#define TOKEN_ERROR     304

/* Scanner functions */
int yywrap();

#endif