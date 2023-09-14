%option nounput
%option noinput

%{
#include "token.h"
%}

DIGIT [0-9]
LETTER [A-Za-z]

%%

    /* Literals */
[-\+]?{DIGIT}+                                  { return TOKEN_INT_LIT; }
[-\+]?{DIGIT}*(.{DIGIT}+|[eE][-\+]?{DIGIT}+)    { return TOKEN_FLOAT_LIT; }
\'([^\\\'\n]|\\.|\\0x..)\'                      { return TOKEN_CHAR_LIT; }
\"([^\\\"\n]|\\.){0,255}\"                      { return TOKEN_STR_LIT; }

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
-                   { return TOKEN_SUB_OR_NEG; }
\+\+                { return TOKEN_INCRE; }
--                  { return TOKEN_DECRE; }
\<                  { return TOKEN_LT; }
\<=                 { return TOKEN_LE; }
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

    /* Identifier */
[A-Za-z_][A-Za-z0-9_]{0,254}     { return TOKEN_IDENT; }

    /* Comments */
\/\/[^\n]*\n                        { ; /* eat up C++ Comment */ }
\/\*([^\*]|\*+[^\/])*\*+\/          { ; /* eat up C Comment */   }

[ \n\t]+            { ; /* eat up white space */ }

    /* Capture error */
.                   { return TOKEN_ERROR; }

%%

int yywrap() {
    /* Reached end of input */
    return 1;
}