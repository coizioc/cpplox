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

#ifndef YY_YY_SRC_PARSER_HPP_INCLUDED
#define YY_YY_SRC_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype {
    IDENTIFIER = 258,
    NUMBER = 259,
    STRING = 260,
    LEFT_PAREN = 261,
    RIGHT_PAREN = 262,
    LEFT_BRACE = 263,
    RIGHT_BRACE = 264,
    COMMA = 265,
    DOT = 266,
    MINUS = 267,
    PLUS = 268,
    SEMICOLON = 269,
    SLASH = 270,
    STAR = 271,
    BANG = 272,
    BANG_EQUAL = 273,
    EQUAL = 274,
    EQUAL_EQUAL = 275,
    GREATER = 276,
    GREATER_EQUAL = 277,
    LESS = 278,
    LESS_EQUAL = 279,
    AND = 280,
    CLASS = 281,
    ELSE = 282,
    FALSE = 283,
    FUN = 284,
    FOR = 285,
    IF = 286,
    NIL = 287,
    OR = 288,
    PRINT = 289,
    RETURN = 290,
    SUPER = 291,
    THIS = 292,
    TRUE = 293,
    VAR = 294,
    WHILE = 295
};
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED

union YYSTYPE {
#line 12 "parser.y" /* yacc.c:1909  */

    Node* node;
    Program* program;

    Declaration* decl;

    Statement* stmt;
    Print* printStmt;

    Expression* expr;
    Number* number;

    std::string* string;
    int token;

#line 111 "src/parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

int yyparse(void);

#endif /* !YY_YY_SRC_PARSER_HPP_INCLUDED  */
