#pragma once
#include "constants.h"
struct ExpressionData
{
    int type;
    int intValue;
    double doubleValue;
    char charValue;
};

inline double getNumValue(struct ExpressionData* data)
{
    return (data->type == INT_TYPE)? (double) data->intValue : data->doubleValue;
}