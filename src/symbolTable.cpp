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

struct parametersList {
    // x( int y )
    // will cause us to store y : INT_TYPE in the map
    // key is variable name, value is type
    std::unordered_map<std::string, int> parameterTypeMap;
};


struct functionStruct
{
    std::vector<parametersList> parametersList;    
    std::vector<int> returnTypes;
};

// TODO: Create Scope zero before start of program
std::unordered_map<int, scopeStruct> scopeTable;
std::unordered_map<std::string, functionStruct> functionTable;
