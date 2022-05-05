%{
void yerror(char *s);
int yylex();

#include <stdio.h>
%}

// Start of Yacc definitions
%union {
    int intValue;
    double doubleValue;
    char charValue;
    char* stringValue;
}

%start stmt
%token 
    GEQ ">="  
    LEQ "<="
    EQ "=="
    NEQ "!="
    AND "&&"
    OR "||"

%token WHILE DO FOR BREAK CONTINUE 
%token IF ELSE SWITCH CASE DEFAULT
%token RETURN
%token ID INTEGER FLOAT CHARACTER

/*
by declaring %left '+' before %left '*', this gives precedence to '*'
the lower you declare something, the higher precedence it has
*/ /* 
If you want to give something precedence without declaring 
associativity simply use %precedence
*/
%left '<' '>' "<=" ">=" "==" "!="
%left '+' '-'
%left  '*' '/'

%%
stmt: GEQ {printf("test");}
%%

main(int argc, char **argv)
{
    yyparse();
}

yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}