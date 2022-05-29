#pragma once

#define createExpressionMacro (struct ExpressionData*) malloc(sizeof(struct ExpressionData))

#include "dataStructures.h"
#include <stdio.h>
#include <cstdlib>

struct ExpressionData * binaryMathExpression(struct ExpressionData * returnOp,
                     struct ExpressionData * leftOp,
                    struct ExpressionData * rightOp,
                    int operation)
{
    int lType = leftOp->type;
    int rType = rightOp->type;

    if ( lType == CHAR_TYPE || rType == CHAR_TYPE)
    {
        printf("Error: Cannot perform arithmetic operation on char\n");
        return NULL;
    }

    returnOp = createExpressionMacro;

    // handle Type of return
    if (lType == FLOAT_TYPE || rType == FLOAT_TYPE)
        returnOp->type = FLOAT_TYPE;
    else
        returnOp->type = INT_TYPE;
   
    // handle Validity of return
    if (leftOp->valueIsValid == 0 || rightOp->valueIsValid == 0)
        returnOp->valueIsValid = 0;
    else
        returnOp->valueIsValid = 1;

    

    // handle Value of return
    if (leftOp->valueIsValid == 1 && rightOp->valueIsValid == 1)
    { 
        if (lType == FLOAT_TYPE || rType == FLOAT_TYPE)
        {
            switch (operation)
            {
            case ADD_OP:
                returnOp->doubleValue = getNumValue(leftOp) + getNumValue(rightOp);
                break;

            case SUB_OP:
                returnOp->doubleValue = getNumValue(leftOp) - getNumValue(rightOp);
                break;
            case MUL_OP:
                returnOp->doubleValue = getNumValue(leftOp) * getNumValue(rightOp);
                break;
            case DIV_OP:
                if (getNumValue(rightOp) == 0)
                {
                    printf("Error: Division by zero\n");
                    return NULL;
                }
                returnOp->doubleValue = getNumValue(leftOp) / getNumValue(rightOp);
                break; 
            case MOD_OP:
                printf("Error: Two operators of mod operator - \% - must be of type integer\n");
                break;
            default:
                printf("Error: Invalid operation\n");
                return NULL;
                break;
            }
        }
        else
        {
            
            switch (operation)
            {
            case ADD_OP:
                returnOp->intValue = leftOp->intValue + rightOp->intValue;
                break;
            case SUB_OP:
                returnOp->intValue = leftOp->intValue - rightOp->intValue;
                break;
            case MUL_OP:
                returnOp->intValue = leftOp->intValue * rightOp->intValue;
                break;
            case DIV_OP:
                if (rightOp->intValue == 0)
                {
                    printf("Error: Division by zero\n");
                    return NULL;
                }
                returnOp->intValue = leftOp->intValue / rightOp->intValue;
                break;
            case MOD_OP:
                if (rightOp->intValue == 0)
                {
                    printf("Error: Division by zero\n");
                    return NULL;
                }
                returnOp->intValue = leftOp->intValue % rightOp->intValue;
                break; 
            default:
                printf("Error: Invalid operation\n");
                return NULL;
                break;
            }
                                                        
        }
    } 
    else if (rightOp->valueIsValid)
    {
        double numValue = getNumValue(rightOp);

        if (operation == DIV_OP || operation == MOD_OP)
            if (numValue == 0)
            {
                printf("Error: Division by zero\n");
                return NULL;
            }

        if (rType == FLOAT_TYPE && operation == MOD_OP)
        {
            printf("Error: Mod operator can only be done on operands of type integer\n");
            return NULL;
        }  
    }

    free(leftOp);
    free(rightOp);

    return returnOp;
}


struct ExpressionData * binaryLogicalExpression(struct ExpressionData * returnOp,
                     struct ExpressionData * leftOp,
                    struct ExpressionData * rightOp,
                    int operation)
{
    int lType = leftOp->type;
    int rType = rightOp->type;

    if ( lType == CHAR_TYPE || rType == CHAR_TYPE)
    {
        if (lType != rType)
        {
            printf("Error: Cannot compare a char with a non char\n");
            return NULL;
        }
    }

    returnOp = createExpressionMacro;

    // return type is always int (1 or 0)
    returnOp->type = INT_TYPE;

    // handle validity
    if (leftOp->valueIsValid == 1 && rightOp->valueIsValid == 1)
        returnOp->valueIsValid = 1;
    else
        returnOp->valueIsValid = 0;

    // handle value
    if (leftOp->valueIsValid == 1 && rightOp->valueIsValid == 1)
    {
        switch (operation)
        
        {
        case AND_OP:

            returnOp->intValue = (getNumValue(leftOp) && getNumValue(rightOp));
            break;
        case OR_OP:

            returnOp->intValue = (getNumValue(leftOp) || getNumValue(rightOp));
            break;
        case GEQ_OP:

            returnOp->intValue = (getNumValue(leftOp) >= getNumValue(rightOp));
            break;
        case LEQ_OP:
            
            returnOp->intValue = (getNumValue(leftOp) <= getNumValue(rightOp));
            break;
        case EQ_OP:

            returnOp->intValue = (getNumValue(leftOp) == getNumValue(rightOp));
            break; 
        
        case LT_OP:

            returnOp->intValue = (getNumValue(leftOp) < getNumValue(rightOp));
            break;
        
        case GT_OP:

            returnOp->intValue = (getNumValue(leftOp) > getNumValue(rightOp));
            break;
        default:
            printf("Error: Invalid operation\n");
            return NULL;
            break;

        }

    }

    free(leftOp);
    free(rightOp);

    return returnOp;
}

struct ExpressionData * unaryMinusMathExpression(struct ExpressionData * leftOp)
{

    if (leftOp->valueIsValid == 1)
    {
        if (leftOp->type == INT_TYPE)
            leftOp->intValue = -leftOp->intValue;
        else if (leftOp->type == FLOAT_TYPE)
            leftOp->doubleValue = -leftOp->doubleValue;
        else
        {
            printf("Error: Unary minus can only be applied to int or float\n");
            return NULL;
        }
    }

    // if it were valid it would still be valid
    // if it were an int, it would be still be an int (same for float)
    // so not required to handle any other errors.

    return leftOp;

}

struct ExpressionData * notLogicalOperation(struct ExpressionData * leftOp)
{
    int lType = leftOp->type;

    struct ExpressionData * returnOp = createExpressionMacro;

    returnOp->type = INT_TYPE;
    
    if (leftOp->valueIsValid == 1)
    {
        if (lType == INT_TYPE)
        {
            returnOp->intValue = !leftOp->intValue;
        }
        else if (lType == FLOAT_TYPE)
        {
            returnOp->intValue = !leftOp->doubleValue;
        }
        else
        {
            printf("Error: Not operator can only be applied to int or float\n");
            return NULL;
        }
    } 

    free(leftOp);

    return returnOp;
}