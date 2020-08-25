%{
#include "node.hpp"

Program* program;

extern int yylex();
extern int yylineno;
void yyerror(const char* s) { fprintf(stderr, "ERROR [line %d]: %s\n", yylineno, s); }
%}

%define parse.error verbose

%union {
    Node* node;
    Program* program;

    Declaration* decl;

    Statement* stmt;
    Print* printStmt;

    Expression* expr;
    Number* number;

    std::string* string;
    int token;
}

%token <string> IDENTIFIER NUMBER STRING
%token <token> LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token <token> COMMA DOT MINUS PLUS SEMICOLON SLASH STAR
%token <token> BANG BANG_EQUAL EQUAL EQUAL_EQUAL
%token <token> GREATER GREATER_EQUAL LESS LESS_EQUAL
%token <token> AND CLASS ELSE FALSE FUN FOR IF NIL OR
%token <token> PRINT RETURN SUPER THIS TRUE VAR WHILE

%type <program> stmts
%type <stmt> printStmt stmt
%type <expr> number expr

%left PLUS MINUS
%left STAR SLASH

%start program

%%

program : stmts { program = $<program>1; };

/* Statements */
stmts : stmt { $$ = new Program(); $$->decls.push_back($<stmt>1); }
      | stmts stmt { $<program>1->decls.push_back($<stmt>2); }

stmt : printStmt
     ;

printStmt : PRINT expr SEMICOLON { $$ = new Print(*$<expr>2); }
          ;

/* Expressions */
expr : number
     | expr binop expr { $$ = new Binary(*$<expr>1, $<token>2, *$<expr>3); }
     ;

binop : PLUS | MINUS | STAR | SLASH
      ;

number : NUMBER {$$ = new Number(atof($1->c_str())); delete $1; }
       ;
