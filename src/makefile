lexer:
	flex lex.l

parser:
	bison -d parse.y

parser_counterexamples:
	bison -d parse.y -Wcounterexamples
compile:
	gcc lex.yy.c parse.tab.c -o a.out