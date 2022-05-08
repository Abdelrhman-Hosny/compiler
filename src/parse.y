%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(char *s);
int yylex();
extern FILE *yyin;
extern int yylineno;
%}

// Start of Yacc definitions
%union {
    int intValue;
    double doubleValue;
    char charValue;
    char* stringValue;
    char* identifierName;
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
%token RETURN VOID
%token INT_DECLARATION FLOAT_DECLARATION CHAR_DECLARATION

%token <identifierName> ID 
%token <doubleValue> FLOAT
%token <intValue> INTEGER
%token <charValue> CHARACTER

%type <identifierName> assignment
%type <doubleValue> math_expr boolean_expr expression
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
%left  '*' '/' '%'

%precedence "then"
%nonassoc ELSE

%%

program: declaration_list 

declaration_list: declaration_list declaration |



declaration: function_declaration |
                variable_declaration


 /* Parameter lit for function definition ( parameter_list ) = ( int x, int y)*/
function_declaration: INT_DECLARATION ID '(' parameter_list ')' block |
                        FLOAT_DECLARATION ID '(' parameter_list ')' block |
                        CHAR_DECLARATION ID '(' parameter_list ')' block |
                        VOID ID '(' parameter_list ')' block

parameter_list: parameter |
                parameter_list ',' parameter|

parameter: INT_DECLARATION ID | CHAR_DECLARATION ID | FLOAT_DECLARATION ID

/* Function calls */

function_call: ID '(' argument_list ')'

argument_list: expression |
                argument_list ',' expression |


/* Variable declaration and assignment */
variable_declaration: INT_DECLARATION ID ';'|
                        FLOAT_DECLARATION ID ';'|
                        CHAR_DECLARATION ID ';'|
                        INT_DECLARATION assignment ';'|
                        FLOAT_DECLARATION assignment ';'|
                        CHAR_DECLARATION assignment ';'

assignment: ID '=' expression

expression : math_expr |
                boolean_expr

  /* mathematical expression //TODO: fix % */
math_expr : INTEGER {$$ = $1;} |
            FLOAT {$$ = $1;} |
            ID {;}|
            CHARACTER {$$ = $1;}|
            function_call {printf("function call");}|
            math_expr '+' math_expr { $$ = $1 + $3; } |
            math_expr '-' math_expr { $$ = $1 - $3; } |
            math_expr '*' math_expr { $$ = $1 * $3; } |
            math_expr '/' math_expr { $$ = $1 / $3; } |
            math_expr '%' math_expr { $$ = $1 - $3; } 


 /* logical expression */

boolean_expr : expression '>' expression {$$ = $1 > $3;}|
                expression '<' expression {$$ = $1 < $3;}|
                expression ">=" expression {$$ = $1 >= $3;}|
                expression "<=" expression {$$ = $1 <= $3;}|
               expression "==" expression {$$ = $1 == $3;}|
                expression "!=" expression {$$ = $1 != $3;}|
                expression "||" expression {$$ = $1 || $3;}|
                expression "&&" expression {$$ = $1 && $3;}|
                '!' expression { $$ = !$2; }


block : '{' stmt_list '}' |
        '{' '}'

stmt_list: stmt_list stmt |
            stmt

stmt : variable_declaration |
        assignment |
        expression |
        loop |
        conditional |
        BREAK ';' |
        CONTINUE ';' |
        RETURN ';' |
        RETURN expression ';' |
        ';'

 /* Loops */
optional_expression : expression |

for_assignment: INT_DECLARATION assignment |
                assignment |

loop: WHILE '(' optional_expression ')' block |
        FOR '(' for_assignment ';' optional_expression ';' for_assignment ')' block |
        DO block WHILE '(' optional_expression ')' ';'

 /* Conditional statements */

conditional: switch_case |
                uif |
                mif

uif : IF '(' expression ')' block |
        IF '(' expression ')' mif ELSE uif

mif : IF '(' expression ')' mif ELSE mif |
        block

switch_case: SWITCH '(' ID ')' '{' case_list '}'

case_list: case_list case_clause |
            DEFAULT ':' stmt_list|


case_clause: CASE '(' expression ')'':' stmt_list




%%

void yyerror(char *s)
{
    fprintf(stdout, "%s in line number: %d\n", s, yylineno);
}

int main(int argc, char* argv[])
{
    FILE * myFile;
    if (argc == 1) myFile = fopen("test.txt", "r");
    else myFile = fopen(argv[1], "r");

    if (!myFile)
    {
        printf("File not found\n");
        return -1;
    }

    yyin = myFile;

    do {
        yyparse();
    } while (!feof(yyin));
}

