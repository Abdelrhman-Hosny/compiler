%{
#include <stdio.h>
#include <stdlib.h>
#include "mathOpUtils.h"
#include "symbolTable.h"
#include "quadruples.h"
#include <string.h>
void yyerror(char *s);
int yylex();
extern FILE *yyin;
extern int yylineno;
// when we exit current scope, we return to the parent
int currentScope = 0, scopeCount = 0;
int currentTempCount = 0;
%}

// Start of Yacc definitions
%union {
    int intValue;
    double doubleValue;
    char charValue;
    char* stringValue;
    char* identifierName;
    struct ExpressionData* expressionData;
    struct FunctionCallParameters* functionCallParameters;
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

%type <identifierName> assignment for_assignment
%type <expressionData> math_expr boolean_expr expression function_call optional_expression loop
%type <stringValue> parameter_list parameter
%type <functionCallParameters> argument_list
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

// TODO : Add func declaration quadruple
 /* Parameter lit for function definition ( parameter_list ) = ( int x, int y)*/
function_declaration:   INT_DECLARATION ID {createNewFunction(scopeCount,$2,INT_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,INT_TYPE)) exit(-1);} block  |
                        FLOAT_DECLARATION ID {createNewFunction(scopeCount,$2,FLOAT_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,FLOAT_TYPE)) exit(-1);} block |
                        CHAR_DECLARATION ID {createNewFunction(scopeCount,$2,CHAR_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,CHAR_TYPE)) exit(-1);} block |
                        VOID ID {createNewFunction(scopeCount,$2,VOID_TYPE);} '(' parameter_list ')' {if(!checkFunctionExists(scopeCount,VOID_TYPE)) exit(-1);} block

parameter_list: parameter|
                parameter_list ',' parameter|

parameter:  INT_DECLARATION ID  
                {
                    if(addParameter($2,INT_TYPE,scopeCount)){
                        std::string variableName($2);
                        pushToStack(variableName);
                    }
                }
                |
            CHAR_DECLARATION ID 
                {
                    if(addParameter($2,CHAR_TYPE,scopeCount)){

                        std::string variableName($2);
                        pushToStack(variableName);
                    }
                }
                |
            FLOAT_DECLARATION ID
                {
                    if(addParameter($2,FLOAT_TYPE,scopeCount)){

                        std::string variableName($2);
                        pushToStack(variableName);
                    }
                }

/* Function calls */
/* type expressionData type isValid = 0*/
function_call: ID '(' argument_list ')' {
                int returnType = checkArgumentList($1,$3); if(returnType == -1) exit(-1);

                // quadruple creation
                // we don't increase temp count as if the function is void, we won't use it
                // so we check for that inside the fn
                currentTempCount = functionCallQuadruple($1, $3, returnType, currentTempCount);
                // ExpressionData creation
                $$ = createExpressionMacro;
                $$->type = returnType;
                $$->valueIsValid = 0;
                }

//we will make counter and check on type of each argrument
argument_list: expression {$$ = new struct FunctionCallParameters(); $$->parameterTypes.push_back($1->type);}|
                argument_list ',' expression  {$$->parameterTypes.push_back($3->type);}|

/* Variable declaration and assignment */
variable_declaration: INT_DECLARATION ID ';'
                        {
                            int ret = createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                        }
                        |
                        FLOAT_DECLARATION ID ';'
                        {
                            int ret = createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                        }
                        |
                        CHAR_DECLARATION ID ';'
                        {
                            int ret = createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                        }
                        |
                        CONST_DECLARATION INT_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($3, currentScope, INT_TYPE, IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                            
                            ret = assignVariable($3, $5, currentScope);
                            if (ret == -1) exit(-1);
                            
                            std::string assignmentDestination($3);
                            assignmentQuadruple(assignmentDestination);

                        }
                        |
                        CONST_DECLARATION FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($3, currentScope, FLOAT_TYPE, IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                            ret = assignVariable($3, $5, currentScope);
                            if (ret == -1) exit(-1);

                            std::string assignmentDestination($3);
                            assignmentQuadruple(assignmentDestination);

                        }
                        |
                        CONST_DECLARATION CHAR_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($3, currentScope, CHAR_TYPE, IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                            
                            ret = assignVariable($3, $5, currentScope);
                            if (ret == -1) exit(-1);

                            std::string assignmentDestination($3);
                            assignmentQuadruple(assignmentDestination);
                        }
                        |
                        INT_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);
                     
                            ret = assignVariable($2, $4, currentScope);
                            if (ret == -1) exit(-1);

                            std::string assignmentDestination($2);
                            assignmentQuadruple(assignmentDestination);
                        }
                        |
                        FLOAT_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($2, currentScope, FLOAT_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);

                            ret = assignVariable($2, $4, currentScope);
                            if (ret == -1) exit(-1);

                            std::string assignmentDestination($2);
                            assignmentQuadruple(assignmentDestination);
                        }
                        |
                        CHAR_DECLARATION  ID '=' expression ';'
                        {
                            int ret = createVariable($2, currentScope, CHAR_TYPE, !IS_CONSTANT,0);
                            if (ret == -1) exit(-1);

                            ret = assignVariable($2, $4, currentScope);
                            if (ret == -1) exit(-1);

                            std::string assignmentDestination($2);
                            assignmentQuadruple(assignmentDestination);

                        }



