#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    int createVariable(char * name, int scope, int type, int isConstant);
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
#ifdef __cplusplus
    }
#endif