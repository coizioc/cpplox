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

    Var* varDecl;

    Statement* stmt;
    Expression* exprStmt;
    Print* printStmt;

    Expr* expr;
    Number* number;
    Variable* variable;

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

%type <program> decls
%type <stmt> varDecl exprStmt printStmt stmt
%type <expr> number variable expr

%left PLUS MINUS
%left STAR SLASH

%start program

%%

program : decls { program = $<program>1; };

/* Declarations */
decls : decl { $$ = new Program(); $$->decls.push_back($<stmt>1); }
      | decls decl { $<program>1->decls.push_back($<stmt>2); }
      ;

decl : varDecl
     | stmt
     ;

varDecl : VAR variable EQUAL expr SEMICOLON { $$ = new Var(*$<variable>2, *$<expr>4); }
        ;

/* Statements */
stmt : exprStmt
     | printStmt
     ;

exprStmt : expr SEMICOLON { $$ = new Expression(*$<expr>1); }
         ;

printStmt : PRINT expr SEMICOLON { $$ = new Print(*$<expr>2); }
          ;

/* Expressions */
expr : number
     | variable
     | expr binop expr { $$ = new Binary(*$<expr>1, $<token>2, *$<expr>3); }
     ;

binop : PLUS | MINUS | STAR | SLASH
      ;

number : NUMBER {$$ = new Number(atof($1->c_str())); delete $1; }
       ;

variable : IDENTIFIER {$$ = new Variable(*$1); delete $1; }
