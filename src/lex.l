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
['][a-zA-Z][']     return CHARACTER;
[a-zA-Z_][a-zA-Z0-9_]* return ID;
[0-9]+ return INTEGER;
[0-9]+\.[0-9]+ return FLOAT;
[ \t]+ ; // ignore whitespace
\n    {yylineno++;}
.   yyerror("unknown character");
%%

int yywrap() {
    return 1;
}
