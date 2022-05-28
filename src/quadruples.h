#pragma once




#ifdef __cplusplus
#include <string>
extern "C" {
#endif

struct QuadrupleStruct
{
    std::string operand1;
    std::string operand2;
    std::string operation;
    std::string destination;
};

void pushToStack(std::string variable);
void executeBinaryOperation(std::string operation, int tempVariablesCount);
void printQuadrupleTable();
void executeUnaryOperation(std::string operation, int tempVariablesCount);
void assignmentQuadruple(std::string operand);


#ifdef __cplusplus
    }
#endif