lexer:
	flex lex.l

parser:
	bison -d parse.y
build: parser lexer

parser_counterexamples:
	bison -d parse.y -Wcounterexamples
compile:
	g++ quadruples.cpp symbolTable.cpp lex.yy.c parse.tab.c -o a.out

all: build compile
