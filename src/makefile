lexer:
	flex lex.l

parser:
	yacc -d parse.y

compile:
	gcc lex.yy.c y.tab.c -o a.out