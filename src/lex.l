%{
#include "parse.tab.h"
#include <stdio.h>

int yylineno;
void yyerror(char *s);
%}

%%
"if"              return IF;
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
['][a-zA-Z][']     {yylval.charValue = yytext[1]; return CHARACTER;}
[a-zA-Z_][a-zA-Z0-9_]* { yylval.identifierName = yytext; return ID;}
[0-9]+ { yylval.intValue = atoi(yytext); return INTEGER;}
[0-9]+\.[0-9]+ { yylval.doubleValue = atof(yytext); return FLOAT;}
[ \t]+ ; // ignore whitespace
\n    {yylineno++;}
[<>=!]= {return yytext; printf("found %s\n", yytext);}
[-+*/%=<>!;{}()] {return yytext[0];}
.   {yyerror("unknown character"); printf("%s\n", yytext);}
%%

int yywrap() {
    return 1;
}
