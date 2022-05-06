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

%start program
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
%token INT_DECLARATION FLOAT_DECLARATION CHAR_DECLARATION
%token VOID

/*
by declaring %left '+' before %left '*', this gives precedence to '*'
the lower you declare something, the higher precedence it has
*/ /* 
If you want to give something precedence without declaring 
associativity simply use %precedence
*/
%left "&&" "||"
%left '<' '>' "<=" ">=" "==" "!="
%left '!'
%left '+' '-'
%left  '*' '/'

%precedence "then"
%nonassoc ELSE

%%

program: declaration_list 

declaration_list: declaration_list declaration |



declaration: function_declaration |
                variable_declaration


 /* Parameter lit for function definition ( parameter_list ) = ( int x, int y)*/
function_declaration: function_return_type ID '(' parameter_list ')' block

parameter_list: parameter |
                parameter_list ',' parameter|

parameter: data_type ID

/* Function calls */

function_call: ID '(' argument_list ')'

argument_list: argument |
                argument_list ',' argument |

argument: ID |
            expression


/* Variable declaration and assignment */
variable_declaration: data_type ID ';'|
                        data_type assignment

 /* TODO: replace expression with expression + function call */
assignment: ID '=' expression ';'

expression : math_expr |


  /* mathematical expression */
math_expr : math_expr '+' term
    | math_expr '-' term
    | term

term : term '*' factor
    | term '/' factor
    | factor

factor: '(' math_expr ')'|
        INTEGER |
        FLOAT |
        ID

 /* logical expression */

  /* Data types and return types*/
data_type : INT_DECLARATION |
            FLOAT_DECLARATION |
            CHAR_DECLARATION

function_return_type: data_type |
                        VOID







stmt: ';' |
    expr |
    LOOP |
    CONDITIONAL 

CONDITIONAL : SWITCH_CASE |
                UIF |
                MIF

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
%%

main(int argc, char **argv)
{
    yyparse();
}

yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}