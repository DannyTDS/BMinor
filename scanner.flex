% {
#include "token.h"
% }

DIGIT [0-9]
LETTER [A-Za-z]

%%

/* Keywords */
array               { return TOKEN_ARRAY; }
auto                { return TOKEN_AUTO; }
void                { return TOKEN_VOID; }
if                  { return TOKEN_IF; }
else                { return TOKEN_ELSE; }
for                 { return TOKEN_FOR; }
while               { return TOKEN_WHILE; }
true                { return TOKEN_TRUE; }
false               { return TOKEN_FALSE; }
function            { return TOKEN_FUNC; }
print               { return TOKEN_PRINT; }
return              { return TOKEN_RETURN; }

/* Types */
integer             { return TOKEN_INT; }
float               { return TOKEN_FLOAT; }
string              { return TOKEN_STR; }
char                { return TOKEN_CHAR; }
boolean             { return TOKEN_BOOL; }

/* Literals */
[1-9]{DIGIT}+       { return TOKEN_INT_LIT; }

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
=                   { return TOKEN_ASSIGN; }
\(                  { return TOKEN_LPAREN; }
\)                  { return TOKEN_RPAREN; }
\[                  { return TOKEN_LBRACKET; }
\]                  { return TOKEN_RBRACKET; }
\{                  { return TOKEN_LBRACE; }
\}                  { return TOKEN_RBRACE; }
;                   { return TOKEN_SEMI; }
:                   { return TOKEN_COLON; }
,                   { return TOKEN_COMMA; }

/* Capture error */
.                   { return TOKEN_ERROR; }

%%