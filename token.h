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
#define TOKEN_NEG       278
#define TOKEN_NOT       279
#define TOKEN_AND       280
#define TOKEN_OR        281

/* Math operation */
#define TOKEN_EXP       282
#define TOKEN_MULT      283
#define TOKEN_DIV       284
#define TOKEN_MOD       285
#define TOKEN_ADD       286
#define TOKEN_SUB       287
#define TOKEN_INCRE     288
#define TOKEN_DECRE     289
#define TOKEN_LT        290
#define TOKEN_LE        291
#define TOKEN_GT        292
#define TOKEN_GE        293
#define TOKEN_EQ        294
#define TOKEN_NE        295

/* Syntax */
#define TOKEN_LPAREN    296
#define TOKEN_RPAREN    297
#define TOKEN_LBRACKET  298
#define TOKEN_RBRACKET  299
#define TOKEN_LBRACE    300
#define TOKEN_RBRACE    301
#define TOKEN_SEMI      302
#define TOKEN_COLON     303
#define TOKEN_COMMA     304

/* Error */
#define TOKEN_ERROR     305

#endif