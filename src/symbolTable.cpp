#include "symbolTable.h"
#include "constants.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>



struct variableData {
    int type;
    int isAssigned;
    int isConstant;

};

struct scopeStruct
{

    std::unordered_map<std::string, variableData> variables;
    int parentScope;
    std::string functionName = "";

};

struct parameterListStruct {
    // x( int y )
    // will cause us to store y : INT_TYPE in the map
    // key is variable name, value is type
    std::map<std::string, int> parameterTypeMap;
};


struct functionStruct
{
    //std::vector<parameterListStruct> parametersList;  
    //(scope as key to add the right parameters , parametersList as value)
    std::unordered_map<int,parameterListStruct> parametersListMap;  
    //std::vector<int> returnTypes;
    //(scope as key to add the right parameters , returnTypesMap as value)
    std::unordered_map<int,int> returnTypesMap;


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
        auto it3 = it->second.returnTypesMap.begin();
        for (auto it2 = it->second.parametersListMap.begin(); it2 != it->second.parametersListMap.end(); ++it2)
        {
            std::cout << "Return Type: " << it3->second << std::endl;
            std::cout << "Parameters: ";
            for (auto it3 = it2->second.parameterTypeMap.begin(); it3 != it2->second.parameterTypeMap.end(); ++it3)
            {
                std::cout << it3->first << " : " << it3->second << " ";
            }
            std::cout << std::endl;


            ++it3;
        }
   }
}


void createNewFunction(int currentScope,char* name , int returnType)
{
    std::string functionName(name);
    std::cout << "Creating new function in scope " << functionName << std::endl;
    int newScope = ++currentScope;
    scopeTable[newScope].functionName = functionName;
    functionTable[functionName].returnTypesMap[newScope] = returnType;
}

void addParameter(char* name, int type, int scope)
{
    std::string parameterName(name);
    scope++;
    std::string functionName = scopeTable[scope].functionName;
    std::cout << "Adding parameter " << parameterName << " in scope " << functionName << std::endl;
    functionTable[functionName].parametersListMap[scope].parameterTypeMap[parameterName] = type;
}

int checkFunctionExists(int functionScope , int returnType)
{
    //as we will know that the function is in the next currentScope
    functionScope++;
    std::string functionName = scopeTable[functionScope].functionName;
    std::vector <int> similarReturnTypesScopes;
    if(functionTable[functionName].returnTypesMap.size() == 1)
        return 1;
    //if the function has multiple return types we will check if the return type is similar to ones that written Before
    for (auto it = functionTable[functionName].returnTypesMap.begin(); it != functionTable[functionName].returnTypesMap.end(); ++it)
    {
        if(it->second == returnType && it->first != functionScope)
            similarReturnTypesScopes.push_back(it->first);
    }
    //if there is no similar return type we will return 1 which means that the function is not defined
    if(similarReturnTypesScopes.size() == 0)
        return 1;
    //if there is a similar return type we will check if the parameters are similar
    for (auto it = similarReturnTypesScopes.begin(); it != similarReturnTypesScopes.end(); ++it)
    {
        //first we will check if the parameters have the same size
        if(functionTable[functionName].parametersListMap[*it].parameterTypeMap.size() == functionTable[functionName].parametersListMap[functionScope].parameterTypeMap.size())
        {
            //if the parameters have the same size we will check if the parameters are similar
            bool isSimilar = true;
            auto startOfCurrentFunctionParameterMap = functionTable[functionName].parametersListMap[functionScope].parameterTypeMap.begin();
            for (auto it2 = functionTable[functionName].parametersListMap[*it].parameterTypeMap.begin(); it2 != functionTable[functionName].parametersListMap[*it].parameterTypeMap.end(); ++it2)
            {
                //that means there exists a parameter that is not similar so we will break
                 if(it2->second != startOfCurrentFunctionParameterMap->second)
                 {
                     isSimilar = false;
                     break;
                 }
                 startOfCurrentFunctionParameterMap++;
            }
            //if the parameters are similar we will return 0 which means that the function is defined
            if(isSimilar)
            {
                 printf("Error: Cannot have two functions that have same parameter and return value.\n");
                 return 0;
            }
        }
    }

    return 1;
    
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