assignment: ID '=' expression {
                                int ret = assignVariable($1, $3, currentScope);
                                std::string assignmentDestination($1);
                                assignmentQuadruple(assignmentDestination);
                                if (ret == -1) exit(-1);
}

expression : math_expr {$$=$1;}|
                boolean_expr {$$=$1;}

  /* mathematical expression */
math_expr : INTEGER 
            {
                $$ = createExpressionMacro;
                $$->type = INT_TYPE;
                $$->intValue = $1;
                $$->valueIsValid = 1;

                std::string operandToPush(std::to_string($1));
                pushToStack(operandToPush);
            }
            |
            FLOAT 
            {
                $$ = createExpressionMacro;
                $$->type = FLOAT_TYPE;
                $$->doubleValue = $1;
                $$->valueIsValid = 1;

                std::string operandToPush(std::to_string($1));
                pushToStack(operandToPush);
           }
            |
            ID 
            {
                // get variable value from sybmol table and handle the error
                // create a new expression data
                $$ = createExpressionMacro;
                int variableType = getVariableType($1, currentScope);

                if (variableType == -1)
                {
                    // means that the variable was not found
                    printf("Variable %s not found\n", $1);
                    free($$);
                    exit(-1);
                }
                $$->type = variableType;
                // we will treat all variables as if their values were unknown
                // even if they are assigned i.e , we won't store the value
                // of int x = 3;
                $$->valueIsValid = 0;
                // since value is not valid no need to assign a value
                // as no operations will be done
                // only the type will be propagated.
    
               std::string operandToPush($1);
                pushToStack(operandToPush);
            }
            |
            CHARACTER 
            {
                $$ = createExpressionMacro;
                $$->type = CHAR_TYPE;
                $$->charValue = $1;          
                $$->valueIsValid = 1;

                std::string operandToPush(1, $1);
                operandToPush = "'" + operandToPush + "'";
                pushToStack(operandToPush);
            }
            |
            function_call 
            {
                // if you've reached this point, this means that the function
                // exists and parameters are satisfied
                // so , we'll need to return the the return type of the function
                $$ = $1;       
            }
            |
            math_expr '+' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, ADD_OP);
               executeBinaryOperation("+", currentTempCount);
               currentTempCount++;
               if (! $$) exit(-1);
            }
            |
            math_expr '-' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, SUB_OP);
               executeBinaryOperation("-", currentTempCount);
               currentTempCount++;
               if (! $$) exit(-1);
            }
            |
            math_expr '*' math_expr 
            { 
               $$ = binaryMathExpression($$, $1, $3, MUL_OP);
               executeBinaryOperation("*", currentTempCount);
               currentTempCount++;
               if (! $$) exit(-1);

            }
            |
            math_expr '/' math_expr 
            {
               $$ = binaryMathExpression($$, $1, $3, DIV_OP);
               executeBinaryOperation("/", currentTempCount);
               currentTempCount++;
                if (! $$) exit(-1);

            }
            |
            math_expr '%' math_expr 
            {
                
               $$ = binaryMathExpression($$, $1, $3, MOD_OP);
               executeBinaryOperation("%", currentTempCount);
               currentTempCount++;
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
                $$ = unaryMinusMathExpression($2);
                executeUnaryOperation("-", currentTempCount);
                currentTempCount++;
                if (!$$) exit(-1);
            }


 /* logical expression */

