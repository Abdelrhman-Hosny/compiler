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

%precedence "then"
%nonassoc ELSE

%%
stmt: ';' |
    expr |
    LOOP |
    CONDITIONAL 

CONDITIONAL : SWITCH_CASE |
                UIF

MIF : IF '(' expr ')' '{' MIF '}' ELSE  '{' MIF '}' |
        stmt

UIF : IF '(' expr ')'  stmt %prec "then" |
        IF '(' expr ')' '{' MIF '}' ELSE UIF

SWITCH_CASE : SWITCH '(' expr ')' '{' SWITCH_STMT_LIST '}'


SWITCH_STMT_LIST: CASE '(' ID ')' ':' stmt SWITCH_STMT_LIST |
            CASE '(' ID ')' ':' stmt |
            DEFAULT ':' stmt 


LOOP : WHILE '(' expr ')' '{' stmt '}' |
    FOR '(' expr ';' expr ';' expr ')' '{' stmt  '}'|
    DO stmt WHILE '(' expr ')' ';'

expr : expr '+' term
    | expr '-' term
    | term

term : term '*' factor
    | term '/' factor
    | factor

factor: '(' expr ')'
    | INTEGER
%%

main(int argc, char **argv)
{
    yyparse();
}

yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}