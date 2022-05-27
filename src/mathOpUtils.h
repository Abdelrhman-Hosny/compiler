#pragma once

#define createExpressionMacro (struct ExpressionData*) malloc(sizeof(struct ExpressionData))

#include "dataStructures.h"
#include "constants.h"
#include <iostream>

int binaryMathExpression(struct ExpressionData * returnOp,
                     struct ExpressionData * leftOp,
                    struct ExpressionData * rightOp,
                    int operation)
{
    int lType = leftOp->type;
    int rType = rightOp->type;

    if ( lType == CHAR_TYPE || rType == CHAR_TYPE)
    {
        std::cout << "Error: Cannot perform arithmetic operation on char" << std::endl;
        return -1;
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
                std::cout << "Error: Division by zero" << std::endl;
                return -1;
            }
            returnOp->doubleValue = getNumValue(leftOp) / getNumValue(rightOp);
            break; 
        case MOD_OP:
            std::cout << "Two operators of mod operator - % - must be of type integer" << std::endl;
            break;
        default:
            std::cout << "Error: Invalid operation" << std::endl;
            return -1;
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
                std::cout << "Error: Division by zero" << std::endl;
                return -1;
            }
            returnOp->intValue = leftOp->intValue / rightOp->intValue;
            break;
        case MOD_OP:
            if (rightOp->intValue == 0)
            {
                std::cout << "Error: Division by zero" << std::endl;
                return -1;
            }
            returnOp->intValue = leftOp->intValue % rightOp->intValue;
            break; 
        default:
            std::cout << "Error: Invalid operation" << std::endl;
            return -1;
            break;
        }
                                                     
    }

    free(leftOp);
    free(rightOp);

    return 1;
}

