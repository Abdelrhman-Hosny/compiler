#pragma once
#define getNumValue(data) (data->type == INT_TYPE) ? 1.0 * data->intValue : data->doubleValue

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

struct CaseData
{
    int type;
    int intValue;
    char charValue;
    char* name = nullptr;
};