boolean_expr : expression '>' expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, GT_OP);  
                   if ($$) {
                        executeBinaryOperation(">", currentTempCount);
                        currentTempCount++;
                    }
                }
                |
                expression '<' expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, LT_OP);
                   if ($$) {
                        executeBinaryOperation("<", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
                expression ">=" expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, GEQ_OP);
                   if ($$){
                        executeBinaryOperation(">=", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
                expression "<=" expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, LT_OP);
                   if ($$){
                        executeBinaryOperation("<=", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
               expression "==" expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, EQ_OP);
                   if ($$){
                        executeBinaryOperation("==", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
                expression "!=" expression 
                {
                   $$ = binaryLogicalExpression($$, $1, $3, NEQ_OP);
                   if ($$){
                        executeBinaryOperation("!=", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
                expression "||" expression 
                {
                    $$ = binaryLogicalExpression($$, $1, $3, OR_OP);
                    if ($$){
                        executeBinaryOperation("||", currentTempCount);
                        currentTempCount++;
                   }
                }
                |
                expression "&&" expression 
                {
                    $$ = binaryLogicalExpression($$, $1, $3, AND_OP);
                    if ( $$) {
                        executeBinaryOperation("&&", currentTempCount);
                        currentTempCount++;
                    }
                }
                |
                '!' expression 
                {
                    $$ = notLogicalOperation($2);
                    if ($$){
                        executeUnaryOperation("!", currentTempCount);
                        currentTempCount++;
                    }
                } 
                |
                '(' boolean_expr ')'   
                {
                    
                    $$ = $2;
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
        BREAK ';' | //TODO: add break and CONTINUE
        CONTINUE ';' |
        RETURN ';' {int result = checkReturn(currentScope,VOID_TYPE); }|
        RETURN expression ';' 
        {
            int result = checkReturn(currentScope,$2->type);
            if (result != -1)
                returnQuadruple();

        }
        |
        block |
        ';'

 /* Loops */
optional_expression : expression {$$ = $1;}|{$$ = NULL; pushToStack("1");}

for_assignment: INT_DECLARATION ID '=' expression {
                                int ret = createVariable($2, currentScope, INT_TYPE, !IS_CONSTANT,0);
                                if (ret == -1) exit(-1);
                                ret = assignVariable($2, $4, currentScope);
                                std::string assignmentDestination($2);
                                assignmentQuadruple(assignmentDestination);
                                if (ret == -1) exit(-1);} | assignment |

//TODO:loops and check expression is in currentScope and defined before
loop: WHILE {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount;} 
        '(' expression ')' {checkIsChar($4->type,yylineno); 
        createNewScope(currentScope,scopeCount); currentScope = ++scopeCount; createLabel(currentScope);
        conditionLabelIfNot(getParentScope(currentScope));} 
        block {
        jumpLabel(currentScope); currentScope = getParentScope(currentScope);
        createLabel(currentScope); currentScope = getParentScope(currentScope);}|

        FOR {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount; createLabel(currentScope);} 
        '(' for_assignment ';' optional_expression ';' for_assignment ')' {checkIsCharFor($6,yylineno);}
        block {exitLabel(currentScope); currentScope = getParentScope(currentScope);}|

        DO {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount; createLabel(currentScope);} 
        block WHILE '(' expression ')' ';' {checkIsChar($6->type,yylineno); exitLabel(currentScope); currentScope = getParentScope(currentScope);}

 /* Conditional statements */

conditional: switch_case |
            if_statement


if_statement: IF {createNewScope(currentScope,scopeCount); currentScope = ++scopeCount;}  '(' expression  ')' 
                 {checkIsChar($4->type,yylineno); 
                 createNewScope(currentScope,scopeCount); 
                 currentScope = ++scopeCount; 
                 conditionLabelIfNot(currentScope);} 
                 stmt 
                 {jumpLabel(getParentScope(currentScope)); 
                 createLabel(currentScope); 
                 currentScope = getParentScope(currentScope);
                 } end_if 

end_if: %prec IFX  { 
                 createLabel(currentScope); 
                 currentScope = getParentScope(currentScope);
                 } | ELSE  stmt { 
                 createLabel(currentScope); 
                 currentScope = getParentScope(currentScope);
                 } 

switch_case: SWITCH '(' ID ')' '{'{createNewScope(currentScope,scopeCount);currentScope = ++scopeCount;} case_list '}' {currentScope = getParentScope(currentScope);}
//TODO: switch case
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

    //printSymbolTable();
    printQuadrupleTable();
}

