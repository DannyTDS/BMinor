% {
#include "token.h"
% }

DIGIT [0-9]
LETTER [A-Za-z]

%%

/* Literals */
[1-9]{DIGIT}+       { return TOKEN_INT_LIT; }
=                   { return TOKEN_ASSIGN; }

/* Logical operations */
!                   { return TOKEN_NOT; }
&&                  { return TOKEN_AND; }
\|\|                { return TOKEN_OR; }


/* Math operations */
\^                  { return TOKEN_EXP; }
\*                  { return TOKEN_MULT; }
\/                  { return TOKEN_DIV; }
%                   { return TOKEN_MOD; }
\+                  { return TOKEN_ADD; }
-                   { return TOKEN_SUB; }
\+\+                { return TOKEN_INCRE; }
--                  { return TOKEN_DECRE; }
<                   { return TOKEN_LT; }
<=                  { return TOKEN_LE; }
>                   { return TOKEN_GT; }
>=                  { return TOKEN_GE; }
==                  { return TOKEN_EQ; }
!=                  { return TOKEN_NE; }

/* Syntax */
\(                  { return TOKEN_LPAREN; }
\)                  { return TOKEN_RPAREN; }
\[                  { return TOKEN_LBRACKET; }
\]                  { return TOKEN_RBRACKET; }
\{                  { return TOKEN_LBRACE; }
\}                  { return TOKEN_RBRACE; }
;                   { return TOKEN_SEMI; }
:                   { return TOKEN_COLON; }
,                   { return TOKEN_COMMA; }

%%