#pragma once

#define createExpressionMacro (struct ExpressionData*) malloc(sizeof(struct ExpressionData))

#include "dataStructures.h"
#include "constants.h"
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


    if (lType == FLOAT_TYPE || rType == FLOAT_TYPE)
    {
        returnOp->type = FLOAT_TYPE;
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
            printf("Two operators of mod operator - % - must be of type integer\n");
            break;
        default:
            printf("Error: Invalid operation\n");
            return NULL;
            break;
        }
    }
    else
    {
        returnOp->type = INT_TYPE;
        
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

    free(leftOp);
    free(rightOp);

    return returnOp;
}

