lexer:
	flex lex.l

parser:
	bison -d parse.y

parser_counterexamples:
	bison -d parse.y -Wcounterexamples
compile:
	gcc lex.yy.c y.tab.c -o a.out