#include "symbolTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
    int blockType = BLOCK_TYPE_OTHER;

};

struct parameterListStruct {
    // x( int y )
    // will cause us to store y : INT_TYPE in the map
    // key is variable name, value is type
    std::vector <int> parameterTypeList;
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

struct switchTableStruct{
    int type;
    std::unordered_set<std::string> caseList;
};


std::unordered_map<int, scopeStruct> scopeTable;
std::unordered_map<std::string, functionStruct> functionTable;
//Scope and an unordered set of the switch cases are in the scope
std::unordered_map<int, switchTableStruct> switchTable;

int getScopeOfVariable(std::string name, int currentScope)
{
    struct scopeStruct currentScopeStruct = scopeTable[currentScope];

    if (currentScopeStruct.variables.find(name) != currentScopeStruct.variables.end())
    {
        return currentScope;
    }
    else
    {
        if (currentScopeStruct.parentScope == currentScope)
            // means that currentScope = global scope and we couldn't find the variable there
            return -1;
        else
            return getScopeOfVariable(name, currentScopeStruct.parentScope);
        
    }
}

int createVariable(char *name, int scope, int type ,int isConstant , int isAssigned)
{
    std::string variableName(name);

    //std::cout << "Creating variable " << variableName << " in scope " << scope << std::endl;

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
    std::string variableName(name);

    int scopeOfVariable = getScopeOfVariable(variableName, currentScope);

    if (scopeOfVariable == -1)
    {
        std::cout << "Error: Variable " << variableName << " does not exist in scope " << currentScope << std::endl;
        return -1;
    }   

    return scopeTable[scopeOfVariable].variables[variableName].type;
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
            for (auto it3 = it2->second.parameterTypeList.begin(); it3 != it2->second.parameterTypeList.end(); ++it3)
            {
                std::cout << *it3<< " ";
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
    //std::cout << "Adding parameter " << parameterName << " in scope " << functionName << std::endl;
    functionTable[functionName].parametersListMap[scope].parameterTypeList.push_back(type);
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
        if(it->first != functionScope && it->second.parameterTypeList.size() == functionTable[functionName].parametersListMap[functionScope].parameterTypeList.size())
        {
            //if the parameters have the same size we will check if the parameters are similar
            bool isSimilar = true;
            auto startOfCurrentFunctionParameterMap = functionTable[functionName].parametersListMap[functionScope].parameterTypeList.begin();
            for (auto it2 = it->second.parameterTypeList.begin(); it2 != it->second.parameterTypeList.end(); ++it2)
            {
                //that means there exists a parameter that is not similar so we will break
                 if(*it2 != *startOfCurrentFunctionParameterMap)
                 {
                     isSimilar = false;
                     break;
                 }
                 startOfCurrentFunctionParameterMap++;
            }
            //if the parameters are similar we will return 0 which means that the function is defined
            if(isSimilar)
            {
                 printf("Error: Cannot have two functions that have same parameters.\n");
                 return 0;
            }
        }
    }
    return 1;
    
}

void createNewScope(int currentScope , int scopeCount,int blockType)
{
    int parentScope = currentScope;
    scopeCount++;
    currentScope = scopeCount;
    scopeTable[currentScope].parentScope = parentScope;
    scopeTable[currentScope].blockType = blockType;
}

int getParentScope(int currentScope)
{
    return scopeTable[currentScope].parentScope;
}


int checkArgumentList(char* name , FunctionCallParameters* ArgumentList)
{
    std::cout << "Checking Argument List: " << std::endl;
    std::string functionName(name);
    //we will check if the function exists
    if(functionTable.find(functionName) == functionTable.end())
    {
        printf("Error: Function %s does not exist.\n", name);
        return -1;
    }
    //we need to get scope of the function that have same number of parameters
    std::vector <int> scopeList;
    for (auto it = functionTable[functionName].parametersListMap.begin(); it != functionTable[functionName].parametersListMap.end(); ++it)
    {
        if(it->second.parameterTypeList.size() == ArgumentList->parameterTypes.size())
        {
            scopeList.push_back(it->first);
        }
    }
    //if there is no scope that has the same number of parameters we will return -1
    if(scopeList.size() == 0)
    {
        printf("Error: Function %s does not exist with that number of parameters\n", name);
        return -1;
    }
    //we will check if the parameters are similar in one of the scopes we found
    for (auto it = scopeList.begin(); it != scopeList.end(); ++it)
    {
        bool isSimilar = true;
        auto startOfCurrentFunctionParameterMap = functionTable[functionName].parametersListMap[*it].parameterTypeList.begin();
        for (auto it2 = ArgumentList->parameterTypes.begin(); it2 != ArgumentList->parameterTypes.end(); ++it2)
        {
            //that means there exists a parameter that is not similar so we will break
             if(*it2 != *startOfCurrentFunctionParameterMap)
             {
                 isSimilar = false;
                 break;
             }
             startOfCurrentFunctionParameterMap++;
        }
        //if the parameters are similar we will return 0 which means that the function is defined
        if(isSimilar)
        {
             printf("We found a function that matches one that we called\n");
             //return type of the function
             return functionTable[functionName].returnTypesMap[*it];
        }
    }
    //we didn't find a similar function so we will return -1
    printf("Error: Function %s does not exist with that same type of parameters\n", name);
    return -1;
}

int getFunctionType(int currentScope)
{
    struct scopeStruct currentScopeStruct = scopeTable[currentScope];
    if (currentScopeStruct.functionName != "")
    {
        return functionTable[currentScopeStruct.functionName].returnTypesMap[currentScope];
    }
    else
    {
        if (currentScopeStruct.parentScope == currentScope)
            // means that currentScope = global scope and we couldn't find the variable there
            return -1;
        else
            return getFunctionType(currentScopeStruct.parentScope);
    }
}

int checkReturn(int scope , int type)
{
    //we will check if the function exists
    int functionType = getFunctionType(scope);
    if(functionType == -1)
    {
        printf("Error: Function does not exist.\n");
        return -1;
    }
    if (functionType != type && !(functionType == FLOAT_TYPE && type == INT_TYPE))
    {
        std::cout << "different return type" << std::endl;
        return -1;
    }
    return 1;
}


int checkIsChar(int type, int numberLine){
    if(type == CHAR_TYPE)
    {
      printf("Error: if conditional can't be CHARACTER at line number: %d\n",numberLine); 
      exit(-1);
    }
    return 1;
}

int checkIsCharFor(ExpressionData* expressionData , int numberLine){ 
    if(expressionData == NULL)
    {
        return 1;
    }
    if(expressionData->type == CHAR_TYPE)
    {
      printf("Error: if conditional can't be CHARACTER at line number: %d\n" , numberLine); 
      exit(-1);
    }
    return 1;
}

int getIDTypeSwitch(char * name, int currentScope)
{   
    std::string variableName(name);
    int scopeOfVariable = getScopeOfVariable(variableName, currentScope);
    if (scopeOfVariable == -1)
    {
        std::cout << "Error: Variable " << variableName << " does not exist in scope " << currentScope << std::endl;
        return -1;
    }   
    if(scopeTable[scopeOfVariable].variables[variableName].isConstant == 0)
    {
        std::cout << "Error: Variable " << variableName << " in Switch Case must be constant "<< std::endl;
        return -1;
    }
    return scopeTable[scopeOfVariable].variables[variableName].type;

}

void createNewSwitch(int currentScope,int type){
    switchTable[currentScope].type = type;
}

void createNewCase(int currentScope,CaseData* value){
    //check if the value is of the same type as the switch
    if(switchTable[currentScope].type != value->type)
    {
        std::cout << "Error: Switch Case must be of the same type as the switch" << std::endl;
    }
    //check if the value is an ID check if it exists in set or not 
    if(value->name != nullptr)
    {
        std::string variableName(value->name);
        if(switchTable[currentScope].caseList.find(variableName) != switchTable[currentScope].caseList.end())
        {
            std::cout << "Error: Variable " << variableName << " Decalared before in the Switch Case" << std::endl;
        }
        else
        {
            switchTable[currentScope].caseList.insert(variableName);
        }
    }
    else
    {
        //here means I inserted an Integer or character
        std::string variableName;
        if(value->type == INT_TYPE)
        {
            variableName = std::to_string(value->intValue);
        }
        else
        {
            variableName = std::string(1,value->charValue);
        }

        if(switchTable[currentScope].caseList.find(variableName) != switchTable[currentScope].caseList.end())
        {
            std::cout << "Error: Variable " << variableName << " Decalared before in the Switch Case" << std::endl;
        }
        else
        {
            switchTable[currentScope].caseList.insert(variableName);
        }
    }
}


int isInSideLoop(int currentScope , std::string name)
{
    struct scopeStruct currentScopeStruct = scopeTable[currentScope];
    if (currentScopeStruct.blockType == BLOCK_TYPE_LOOPS)
    {
        return 1;
    }
    else
    {
        if (currentScopeStruct.parentScope == currentScope)
            // means that currentScope = global scope and we couldn't find the variable there
            {
                std::cout<<name<<" Must be inside a loop"<<std::endl;
                return -1;
            }
        else
            return isInSideLoop(currentScopeStruct.parentScope,name);
    }
}
