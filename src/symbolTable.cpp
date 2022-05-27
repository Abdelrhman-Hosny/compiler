#include "symbolTable.h"
#include "constants.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>


struct variableData {
    int type;
    int isAssigned;
    int isConstant;

};

struct scopeStruct
{

    std::unordered_map<std::string, variableData> variables;
    int parentScope;

};

struct parameterListStruct {
    // x( int y )
    // will cause us to store y : INT_TYPE in the map
    // key is variable name, value is type
    std::unordered_map<std::string, int> parameterTypeMap;
};


struct functionStruct
{
    std::vector<parameterListStruct> parametersList;    
    std::vector<int> returnTypes;
};

// TODO: Create Scope zero before start of program
std::unordered_map<int, scopeStruct> scopeTable;
std::unordered_map<std::string, functionStruct> functionTable;



int createVariable(char *name, int scope, int type ,int isConstant)
{
    std::string variableName(name);

    std::cout << "Creating variable " << variableName << " in scope " << scope << std::endl;

    // Check if variable already exists in this scope
    if (scopeTable[scope].variables.find(variableName) != scopeTable[scope].variables.end())
    {
        std::cout << "Variable " << variableName << " already exists in scope " << scope << std::endl;
        return -1;
    }

    // varData : type, isAssigned, isConstant
    variableData varData = {type, 0, isConstant};
    scopeTable[scope].variables[variableName] =  varData;
    
    return 1;
}

void printSymbolTable()
{
    std::cout << "Printing Symbol Table" << std::endl;
    for (auto it = scopeTable.begin(); it != scopeTable.end(); ++it)
    {
        std::cout << "Scope: " << it->first << std::endl;
        for (auto it2 = it->second.variables.begin(); it2 != it->second.variables.end(); ++it2)
        {
            std::cout << "Variable: " << it2->first << " Type: " << it2->second.type << " isAssigned: " << it2->second.isAssigned << " isConstant: " << it2->second.isConstant << std::endl;
        }
    }

    std::cout << "Printing Function Table" << std::endl;
    for (auto it = functionTable.begin(); it != functionTable.end(); ++it)
    {
        std::cout << "Function: " << it->first << std::endl;
        auto it3 = it->second.returnTypes.begin();
        for (auto it2 = it->second.parametersList.begin(); it2 != it->second.parametersList.end(); ++it2)
        {
            std::cout << "Return Type: " << *it3 << std::endl;
            std::cout << "Parameters: ";
            for (auto it3 = it2->parameterTypeMap.begin(); it3 != it2->parameterTypeMap.end(); ++it3)
            {
                std::cout << it3->first << " : " << it3->second << " ";
            }
            std::cout << std::endl;


            ++it3;
        }
   }
}


void createNewScope(int currentScope , int scopeCount)
{
    int parentScope = currentScope;
    scopeCount++;
    currentScope = scopeCount;
    scopeTable[currentScope].parentScope = parentScope;
}

int getParentScope(int currentScope)
{
    return scopeTable[currentScope].parentScope;
}


int createFunction(char * name, char* parameters,int returnType)
{
    std::string functionName(name);

    std::cout << "Creating function " << functionName << std::endl;

    // Check if function already exists in this scope
    if (functionTable.find(functionName) != functionTable.end())
    {
        std::cout << "Function " << functionName << " already exists in scope " << std::endl;
        return -1;
    }
    std::string parametersName(parameters);
    std::cout << "Parameters: " << parametersName << std::endl;
    return 1;
}