%{
#include "parse.tab.h"
#include <stdio.h>

int yylineno;
void yyerror(char *);
%}

%%
"if"              return IF;
"const"           return CONST_DECLARATION;
"else"            return ELSE;
"switch"          return SWITCH;
"case"            return CASE;
"default"         return DEFAULT;
"while"           return WHILE;
"do"              return DO;
"for"             return FOR;
"break"           return BREAK;
"continue"        return CONTINUE;
"return"          return RETURN;
"char"            return CHAR_DECLARATION;
"int"            return INT_DECLARATION;
"float"           return FLOAT_DECLARATION;
"void"              return VOID;
['][a-zA-Z][']     {strncpy(&yylval.charValue, &yytext[1], 1); return CHARACTER;}
[a-zA-Z_][a-zA-Z0-9_]* { yylval.identifierName = strdup(yytext); return ID;}
[0-9]+ { yylval.intValue = atoi(yytext); return INTEGER;}
[0-9]+\.[0-9]+ { yylval.doubleValue = strtof(yytext, NULL); return FLOAT;}
[ \t]+ ; // ignore whitespace
\n    {yylineno++;}
">="              return GEQ;
"<="             return LEQ;
"=="              return EQ;
"!="              return NEQ;
"&&"              return AND;
"||"              return OR;
[-+*/%=<>!;{}():,] {return yytext[0];}
[/][/].*\n {printf("comments\n");}
.   {yyerror("unknown character"); printf("%s\n", strdup(yytext));}
%%

int yywrap() {
    return 1;
}
