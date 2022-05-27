%{
#include "mathOpUtils.h"
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
%type <expressionData> math_expr boolean_expr expression function_call
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
function_declaration:   INT_DECLARATION ID {createNewFunction(scopeCount,$2,INT_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,INT_TYPE)) exit(-1);} block  |
                        FLOAT_DECLARATION ID {createNewFunction(scopeCount,$2,FLOAT_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,FLOAT_TYPE)) exit(-1);} block |
                        CHAR_DECLARATION ID {createNewFunction(scopeCount,$2,CHAR_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,CHAR_TYPE)) exit(-1);} block |
                        VOID ID {createNewFunction(scopeCount,$2,VOID_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,VOID_TYPE)) exit(-1);} block

parameter_list: parameter|
                parameter_list ',' parameter|

parameter: INT_DECLARATION ID  {if(!addParameter($2,INT_TYPE,scopeCount)) exit(-1);}|
            CHAR_DECLARATION ID {if(!addParameter($2,CHAR_TYPE,scopeCount))  exit(-1);}|
            FLOAT_DECLARATION ID{if(!addParameter($2,FLOAT_TYPE,scopeCount))  exit(-1);}

//TODO: add Function calls
/* Function calls */
/* type expressionData type isValid = 0*/
function_call: ID '(' argument_list ')' {printf("ID function call: %s\n",$1);}

//we will make counter and check on type of each argrument
argument_list: expression {printf("type: %f\n",$1->type);}|
                argument_list ',' expression  {printf("type: %f\n",$3->type);}|

/* Variable declaration and assignment */
variable_declaration: INT_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT,0);
                        }
                        |
                        FLOAT_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT,0);
                        }
                        |
                        CHAR_DECLARATION ID ';'
                        {
                            createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT,0);
                        }
                        |
                        CONST_DECLARATION INT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, INT_TYPE, IS_CONSTANT,0);
                        }
                        |
                        CONST_DECLARATION FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, FLOAT_TYPE, IS_CONSTANT,0);
                        }
                        |
                        CONST_DECLARATION CHAR_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($3, currentScope, CHAR_TYPE, IS_CONSTANT,0);
                        }
                        |
                        INT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT,0);
                        }
                        |
                        FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT,0);
                        }
                        |
                        CHAR_DECLARATION  ID '=' expression ';'
                        {
                            createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT,0);
                        }


//TODO: check if variable exists in the parent scope
assignment: ID '=' expression 

expression : math_expr |
                boolean_expr

  /* mathematical expression //TODO: fix % */
math_expr : INTEGER 
            {
                $$ = createExpressionMacro;
                $$->type = INT_TYPE;
                $$->intValue = $1;
                $$->valueIsValid = 1;
            }
            |
            FLOAT 
            {
                $$ = createExpressionMacro;
                $$->type = FLOAT_TYPE;
                $$->doubleValue = $1;
                $$->valueIsValid = 1;
            }
            |
            ID 
            {
                // TODO
                // get variable value from sybmol table and handle the error

                // create a new expression data
                // $$ = createExpressionMacro
                // $$->type = variableType;
                
                // based on the type assign the value in the correct position
    

            }
            |
            CHARACTER 
            {
                $$ = createExpressionMacro;
                $$->type = CHAR_TYPE;
                $$->charValue = $1;          
                $$->valueIsValid = 1;
            }
            |
            function_call 
            {
                ;
            }
            |
            math_expr '+' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, ADD_OP);
               if (! $$) exit(-1);
            }
            |
            math_expr '-' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, SUB_OP);

               if (! $$) exit(-1);
            }
            |
            math_expr '*' math_expr 
            { 
               $$ = binaryMathExpression($$, $1, $3, MUL_OP);

               if (! $$) exit(-1);

            }
            |
            math_expr '/' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, DIV_OP);

                if (! $$) exit(-1);

            }
            |
            math_expr '%' math_expr 
            {
                
               $$ = binaryMathExpression($$, $1, $3, MOD_OP);

                if (! $$) exit(-1);

            }
            |
            '(' math_expr ')'   
            {
                $$ = $2;
            }
            |
            '-' math_expr %prec UNARY_MINUS 
            {
                if ($2->type == INT_TYPE)
                    $2->intValue = -$2->intValue;
                else if ($2->type == FLOAT_TYPE)
                    $2->doubleValue = -$2->doubleValue;
                else
                {
                    printf("Error: Unary minus can only be applied to int or float\n");
                    exit(1);
                }
                $$ = $2;

            }


 /* logical expression */

boolean_expr : expression '>' expression 
                {
                    
                }
                |
                expression '<' expression 
                {
                    
                }
                |
                expression ">=" expression 
                {
                    
                }
                |
                expression "<=" expression 
                {
                    
                }
                |
               expression "==" expression 
                {
                    
                }
                |
                expression "!=" expression 
                {
                    
                }
                |
                expression "||" expression 
                {
                    
                }
                |
                expression "&&" expression 
                {
                    
                }
                |
                '!' expression 
                {
                    
                } 
                |
                '(' boolean_expr ')'   
                {
                    
                
                }


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
        RETURN ';' | //TODO: check if return is in the correct place
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

