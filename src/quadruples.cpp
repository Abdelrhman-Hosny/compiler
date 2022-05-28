#include "quadruples.h"
#include <iostream>
#include <stack>
#include <vector>


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

void printQuadruple(QuadrupleStruct quad)
{
    std::cout << "(" << quad.operand1 << ", " << quad.operand2 << ", " << quad.operation << ", " << quad.destination << ")" << std::endl;
}

void printQuadrupleTable()
{
    std::cout << "Quadruples Table:" << std::endl;
    std::cout << "Op1, Op2, Op, Dest" << std::endl;
    for (int i = 0; i < quadruplesTable.size(); i++)
    {
        printQuadruple(quadruplesTable[i]);
    }
}

void executeUnaryOperand(char * operand)
{
    std::string operandName(operand);

}


