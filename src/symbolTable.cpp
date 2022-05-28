#include "symbolTable.h"
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

int getScopeOfVariable(std::string name, int currentScope)
{
    struct scopeStruct currentScopeStruct = scopeTable[currentScope];

    if (currentScopeStruct.variables.find(name) != currentScopeStruct.variables.end())
    {
        return currentScope;
    }
    else
    {
        // TODO : Make the global scope have a parent of -1
        if (currentScopeStruct.parentScope == -1)
            // means that currentScope = global scope and we couldn't find the variable there
            return -1;
        else
            return getScopeOfVariable(name, currentScopeStruct.parentScope);
        
    }
}

int createVariable(char *name, int scope, int type ,int isConstant , int isAssigned)
{
    std::string variableName(name);

    std::cout << "Creating variable " << variableName << " in scope " << scope << std::endl;

    // Check if variable already exists in this scope
    if (scopeTable[scope].variables.find(variableName) != scopeTable[scope].variables.end())
    {
        std::cout << "Error: Variable " << variableName << " already exists in scope " << scope << std::endl;
        return -1;
    }

    // varData : type, isAssigned, isConstant
    variableData varData = {type, isAssigned, isConstant};
    scopeTable[scope].variables[variableName] =  varData;
    
    return 1;
}

int assignVariable(char *name, struct ExpressionData * expressionData, int scope)
{
    // when we assign a variable, we have to handle
    // scopes, constants, variable types and isAssigned
    std::string variableName(name);

    // handling scope
    int scopeOfVariable = getScopeOfVariable(variableName, scope);

    if (scopeOfVariable == -1)
    {
        std::cout << "Error: Variable " << variableName << " does not exist in scope " << scope << std::endl;
        return -1;
    }

    struct variableData varData = scopeTable[scopeOfVariable].variables[variableName];


    // handling types
    // check that types are the same
    // if variable is double and expression is int, it can be assigned
    if (varData.type != expressionData->type && !(varData.type == FLOAT_TYPE && expressionData->type == INT_TYPE))
    {
        std::cout << "Error: Variable " << variableName << " in scope " << scope << " has type " << varData.type << " and cannot be assigned type " << expressionData->type << std::endl;
        return -1;
    }


    // handling constants
    if (varData.isConstant == 1)
    {
        if (varData.isAssigned == 0)
        {
            varData.isAssigned = 1;

        } else {

            std::cout << "Error : Variable " << variableName << " is constant in scope " << scope << std::endl;
            return -1;
        }
    }

    scopeTable[scopeOfVariable].variables[variableName] = varData;
   return 1; 
}

int getVariableType(char * name, int currentScope)
{
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

int addParameter(char* name, int type, int scope)
{
    std::string parameterName(name);
    scope++;
    std::string functionName = scopeTable[scope].functionName;
    std::cout << "Adding parameter " << parameterName << " in scope " << functionName << std::endl;
    functionTable[functionName].parametersListMap[scope].parameterTypeMap[parameterName] = type;
    int isDuplicated = createVariable(name, scope, type, 0, 1);
    if(isDuplicated == -1)
    {
        return 0;
    }
    return isDuplicated;
}


int checkFunctionExists(int functionScope , int returnType)
{
    //as we will know that the function is in the next currentScope
    functionScope++;
    std::string functionName = scopeTable[functionScope].functionName;
    if(functionTable[functionName].returnTypesMap.size() == 1)
        return 1;
    //loop over all functions that have the same number and compare the parameters
    for (auto it = functionTable[functionName].parametersListMap.begin(); it != functionTable[functionName].parametersListMap.end(); ++it)
    {
        //first we will check if the parameters have the same size
        if(it->first != functionScope && it->second.parameterTypeMap.size() == functionTable[functionName].parametersListMap[functionScope].parameterTypeMap.size())
        {
            //if the parameters have the same size we will check if the parameters are similar
            bool isSimilar = true;
            auto startOfCurrentFunctionParameterMap = functionTable[functionName].parametersListMap[functionScope].parameterTypeMap.begin();
            for (auto it2 = it->second.parameterTypeMap.begin(); it2 != it->second.parameterTypeMap.end(); ++it2)
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
