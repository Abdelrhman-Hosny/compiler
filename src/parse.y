%{
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "symbolTable.h"
#include "dataStructures.h"
#include <string.h>
void yyerror(char *s);
int yylex();
extern FILE *yyin;
extern int yylineno;
// when we exit current scope, we return to the parent
int currentScope = 0, scopeCount = 0;
%}

// Start of Yacc definitions
%union {
    int intValue;
    double doubleValue;
    char charValue;
    char* stringValue;
    char* identifierName;
    struct ExpressionData* expressionData;
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
%token INT_DECLARATION FLOAT_DECLARATION CHAR_DECLARATION CONST_DECLARATION

%token <identifierName> ID 
%token <doubleValue> FLOAT
%token <intValue> INTEGER
%token <charValue> CHARACTER

%type <identifierName> assignment
%type <expressionData> math_expr boolean_expr expression
%type <stringValue> parameter_list parameter
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


%nonassoc IFX
%nonassoc ELSE

%nonassoc UNARY_MINUS
%%

program: declaration_list 

declaration_list: declaration_list declaration |



declaration: function_declaration |
                variable_declaration

 /* Parameter lit for function definition ( parameter_list ) = ( int x, int y)*/
function_declaration:   INT_DECLARATION ID {  printf("ID: %s\n",$2);} '(' parameter_list ')'  
                        {createFunction($2,$2,INT_TYPE); } block  |
                        FLOAT_DECLARATION ID '(' parameter_list ')' block |
                        CHAR_DECLARATION ID '(' parameter_list ')' block |
                        VOID ID '(' parameter_list ')' block

parameter_list: parameter|
                parameter_list ',' parameter|

parameter: INT_DECLARATION ID  {printf("parameter: %s\n",$2)}|
            CHAR_DECLARATION ID |
            FLOAT_DECLARATION ID

/* Function calls */

function_call: ID '(' argument_list ')' 

argument_list: expression |
                argument_list ',' expression |

/* Variable declaration and assignment */
variable_declaration: INT_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT);
                        }
                        |
                        FLOAT_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT);
                        }
                        |
                        CHAR_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT);
                        }
                        |
                        CONST_DECLARATION INT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, INT_TYPE, IS_CONSTANT);
                        }
                        |
                        CONST_DECLARATION FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, FLOAT_TYPE, IS_CONSTANT);
                        }
                        |
                        CONST_DECLARATION CHAR_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, CHAR_TYPE, IS_CONSTANT);
                        }
                        |
                        INT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT);
                        }
                        |
                        FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT);
                        }
                        |
                        CHAR_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT);
                        }


assignment: ID '=' expression 

expression : math_expr |
                boolean_expr

  /* mathematical expression //TODO: fix % */
math_expr : INTEGER 
            {
                $$ = (struct ExpressionData*) malloc(sizeof(struct ExpressionData));
                $$->type = INT_TYPE;
                $$->intValue = $1;
            }
             |
            FLOAT 
            {
                $$ = (struct ExpressionData*) malloc(sizeof(struct ExpressionData));
                $$->type = FLOAT_TYPE;
                $$->doubleValue = $1;
            }
             |
            ID 
            {
                ;
            }
            |
            CHARACTER 
            {
                $$ = (struct ExpressionData*) malloc(sizeof(struct ExpressionData));
                $$->type = CHAR_TYPE;
                $$->charValue = $1;          
            }
            |
            function_call 
            {
                ;
            }
            |
            math_expr '+' math_expr 
            {
            }
             |
            math_expr '-' math_expr 
            {
            }
             |
            math_expr '*' math_expr 
            {
            }
             |
            math_expr '/' math_expr 
            {
            }
             |
            math_expr '%' math_expr 
            {
                ;
            }
             |
            '(' math_expr ')'   
            {
            }
             |
            '-' math_expr %prec UNARY_MINUS 
            {
            
            }


 /* logical expression */

boolean_expr : expression '>' expression {}|
                expression '<' expression {}|
                expression ">=" expression {}|
                expression "<=" expression {}|
               expression "==" expression {}|
                expression "!=" expression {}|
                expression "||" expression {}|
                expression "&&" expression {}|
                '!' expression {} |
                '(' boolean_expr ')'   {}


block : '{' {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount;} stmt_list '}' {currentScope = getParentScope(currentScope);} |
        '{' {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount;} '}' {currentScope = getParentScope(currentScope);}

stmt_list: stmt_list stmt |
            stmt

stmt : variable_declaration |
        assignment ';'|
        expression ';' |
        loop |
        conditional |
        BREAK ';' |
        CONTINUE ';' |
        RETURN ';' |
        RETURN expression ';' |
        block |
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
            if_statement

if_statement: IF  '(' expression  ')' stmt end_if 

end_if: %prec IFX | ELSE  stmt 

switch_case: SWITCH '(' ID ')' '{'{createNewScope(currentScope,scopeCount);currentScope = ++scopeCount;} case_list '}' {currentScope = getParentScope(currentScope);}

case_list:  case_clause case_list|
            DEFAULT ':' stmt_list|


case_clause: CASE  expression ':' stmt_list




%%
void yyerror(char *s)
{
    fprintf(stdout, "%s in line number: %d\n", s, yylineno);
}

int main(int argc, char* argv[])
{
    FILE * myFile;
    if (argc == 1) myFile = fopen("test.c", "r");
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

    printSymbolTable();
}

