/* Declaration of tokens to be included in parser.h, consumed by scanner.flex */

%token TOKEN_EOF

/* Identifier and assignment */
%token TOKEN_IDENT
%token TOKEN_ASSIGN

/* Keywords */
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_VOID
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_WHILE
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_FUNC
%token TOKEN_PRINT
%token TOKEN_RETURN

/* Type keywords and literals */
%token TOKEN_INT
%token TOKEN_FLOAT
%token TOKEN_STR
%token TOKEN_CHAR
%token TOKEN_BOOL
%token TOKEN_INT_LIT
%token TOKEN_FLOAT_LIT
%token TOKEN_STR_LIT
%token TOKEN_CHAR_LIT

/* Logical operation */
%token TOKEN_NOT
%token TOKEN_AND
%token TOKEN_OR

/* Math operation */
%token TOKEN_EXP
%token TOKEN_MULT
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_ADD

/* Scanner cannot decide if this token is subtraction or unary negation */
%token TOKEN_SUB_OR_NEG
%token TOKEN_INCRE
%token TOKEN_DECRE
%token TOKEN_LT
%token TOKEN_LE
%token TOKEN_GT
%token TOKEN_GE
%token TOKEN_EQ
%token TOKEN_NE

/* Syntax */
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LBRACE
%token TOKEN_RBRACE
%token TOKEN_SEMI
%token TOKEN_COLON
%token TOKEN_COMMA

/* Error */
%token TOKEN_ERROR

%{

%}

%%
%%

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}