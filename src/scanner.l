%option nounput
%option noinput

%{
#include "utils.h"
#include "token.h"

int yywrap();
int check_yyleng(token_t token, int yyleng);
int clean_string(token_t token, char* yytext);

extern int string_decode(char* es, char* s);
%}

DIGIT [0-9]

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
-                   { return TOKEN_SUB; }       // Scanner cannot decide between subtract and unary negation
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

    /* Literals */
{DIGIT}+                                                    { return TOKEN_INT_LIT; }
{DIGIT}*\.?{DIGIT}+([eE][-\+]?{DIGIT}+)?                    { return TOKEN_FLOAT_LIT; }
\'([^\\\'\n]|\\.|\\0x..)\'                                  {
                                                                if (clean_string(TOKEN_CHAR_LIT, yytext) != SUCCESS) return TOKEN_ERROR;
                                                                return TOKEN_CHAR_LIT; 
                                                            }
\"([^\\\"\n]|\\.)*\"                                        {
                                                                if (clean_string(TOKEN_STR_LIT, yytext) != SUCCESS) return TOKEN_ERROR;
                                                                return TOKEN_STR_LIT;
                                                            }

    /* Identifier */
[A-Za-z_][A-Za-z0-9_]*  {
                            if (check_yyleng(TOKEN_IDENT, yyleng) != SUCCESS) return TOKEN_ERROR;
                            return TOKEN_IDENT; 
                        }

    /* Comments */
\/\/[^\n]*                            { ; /* eat up C++ Comment */ }
\/\*([^\*]|\*+[^\/\*])*\*+\/          { ; /* eat up C Comment */   }

[ \n\t]+            { ; /* eat up white space */ }

    /* Capture error */
.                   { return TOKEN_ERROR; }

%%

int yywrap() {
    /* Reached end of input */
    return 1;
}

int check_yyleng(token_t token, int yyleng) {
    if (yyleng > 255) {
        switch (token) {
            case TOKEN_IDENT:
                error("Scan error: TOKEN_IDENT over 255 characters long.");
                break;
            case TOKEN_STR_LIT:
                error("Scan error: TOKEN_STR_LIT over 255 characters long.");
                break;
            default:
                break;
        }
        return FAILURE;
    }
    return SUCCESS;
}

int clean_string(token_t token, char* yytext) {
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
    char decoded[BUFSIZ];
    /* Call the decode function */
    int status = string_decode(yytext, decoded);
    if (status != SUCCESS) {
        switch (token) {
            case TOKEN_CHAR_LIT:
                error("Scan error: TOKEN_CHAR_LIT token decode error.");
                break;
            case TOKEN_STR_LIT:
                error("Scan error: TOKEN_STR_LIT token decode error.");
                break;
            default:
                break;
        }
    }
    strcpy(yytext, decoded);
    if (status == SUCCESS && check_yyleng(token, strlen(decoded)) == SUCCESS) {
        return SUCCESS;
    } else {
        return FAILURE;
    }
}