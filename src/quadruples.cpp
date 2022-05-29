#include "quadruples.h"
#include "symbolTable.h"
#include <iostream>
#include <stack>
#include <vector>
#include <fstream>


std::stack<std::string> expressionStack;

std::vector<QuadrupleStruct> quadruplesTable;

void pushToStack(std::string expressionName)
{
    expressionStack.push(expressionName);
}


std::string popStack()
{
    std::string expressionName = expressionStack.top();
    expressionStack.pop();

    return expressionName;
}

void executeBinaryOperation(std::string operandName, int tempVariablesCount)
{
    std::cout << "Executing binary operation: " << operandName << std::endl;
    std::cout << "Stack length : " << expressionStack.size() << std::endl;
    std::string operand1 = popStack();

    std::string operand2 = popStack();

    std::string destination = "t" + std::to_string(tempVariablesCount);

    QuadrupleStruct quadruple = {operand2, operand1, operandName, destination};

    pushToStack(destination);
    quadruplesTable.push_back(quadruple);
    
}

void executeUnaryOperation(std::string operandName, int tempVariablesCount)
{
    std::cout << "Executing unary operation: " << operandName << std::endl;
    std::cout << "Stack length : " << expressionStack.size() << std::endl;
    std::string operand1 = popStack();

    std::string destination = "t" + std::to_string(tempVariablesCount);

    QuadrupleStruct quadruple = {operand1, "", operandName, destination};

    pushToStack(destination);
    quadruplesTable.push_back(quadruple);
    
}


void assignmentQuadruple(std::string destination)
{
    std::cout << "Executing assignment: " << destination << std::endl;
    std::cout << "Stack length : " << expressionStack.size() << std::endl;
    std::string operand1 = popStack();

    QuadrupleStruct quadruple = {operand1, "", "=", destination};

    quadruplesTable.push_back(quadruple);
}


int functionCallQuadruple(char * funcName, FunctionCallParameters * functionCallParameters, int functionReturnType, int currentTemporaryVariable)
{
    std::string functionName(funcName);
    int argumentsCount = functionCallParameters->parameterTypes.size();
    for (int i = 0; i < argumentsCount; i++)
    {
        std::string parameter = popStack();
        QuadrupleStruct quadruple = { "", "", "" , ""};
        quadruple.operation = "arg";
        quadruple.operand1 = parameter;
        quadruplesTable.push_back(quadruple);
    }

    std::string destination = (functionReturnType == VOID_TYPE) ? "" : "t" + std::to_string(currentTemporaryVariable);
    currentTemporaryVariable = (functionReturnType == VOID_TYPE) ? currentTemporaryVariable : currentTemporaryVariable + 1;

    if (functionReturnType != VOID_TYPE) pushToStack(destination);

    QuadrupleStruct quadruple = {functionName, std::to_string(argumentsCount), "call", destination};
    quadruplesTable.push_back(quadruple);
    return currentTemporaryVariable;
}


void printQuadruple(QuadrupleStruct quad)
{
    std::cout << "("  << quad.destination << ", " << quad.operand1 << ", " << quad.operation << ", " << quad.operand2 << ")" << std::endl;
}

void printQuadrupleTable()
{
    std::cout << "Quadruples Table:" << std::endl;
    std::cout << "Dest, Op1 , Operation , Op2" << std::endl;
    for (int i = 0; i < quadruplesTable.size(); i++)
    {
        printQuadruple(quadruplesTable[i]);
    }
}

void createLabel(int labelNumber)
{
    QuadrupleStruct quadruple = {"", "", "label", "L" + std::to_string(labelNumber)};
    quadruplesTable.push_back(quadruple);
}

void exitLabel(int labelNumber)
{
    std::string expressionStack = popStack();
    QuadrupleStruct quadruple = {"if", expressionStack, "goto", "L" + std::to_string(labelNumber)};
    quadruplesTable.push_back(quadruple);
}

void jumpLabel(int labelNumber)
{
    QuadrupleStruct quadruple = {"","", "goto", "L" + std::to_string(labelNumber)};
    quadruplesTable.push_back(quadruple);
}

void conditionLabelIfNot(int labelNumber)
{
    std::string expressionStack = popStack();
    QuadrupleStruct quadruple = {"if not", expressionStack, "goto", "L" + std::to_string(labelNumber)};
    quadruplesTable.push_back(quadruple);
}

void returnQuadruple()
{
    std::string expressionStack = popStack();
    QuadrupleStruct quadruple = {"", "", "", ""};
    quadruple.operation = "return";
    quadruple.operand1 = expressionStack;
    quadruplesTable.push_back(quadruple);
}


void functionDeclarationQuadruple(std::string functionName, int functionScope)
{
    int numParameters = getNumParamsOfFunction(functionScope);

    QuadrupleStruct quadruple = {"", "", "", ""};
    quadruple.operation = "function";
    quadruple.operand1 = functionName;

    quadruplesTable.push_back(quadruple);

    for (int i = 0; i < numParameters; i++)
    {
        QuadrupleStruct quadruple = {"", "", "", ""};
        quadruple.operation = "param";
        quadruple.operand1 = popStack();
        quadruplesTable.push_back(quadruple);

    }
    
}

void printQuadruplesTableToFile()
{
    std::cout << "Printing quad table" << std::endl;

    // print quadruples table into a file
    std::ofstream myfile;
    myfile.open ("quadruples.txt");
    for (int i = 0; i < quadruplesTable.size(); i++)
    {
        printQuadruple(quadruplesTable[i]);
        myfile << "("  << quadruplesTable[i].destination << ", " << quadruplesTable[i].operand1 << ", " << quadruplesTable[i].operation << ", " << quadruplesTable[i].operand2 << ")" << std::endl;
    }
    myfile.close();

}
//TODO: switch
