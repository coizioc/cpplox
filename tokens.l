%{
#include <string>
#include "node.hpp"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}

%option noyywrap
%option yylineno

%%

[ \t\n]                 ;
"//".*$                 ;
"("                     return TOKEN(LEFT_PAREN);
")"                     return TOKEN(RIGHT_PAREN);
"{"                     return TOKEN(LEFT_BRACE);
"}"                     return TOKEN(RIGHT_BRACE);
","                     return TOKEN(COMMA);
"."                     return TOKEN(DOT);
"-"                     return TOKEN(MINUS);
"+"                     return TOKEN(PLUS);
";"                     return TOKEN(SEMICOLON);
"/"                     return TOKEN(SLASH);
"*"                     return TOKEN(STAR);

"!"                     return TOKEN(BANG);
"!="                    return TOKEN(BANG_EQUAL);
"="                     return TOKEN(EQUAL);
"=="                    return TOKEN(EQUAL_EQUAL);
">"                     return TOKEN(GREATER);
">="                    return TOKEN(GREATER_EQUAL);
"<"                     return TOKEN(LESS);
"<="                    return TOKEN(LESS_EQUAL);

"and"                   return TOKEN(AND);
"class"                 return TOKEN(CLASS);
"else"                  return TOKEN(ELSE);
"false"                 return TOKEN(FALSE);
"fun"                   return TOKEN(FUN);
"for"                   return TOKEN(FOR);
"if"                    return TOKEN(IF);
"nil"                   return TOKEN(NIL);
"or"                    return TOKEN(OR);
"print"                 return TOKEN(PRINT);
"return"                return TOKEN(RETURN);
"super"                 return TOKEN(SUPER);
"this"                  return TOKEN(THIS);
"var"                   return TOKEN(VAR);
"while"                 return TOKEN(WHILE);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return IDENTIFIER;
[0-9]+(\.[0-9]+)?       SAVE_TOKEN; return NUMBER;
\".*\"                  SAVE_TOKEN; return STRING;

.                       perror("Unknown token!\n"); yyterminate();

%%