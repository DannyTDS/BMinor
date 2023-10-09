/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDE_TOKEN_H_INCLUDED
# define YY_YY_INCLUDE_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_IDENT = 258,
    TOKEN_ASSIGN = 259,
    TOKEN_ARRAY = 260,
    TOKEN_AUTO = 261,
    TOKEN_VOID = 262,
    TOKEN_IF = 263,
    TOKEN_ELSE = 264,
    TOKEN_FOR = 265,
    TOKEN_WHILE = 266,
    TOKEN_TRUE = 267,
    TOKEN_FALSE = 268,
    TOKEN_FUNC = 269,
    TOKEN_PRINT = 270,
    TOKEN_RETURN = 271,
    TOKEN_INT = 272,
    TOKEN_FLOAT = 273,
    TOKEN_STR = 274,
    TOKEN_CHAR = 275,
    TOKEN_BOOL = 276,
    TOKEN_INT_LIT = 277,
    TOKEN_FLOAT_LIT = 278,
    TOKEN_STR_LIT = 279,
    TOKEN_CHAR_LIT = 280,
    TOKEN_NOT = 281,
    TOKEN_AND = 282,
    TOKEN_OR = 283,
    TOKEN_EXP = 284,
    TOKEN_MULT = 285,
    TOKEN_DIV = 286,
    TOKEN_MOD = 287,
    TOKEN_ADD = 288,
    TOKEN_SUB = 289,
    TOKEN_INCRE = 290,
    TOKEN_DECRE = 291,
    TOKEN_LT = 292,
    TOKEN_LE = 293,
    TOKEN_GT = 294,
    TOKEN_GE = 295,
    TOKEN_EQ = 296,
    TOKEN_NE = 297,
    TOKEN_LPAREN = 298,
    TOKEN_RPAREN = 299,
    TOKEN_LBRACKET = 300,
    TOKEN_RBRACKET = 301,
    TOKEN_LBRACE = 302,
    TOKEN_RBRACE = 303,
    TOKEN_SEMI = 304,
    TOKEN_COLON = 305,
    TOKEN_COMMA = 306,
    TOKEN_ERROR = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 71 "src/parser.y" /* yacc.c:1909  */

	struct decl* decl;
	struct expr* expr;
	struct stmt* stmt;
	struct type* type;
	struct param_list* param_list;

#line 115 "include/token.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_TOKEN_H_INCLUDED  */
