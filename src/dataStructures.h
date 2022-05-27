#pragma once

#include "constants.h"
#include <stdio.h>
struct ExpressionData
{
    int type;
    int intValue;
    double doubleValue;
    char charValue;
    int valueIsValid = 0;
};

double getNumValue(struct ExpressionData* data)
{
    double value = (data->type == INT_TYPE) ? data->intValue : data->doubleValue;
    return value;
}