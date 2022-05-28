#pragma once
#include "dataStructures.h"

#ifdef __cplusplus
extern "C" {
#endif
    int createVariable(char * name, int scope, int type, int isConstant, int isAssigned);
    int assignVariable(char * name, struct ExpressionData* value, int scope);
    int getVariableType(char * name, int scope);
    void printSymbolTable();
    // void print_operation(char* operation);
    // float get_value(char* name, int &flag); 
    // int assign_int(char* name, int val);
    // int assign_float(char* name, float val);
    // int assign_char(char* name, char val);
    // int assign_string(char* name, char* val);
    // int assign_value(char* name,float val);
    // void print_table();
    // char get_value_c(char* name, int& flag);
    // char* get_value_s(char* name, int& flag);
    void createNewScope(int currentScope,int scopeCount);
    int addParameter(char* name, int type, int scope);
    void createNewFunction(int currentScope,char* name , int returnType);
    int getParentScope(int currentScope);
    int checkFunctionExists(int functionScope , int returnType);
#ifdef __cplusplus
    }
#endif