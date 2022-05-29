#pragma once




#ifdef __cplusplus
#include <string>
#include "symbolTable.h"
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
void createLabel(int labelNumber);
void exitLabel(int labelNumber);
void jumpLabel(int labelNumber);
void conditionLabelIfNot(int labelNumber);
int functionCallQuadruple(char * functionName, FunctionCallParameters * functionCallParameters, int functionReturnType, int currentTemporaryVariable);
void returnQuadruple();
void returnEmptyQuadruple();
void functionDeclarationQuadruple(std::string functionName, int functionScope);
void printQuadruplesTableToFile();
void executeBinaryOperationSwitchCase(std::string operandName, int tempVariablesCount);

#ifdef __cplusplus
    }
#endif